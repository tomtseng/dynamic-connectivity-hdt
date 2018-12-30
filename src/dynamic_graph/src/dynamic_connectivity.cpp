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

#include <utilities/assert.hpp>

namespace {

  // Returns floor(log_2(x)) for x > 0.
  int8_t FloorLog2(int64_t x) {
    int32_t a = 0;
    while (x > 1) {
      x >>= 1;
      a++;
    }
    return a;
  }
}  // namespace

DynamicConnectivity::DynamicConnectivity(int64_t num_vertices)
    : num_vertices_{num_vertices} {
  const int8_t num_levels = 1 + FloorLog2(num_vertices_);
  // TODO
}

bool DynamicConnectivity::IsConnected(Vertex u, Vertex v) const {
  return spanning_forests_[0].IsConnected(u, v);
}

int64_t DynamicConnectivity::GetSizeOfConnectedComponent(Vertex v) const {
  return spanning_forests_[0].GetSizeOfTree(v);
}

void DynamicConnectivity::AddEdge(const UndirectedEdge& edge) {
  if (edge.first == edge.second) {
    return;
  }
  // TODO
}

void DynamicConnectivity::DeleteEdge(const UndirectedEdge& edge) {
  // TODO
}

DynamicConnectivity::~DynamicConnectivity() {}
