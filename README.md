# Dynamic Connectivity [![Build Status](https://travis-ci.com/tomtseng/dynamic-connectivity-hdt.svg?token=4YxzvXpUhyrEkNzZRD8y&branch=master)](https://travis-ci.com/tomtseng/dynamic-connectivity-hdt)

This is an implementation of Holm et al.'s data structure for dynamic
connectivity.

## About dynamic connectivity

### Problem

We have an undirected graph undergoing edge insertions and edge deletions. We
want to be able to answer queries of the form "are vertices x and y connected?"

### Data structure

The relevant header file is `dynamic_connectivity.hpp`.

The data structure implemented in this repository is described in section 3 of
the paper [_Poly-Logarithmic Deterministic Fully-Dynamic Algorithms for
Connectivity, Minimum Spanning Tree, 2-Edge, and
Biconnectivity_](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.89.919&rep=rep1&type=pdf).
Another helpful description is in the lecture [MIT 6.851 Advanced Data
Structures (Spring 2012) — Lecture 20: Dynamic Graphs
II](https://youtu.be/L7ywsci9ujo?t=3140). The data structure achieves O(log^2 n)
amortized time edge insertions and deletions and O(log n) time connectivity
queries. (This repository does not implement the extra trick of using a B-tree
to reduce the time complexity for connectivity queries from O(log n) to O(log n /
log log n).)

## Building

### Requirements

This code uses CMake version 3.13.2 and g++ version 7.4.0 with Boost version
1.68 on Linux.

### Commands

Run the following shell commands:

```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug .. # or `cmake -DCMAKE_BUILD_TYPE=Release ..` to remove debug assertions and info
make
make check # run tests
make docs # optional: make documentation files in docs/html/
./src/dynamic_graph/benchmark/benchmark_dynamic_connectivity # run a benchmark
```

## References

Jacob Holm, Kristian de Lichtenberg, and Mikkel Thorup. Poly-logarithmic
deterministic fully-dynamic algorithms for connectivity, minimum spanning tree,
2-edge, and biconnectivity. _Journal of the ACM_, 48(4):723–760, 2001.
