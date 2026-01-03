# Reader Module

[Korean README](README.ko.md)

This directory contains the implementation for the shared memory reader using Boost.Interprocess. The reader is responsible for connecting to an existing shared memory segment (created by the writer) and reading data from it in a synchronized and safe manner.

## File
- `reader.cpp`: The main source file that demonstrates how to connect to and read from shared memory using Boost.Interprocess.

## Features
- Connects to an existing shared memory segment.
- Reads and displays data written by the writer process.
- Uses interprocess mutex and condition variable for safe, synchronized access.
- Handles graceful shutdown via signal handling.

## Usage
This module is typically used in conjunction with the writer module, which creates and writes data to the shared memory. To build and run the reader:

1. Ensure Boost is installed on your system.
2. Build the project using CMake:
   ```sh
   cd reader
   cmake .
   cmake --build .
   ```

3. Run the reader executable:
   ```sh
   ./reader
   ```

### Reading Once and Exiting

The reader executable supports the `--once` argument, which makes it read the shared memory segment a single time and then exit. This is useful for scripting or when you only need to fetch the latest message without continuous monitoring. To use this mode, run:

```sh
./reader --once
```
The program will connect to the shared memory, read and display the current message, and then terminate.

By default (without `--once`), the reader will continuously monitor and display new messages as they arrive.

## Requirements
- C++ compiler with C++11 support or higher
- Boost.Interprocess library

## See Also
- [../writer](../writer): The writer module for creating and writing data to shared memory.
- [../include/shared_ipc.hpp](../include/shared_ipc.hpp): Shared header for IPC logic and data structure.
