#pragma once

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>

#include <atomic>
#include <cstdint>
#include <csignal>
#include <cstring>
#include <string>
#include <thread>
#include <chrono>

namespace ipc
{
namespace bip = boost::interprocess; // short alias

inline constexpr const char* k_segment_name = "demo_shared_segment"; // name of the shared memory segment
inline constexpr const char* k_object_name  = "demo_shared_object"; // name of the shared object within the segment. sub object of shared memory segment. 
inline constexpr std::size_t k_segment_size = 1 * 1024 * 1024; // 1 MB. you must ensure this is enough for your data. check sizeof(shared_payload).

// Structure stored in shared memory.
// This must be kept in sync between writer and reader.
// You may modify this structure as needed, but ensure compatibility.
struct shared_payload
{
  std::uint32_t magic = 0xAABBCCDD; // unique identifier for the shared object
  std::uint32_t version = 1; // version of the shared object structure

  std::uint64_t sequence = 0; // incrementing sequence number
  std::uint64_t unix_ms = 0; // timestamp in milliseconds since epoch

  char message[256]{}; // user data. you can adjust size as needed.

  bip::interprocess_mutex mutex; // mutex for synchronizing access
  bip::interprocess_condition cond; // condition variable for signaling updates
};

// Safe string copy into fixed-size char array
inline void copy_cstr(char* dst, std::size_t dst_size, const char* src)
{
  if (!dst || dst_size == 0) return;
  if (!src) { dst[0] = '\0'; return; }
  std::strncpy(dst, src, dst_size - 1);
  dst[dst_size - 1] = '\0';
}

// Shutdown flag to handle termination signals
class shutdown_flag
{
public:
  shutdown_flag()
  {
    instance_ptr() = this;
    std::signal(SIGINT, &shutdown_flag::signal_handler);
    std::signal(SIGTERM, &shutdown_flag::signal_handler);
  }

  ~shutdown_flag()
  {
    instance_ptr() = nullptr;
  }
  
  bool is_set() const
  {
    return stop_.load(std::memory_order_relaxed);
  }

private:
  static shutdown_flag*& instance_ptr()
  {
    static shutdown_flag* ptr = nullptr;
    return ptr;
  }

  static void signal_handler(int)
  {
    if (instance_ptr())
      instance_ptr()->stop_.store(true, std::memory_order_relaxed);
  }

  std::atomic<bool> stop_{false};
};

// Get current time in milliseconds since epoch
inline std::uint64_t now_unix_ms()
{
  using namespace std::chrono;
  return static_cast<std::uint64_t>(
    duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()
  );
}

// Sleep for specified milliseconds
inline void sleep_ms(int ms)
{
  std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

} // namespace ipc
