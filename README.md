# skiplist
# High-Performance SkipList Implementation in C++

A modern, optimized C++ implementation of the SkipList probabilistic data structure with performance benchmarking against `std::map`.

## Overview

This project implements a SkipList - a probabilistic alternative to balanced trees that provides O(log n) search, insertion, and deletion operations. The implementation focuses on practical performance optimizations including fast random number generation, memory-efficient design, and cache-friendly traversal patterns.

**Note**: SkipList provides *expected* O(log n) performance with high probability.

## Features

- **Generic Template Design**: Supports any key-value types with comparison operators
- **Optimized Random Number Generation**: Uses PCG32/Xorshift instead of std::mt19937 for 10-15x faster level generation
- **Cache-Friendly**: Forward iteration and prefetching for improved cache locality
- **Automatic Level Calculation**: Computes optimal max levels based on expected dataset size

## Compilation
```
clang++ -std=c++23 -O3 -march=native -funroll-loops -fvectorize -DNDEBUG -Wall -Wextra main.cc -o skiplist_bench
```

## Project Structure
```
.
├── main.cc              # Benchmark harness and test driver
├── skiplist.hh          # Base SkipList implementation
├── skiplist_opt.hh      # Optimized SkipList with performance enhancements
├── node.hh
└── README.md            # This file
```


![alt text](https://github.com/fkastrati/skiplist/blob/main/skiplist_viz.png)
