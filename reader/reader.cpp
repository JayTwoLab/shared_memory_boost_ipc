#include "shared_ipc.hpp" // NOLINT(modernize-deprecated-headers)

#include <boost/interprocess/exceptions.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <iostream>
#include <memory>

// Try to open existing shared memory segment
static std::unique_ptr<ipc::bip::managed_shared_memory> try_open()
{
  try
  {
    return std::make_unique<ipc::bip::managed_shared_memory>(
      ipc::bip::open_only,
      ipc::k_segment_name
    );
  }
  catch (...)
  {
    return nullptr;
  }
}

int main(int argc, char** argv)
{
  bool once = (argc >= 2 && std::string(argv[1]) == "--once"); // read only one message and exit

  ipc::shutdown_flag stop_flag; // not used in this example, but could be extended to handle signals
  std::uint64_t last_seq = 0; // last read sequence number
  int sleep_mil_sec = 300; // milliseconds
  
  std::cout << "reader started\n";

  while (!stop_flag.is_set()) // outer loop to reconnect
  {
    std::cout << "[DEBUG] Trying to open shared memory segment...\n";
    auto segment = try_open(); // attempt to open existing segment
    if (!segment)
    {
      std::cout << "[DEBUG] Shared memory segment not found. Retrying...\n";
      ipc::sleep_ms(sleep_mil_sec);
      continue;
    }

    std::cout << "[DEBUG] Shared memory segment opened. Looking for object...\n";
    auto found = segment->find<ipc::shared_payload>(ipc::k_object_name); // find shared object
    ipc::shared_payload* obj = found.first; // get pointer to shared object

    if (!obj) // object not found
    {
      std::cout << "[DEBUG] Shared object not found in segment. Retrying...\n";
      ipc::sleep_ms(sleep_mil_sec);
      continue;
    }

    std::cout << "[DEBUG] reader connected to shared object.\n";

    std::uint32_t last_magic = 0; // to detect object recreation
    std::uint32_t last_version = 0; // to detect object recreation
    while (!stop_flag.is_set()) // inner loop to read messages
    {
      try
      {
        std::cout << "[DEBUG] Top of inner loop. last_seq=" << last_seq << ", last_magic=" << last_magic << ", last_version=" << last_version << "\n";
        boost::interprocess::scoped_lock lock(obj->mutex); // lock the mutex
        std::cout << "[DEBUG] Acquired lock. Waiting for new sequence... (last_seq=" << last_seq << ")\n";

        // last_seq sync if object is newly created (magic/version change)
        if (obj->magic != last_magic || obj->version != last_version) {
          std::cout << "[DEBUG] Detected new shared object (magic/version changed). Sync last_seq. last_magic=" << last_magic << ", obj->magic=" << obj->magic << ", last_version=" << last_version << ", obj->version=" << obj->version << "\n";
          last_seq = obj->sequence; // sync to current sequence
          last_magic = obj->magic; // update magic
          last_version = obj->version; // update version
        }

        int wait_count = 0; // to track number of waits
        while (obj->sequence == last_seq && !stop_flag.is_set()) // wait for sequence change
        {
          std::cout << "[DEBUG] Waiting for sequence to change... (wait_count=" << wait_count << ", last_seq=" << last_seq << ", obj->sequence=" << obj->sequence << ")\n";

          // wait with timeout to periodically check stop_flag
          obj->cond.timed_wait(
            lock,
            boost::posix_time::microsec_clock::universal_time()
              + boost::posix_time::milliseconds(500));
          ++wait_count;
        }

        if (obj->sequence < last_seq) { // sequence reset detected
          std::cout << "[DEBUG] Detected sequence reset (writer restarted or shared memory recreated). last_seq=" << last_seq << ", obj->sequence=" << obj->sequence << "\n";
          last_seq = 0;
        }

        if (obj->sequence != last_seq) // new message available
        {
          std::cout << "[DEBUG] Sequence changed: " << last_seq << " -> " << obj->sequence << "\n";
          last_seq = obj->sequence;
          std::cout << "read: seq=" << obj->sequence
                    << ", unix_ms=" << obj->unix_ms
                    << ", message=\"" << obj->message << "\"\n";

          if (once)
            return 0;
        }
        
        std::cout << "[DEBUG] End of inner loop iteration.\n";
      }
      catch (const std::exception& ex)
      {
        std::cout << "[ERROR] Exception in inner loop: " << ex.what() << "\n";
        break;
      }
      catch (...)
      {
        std::cout << "[ERROR] Unknown exception in inner loop.\n";
        break;
      }
    }

    std::cout << "[DEBUG] reader reconnecting...\n";
    ipc::sleep_ms(sleep_mil_sec);
  }

  return 0;
}
