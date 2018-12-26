## Dynamic Connectivity

This is an implementation of Holm et al.'s data structure for dynamic
connectivity. 

Full details of the data structure may be found in their paper titled
[_Poly-Logarithmic Deterministic Fully-Dynamic Algorithms for Connectivity,
Minimum Spanning Tree, 2-Edge, and
Biconnectivity_](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.89.919&rep=rep1&type=pdf).
Another helpful description may be found in [MIT 6.851 Advanced Data Structures
(Spring 2012) — Lecture 20: Dynamic Graphs
II](https://www.youtube.com/watch?v=L7ywsci9ujo).

### Requirements

This code uses CMake version 3.13.2 and g++ version 5.5.0.

### Building

Run the following shell commands:

```
mkdir build 
cd build
cmake ..
make
make check # run tests
```

### References

J. Holm, K. de Lichtenberg, and M. Thorup. Poly-logarithmic deterministic fully-dynamic algorithms
for connectivity, minimum spanning tree, 2-edge, and biconnectivity. _J. ACM_, 48(4), 2001.
