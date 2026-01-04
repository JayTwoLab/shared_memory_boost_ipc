#include "shared_ipc.hpp"

#include <boost/interprocess/exceptions.hpp>
#include <iostream>

// Guard to remove shared memory segment on destruction
class shared_segment_guard
{
public:
  explicit shared_segment_guard(const char* name)
    : name_(name)
  {}

  ~shared_segment_guard()
  {
    // boost::interprocess::shared_memory_object::remove(name_.c_str());
    // NOTICE: Do not delete shared memory at termination to allow readers to reconnect.
  }

private:
  std::string name_;
};

int main(int argc, char** argv)
{
  if (argc >= 2 && std::string(argv[1]) == "--remove")
  {
    std::cout << "[DEBUG] Removing shared memory segment...\n";
    bool removed = boost::interprocess::shared_memory_object::remove(ipc::k_segment_name); // remove shared memory segment
    std::cout << "remove shared memory: " << (removed ? "true" : "false") << "\n";
    return 0;
  }

  ipc::shutdown_flag stop_flag; // handle termination signals
  shared_segment_guard guard(ipc::k_segment_name); // ensure cleanup on exit

  std::cout << "[DEBUG] Creating or opening shared memory segment...\n";
  auto segment = boost::interprocess::managed_shared_memory(
      boost::interprocess::open_or_create,
      ipc::k_segment_name,
      ipc::k_segment_size); // create or open segment

  std::cout << "[DEBUG] Creating or finding shared object in segment...\n";
  auto* obj = segment.find_or_construct<ipc::shared_payload>(ipc::k_object_name)(); // create or find shared object

  std::uint64_t seq = 0; // sequence number

  std::cout << "writer started\n";

  while (!stop_flag.is_set()) // main loop
  {
    try {
      boost::interprocess::scoped_lock lock(obj->mutex); // lock the mutex

      if (seq == UINT64_MAX) { // rollover sequence
        std::cout << "[DEBUG] seq reached UINT64_MAX, rolling over to 0 and resetting shared object.\n";
        seq = 0;
        obj->sequence = 0;
        obj->unix_ms = 0;
        ipc::copy_cstr(obj->message, sizeof(obj->message), "");
      } else {
        std::cout << "[DEBUG] Acquired lock. Writing new sequence... (seq=" << seq+1 << ")\n";
        obj->sequence = ++seq; // increment and store sequence
        obj->unix_ms = ipc::now_unix_ms(); // store current time
        std::string msg = "hello from writer, seq=" + std::to_string(seq);
        ipc::copy_cstr(obj->message, sizeof(obj->message), msg.c_str()); // store message
      }
      
      std::cout << "[DEBUG] Notifying all readers.\n";
      obj->cond.notify_all(); // notify all waiting readers

    } catch (const std::exception& ex) {
      std::cout << "[ERROR] Exception during write: " << ex.what() << "\n";
    } catch (...) {
      std::cout << "[ERROR] Unknown exception during write.\n";
    }

    std::cout << "write: seq=" << seq << "\n";
    ipc::sleep_ms(500);
  }

  std::cout << "writer exiting\n";
  return 0;
}
