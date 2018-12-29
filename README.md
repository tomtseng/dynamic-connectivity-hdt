# Dynamic Connectivity [![Build Status](https://travis-ci.com/tomtseng/dynamic-connectivity-hdt.svg?token=4YxzvXpUhyrEkNzZRD8y&branch=master)](https://travis-ci.com/tomtseng/dynamic-connectivity-hdt)

This is an implementation of Holm et al.'s data structure for dynamic
connectivity.

## Data structure

The data structure is described in the paper
[_Poly-Logarithmic Deterministic Fully-Dynamic Algorithms for Connectivity,
Minimum Spanning Tree, 2-Edge, and
Biconnectivity_](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.89.919&rep=rep1&type=pdf).
Another helpful description is in the lecture [MIT 6.851 Advanced Data Structures
(Spring 2012) — Lecture 20: Dynamic Graphs
II](https://www.youtube.com/watch?v=L7ywsci9ujo).

We implement this data structure in this repository, minus the extra trick to
reduce the complexity for connectivity queries from O(log n) to O(n / log log
n). The relevant header file is `dynamic_connectivity.hpp`.

## Requirements

This code uses CMake version 3.13.2 and g++ version 7.4.0 on Linux.

## Building

Run the following shell commands:

```bash
mkdir build
cd build
cmake ..
make
make check # run tests
```

## References

J. Holm, K. de Lichtenberg, and M. Thorup. Poly-logarithmic deterministic
fully-dynamic algorithms for connectivity, minimum spanning tree, 2-edge, and
biconnectivity. _J. ACM_, 48(4), 2001.
