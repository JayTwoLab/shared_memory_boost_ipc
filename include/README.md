# shared_ipc.hpp Detailed Overview

[Korean README](README.ko.md)

This header provides the core definitions and utilities for interprocess communication (IPC) using Boost.Interprocess shared memory. It is designed to be used by both writer and reader modules to facilitate safe and efficient data exchange between processes.

## Main Components

### Constants
- `k_segment_name`: Name of the shared memory segment.
- `k_object_name`: Name of the shared object within the segment.
- `k_segment_size`: Size of the shared memory segment (default: 1 MB). Ensure this is sufficient for your data.

### `shared_payload` Structure
- Contains the actual data exchanged between processes.
- Fields:
  - `magic`: Unique identifier for the shared object (default: 0xAABBCCDD).
  - `version`: Structure versioning for compatibility.
  - `sequence`: Incrementing sequence number for updates.
  - `unix_ms`: Timestamp in milliseconds since epoch.
  - `message[256]`: User data buffer (modifiable size).
  - `mutex`: Interprocess mutex for safe concurrent access.
  - `cond`: Condition variable for signaling updates.

### Utility Functions
- `copy_cstr`: Safely copies a C-string into a fixed-size buffer.
- `now_unix_ms`: Returns the current time in milliseconds since epoch.
- `sleep_ms`: Sleeps the current thread for the specified milliseconds.

### `shutdown_flag` Class
- Handles graceful shutdown via SIGINT/SIGTERM signals.
- Use `is_set()` to check if a shutdown has been requested.

## How to Use as a Template
This header is a solid starting point for shared memory IPC, but you may want to:

- **Adjust Data Structure**: Modify `shared_payload` to fit your application's data needs (e.g., add fields, change `message` size/type).
- **Segment/Object Names**: Change `k_segment_name` and `k_object_name` to avoid conflicts if running multiple IPC applications.
- **Segment Size**: Increase `k_segment_size` if your data structure grows.
- **Synchronization**: If you need more complex synchronization, consider adding more mutexes/condition variables or using other Boost IPC primitives.
- **Error Handling**: Add more robust error handling for production use (e.g., check for allocation failures, handle exceptions).
- **Cross-Platform Signals**: The `shutdown_flag` uses POSIX signals; adapt for Windows if needed (e.g., use `SetConsoleCtrlHandler`).

## Related Files
- Used by both the writer and reader modules in this project.
- See also: [../writer/writer.cpp](../writer/writer.cpp), [../reader/reader.cpp](../reader/reader.cpp)

## Requirements
- C++11 or higher
- Boost.Interprocess library

---
This file is intended to be a reusable template for shared memory IPC projects. Adapt the structure and utilities as needed for your specific use case.
