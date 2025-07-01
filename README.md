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

and for the command .../main.exe --allocator="fixedsizepooledallocator.exe 1 10000" --analyzer="analyzer.exe"

```
Average allocated block count:    98.68
Average allocated block size :  8191.18
Average free      block count:     5.26
Average free      block size : 12817.86
Average heap usage           :    92.30%
```

## How does this work

There are an allocator programs, that do random allocations and deallocations and outputs the result (address and size) to the stdout. Then the output is pipelined into an analyzer program, that takes the data and computes several properties from the data provided by the allocator. These can be any program as long they are compatible with each other. The test programs that come in this repository work like this, allocator does one random size allocation every iteration, and all allocations have certain probability (provided from the command line arguments) to be deallocated each iteration. Allocations and deallocations are done with malloc and free.

There is another allocator program, that only does fixed size allocations from a static memory pool.
The output shown in the example section shows that on average the fixed size pooled allocator has like 5 free blocks as of there are six contagious allocated memory blocks (the 98 is counting individual allocations, not contagious blocks) opposed to the 5000 from the normal allocator. The average allocated block size is also much larger, as the fixed size pooled allocator allocates blocks of 8192 bytes, while the normal allocator allocates blocks of random sizes, that on average are 20432 bytes.

The memory usage is almost the same, because the allocations and deallocations are done in same random way, and there is no aggressive way to reduce fragmentation implemented (like shifting already allocated blocks to the left).