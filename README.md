# AllocatorAnalyzer

## Table of Contents
- [Introduction](#introduction)
- [Build and Run](#build-and-run)
- [Example Output](#example-output)
- [How does this work](#how-does-this-work)

## Introduction
This program analyzes the heap usage of an allocator/deallocator pair.

## Build and Run
To clone and run this project, you'll need [Git](https://git-scm.com) and [CMake](https://cmake.org/) installed on your computer. From your command line:

```bash
# Clone this repository
$ git clone https://github.com/Hayk10002/AllocatorAnalyzer

# Go into the repository
$ cd AllocatorAnalyzer

# Generate the build files
$ cmake -DCMAKE_BUILD_TYPE=Release -S . -B build

# Build the project
$ cmake --build build --config Release

# Run the executable with the path to the target file as an argument.
$ your/path/to/exe/main.exe --allocator="{path/to/the/allocator.exe arg1 ...}" --analyzer="{path/to/the/analyzer.exe arg1 ...}"
# example - .../main.exe --allocator="allocator.exe 1 10000" --analyzer="analyzer.exe"
# there is an allocator and analyzer in the repository
```

## Example Output
for the example displayed in the previous section
```
Average allocated block count:  5001.00
Average allocated block size : 20432.63
Average free      block count:  5000.00
Average free      block size :  1627.28
Average heap usage           :    94.13%
```

## How does this work

There is an allocator program, that does random allocations and deallocations and outputs the result (address and size) to the stdout. Then the output is pipelined into an analyzer program, that takes the data and computes several properties from the data provided by the allocator. These can be any program as long they are compatible with each other. The test programs that come in this repository work like this, allocator does one random size allocation every iteration, and all allocations have certain probability (provided from the command line arguments) to be deallocated each iteration. Allocations and deallocations are done with malloc and free.