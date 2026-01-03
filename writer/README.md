# Writer Module

[Korean README](README.ko.md)

This directory contains the implementation for the shared memory writer using Boost.Interprocess. The writer is responsible for creating and writing data to a shared memory segment, which can then be accessed by other processes, such as the reader module.

## File
- `writer.cpp`: The main source file that demonstrates how to create and write to shared memory using Boost.Interprocess.

## Features
- Creates a shared memory segment.
- Writes data to the shared memory.
- Demonstrates interprocess communication (IPC) using Boost libraries.

## Usage
This module is typically used in conjunction with the reader module, which reads the data written to the shared memory. To build and run the writer:

1. Ensure Boost is installed on your system.
2. Build the project using CMake:
   ```sh
   cd writer
   cmake .
   cmake --build .
   ```

3. Run the writer executable:
   ```sh
   ./writer
   ```

### Removing the Shared Memory Segment

The writer executable supports the `--remove` argument, which removes the shared memory segment from the system. This is useful for cleaning up resources or resetting the shared memory state. To remove the shared memory segment, run:

```sh
./writer --remove
```
If the segment exists, it will be deleted and a message will be printed indicating the result.

## Requirements
- C++ compiler with C++11 support or higher
- Boost.Interprocess library

## See Also
- [../reader](../reader): The reader module for reading data from shared memory.
- [../include/shared_ipc.hpp](../include/shared_ipc.hpp): Shared header for IPC logic.
