# Shared Memory Boost IPC

[Korean README](README.ko.md)

This project demonstrates inter-process communication (IPC) using shared memory with [Boost.Interprocess](https://www.boost.org/doc/libs/release/doc/html/interprocess.html). It provides simple reader and writer examples to show how multiple processes can exchange data efficiently via shared memory.

## Features
- Shared memory communication using Boost.Interprocess
- Example writer and reader applications
- CMake-based build system
- Header-only interface for easy integration

## Project Structure
```
shared_memory_boost_ipc/
├── include/
│   └── shared_ipc.hpp      # Shared memory IPC interface
├── reader/
│   ├── reader.cpp          # Reader example
│   └── CMakeLists.txt      # Reader build config
├── writer/
│   ├── writer.cpp          # Writer example
│   └── CMakeLists.txt      # Writer build config
├── CMakeLists.txt          # Top-level build config
├── LICENSE                 # License information
├── README.md               # Project documentation
```

## Prerequisites
- C++17 or later
- [Boost](https://www.boost.org/) library (Interprocess module)
- CMake 3.10+

## Build Instructions
1. Install Boost (ensure `boost_system` and `boost_interprocess` are available).
2. Clone this repository:
	```sh
	git clone https://github.com/JayTwoLab/shared_memory_boost_ipc.git
	cd shared_memory_boost_ipc
	```
3. Create a build directory and run CMake:
	```sh
	mkdir build
	cd build
	cmake ..
	cmake --build .
	```
    - For fast search boost 
    ```sh
    cmake -S . -B build -DBOOST_ROOT=/usr -DBOOST_INCLUDEDIR=/usr/include -DBOOST_LIBRARYDIR=/usr/lib64   -DBoost_NO_SYSTEM_PATHS=ON -DBoost_DEBUG=ON
    ```
4. The executables for the reader and writer will be built in their respective folders.

## Usage
1. Start the writer process to create and write data to shared memory:
	```sh
	./writer/writer
	```
2. Start the reader process to read data from shared memory:
	```sh
	./reader/reader
	```

## License
This project is licensed under the terms of the LICENSE file in this repository.

## References
- [Boost.Interprocess Documentation](https://www.boost.org/doc/libs/release/doc/html/interprocess.html)
