// This is implemented using the data structure described in section 2 of the
// following paper:
//   Jacob Holm, Kristian de Lichtenberg, and Mikkel Thorup. "Poly-logarithmic
//   deterministic fully-dynamic algorithms for connectivity, minimum spanning
//   tree, 2-edge, and biconnectivity." Journal of the ACM, 48(4):723–760, 2001.
//
// The data structure takes several subgraphs of the represented graph, each
// subgraph having fewer and fewer edges. It maintains a spanning forest on each
// subgraph. Edge insertions and connectivity queries are handled by looking at
// the spanning forest on the whole graph. For edge deletions, the difficult
// part comes if we delete an edge in the spanning forest for the whole graph.
// Then we have to determine whether there is a replacement edge that can
// reconnect the spanning forest. We look at the edges in the subgraphs,
// amortizing the cost of looking at an edge in a subgraph by moving it into
// another subgraph and making sure that no edge can be moved too many times.
#include <dynamic_graph/dynamic_connectivity.hpp>

DynamicConnectivity::DynamicConnectivity(int64_t num_vertices)
    : num_vertices_{num_vertices} {}

DynamicConnectivity::~DynamicConnectivity() {}
