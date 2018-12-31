// This is implemented using the data structure described in the following
// paper:
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
//
// The comments in the file won't make much sense without reading the
// description of the data structure in the above paper.
//
// ---
//
// Some implementation details:
//
// We use `DynamicForest::void MarkEdge()` to mark level-i tree edges in
// `spanning_forests_[i]`.
// We use `DynamicForest::void MarkVertex()` to mark vertices in
// `spanning_forests_[i]` that are incident to level-i non-tree edges.
//
// TODO(tomtseng): Two practical optimizations are in Iyer et al.'s paper "An
// Experimental Study of Polylogarithmic, Fully Dynamic, Connectivity
// Algorithms".
// 1. If a tree edge is deleted, it's costly to push all the tree edges to the
// next level. So before doing that, randomly look at O(log n) incident edges
// and quit early if we find a replacement edge.
// 2. Once we get to high levels, the subgraphs and corresponding spanning
// forests are small. It's not worth it to do anything sophisticated at that
// point -- brute force search instead.
#include <dynamic_graph/dynamic_connectivity.hpp>

#include <utilities/assert.hpp>

namespace {

// Returns floor(log_2(x)) for x > 0.
int8_t FloorLog2(int64_t x) {
  int8_t a{0};
  while (x > 1) {
    x >>= 1;
    a++;
  }
  return a;
}

inline void ValidateEdge(const UndirectedEdge& edge, int64_t num_vertices) {
  ASSERT_MSG(
      0 <= edge.first && edge.first < num_vertices
        && 0 <= edge.second && edge.second < num_vertices,
      "Edge " << edge << " out of bounds");
}

inline void ValidateVertex(Vertex v, int64_t num_vertices) {
  ASSERT_MSG(0 <= v && v < num_vertices, "Vertex " << v << " out of bounds");
}

}  // namespace

using namespace detail;

DynamicConnectivity::DynamicConnectivity(int64_t num_vertices)
    : num_vertices_{num_vertices} {
  ASSERT_MSG_ALWAYS(
      num_vertices_ > 0,
      "The number of vertices must be positive");
  const int8_t num_levels = FloorLog2(num_vertices_) + 1;
  spanning_forests_ =
    std::vector<DynamicForest>{
      static_cast<std::size_t>(num_levels),
      DynamicForest(num_vertices_)};
  non_tree_adjacency_lists_ =
    std::vector<std::vector<std::unordered_set<Vertex>>>{
      static_cast<std::size_t>(num_levels),
      std::vector<std::unordered_set<Vertex>>{
        static_cast<std::size_t>(num_vertices_),
        std::unordered_set<Vertex>{}}};
}

DynamicConnectivity::~DynamicConnectivity() {}

DynamicConnectivity::DynamicConnectivity(DynamicConnectivity&& other) noexcept
    : num_vertices_{other.num_vertices_}
    , spanning_forests_{std::move(other.spanning_forests_)}
    , non_tree_adjacency_lists_{std::move(other.non_tree_adjacency_lists_)}
    , edges_{std::move(other.edges_)} {}

bool DynamicConnectivity::IsConnected(Vertex u, Vertex v) const {
  return spanning_forests_[0].IsConnected(u, v);
}

bool DynamicConnectivity::HasEdge(const UndirectedEdge& edge) const {
  return edges_.find(edge) != edges_.end();
}

int64_t DynamicConnectivity::GetSizeOfConnectedComponent(Vertex v) const {
  return spanning_forests_[0].GetSizeOfTree(v);
}

void DynamicConnectivity::AddEdgeToAdjacencyList(
    const UndirectedEdge& edge, detail::Level level) {
  {
    auto& adj_list_1{non_tree_adjacency_lists_[level][edge.first]};
    if (adj_list_1.empty()) {
      spanning_forests_[level].MarkVertex(edge.first, true);
    }
    adj_list_1.emplace(edge.second);
  }
  {
    auto& adj_list_2{non_tree_adjacency_lists_[level][edge.second]};
    if (adj_list_2.empty()) {
      spanning_forests_[level].MarkVertex(edge.second, true);
    }
    adj_list_2.emplace(edge.first);
  }
}

void DynamicConnectivity::DeleteEdgeFromAdjacencyList(
    const UndirectedEdge& edge, detail::Level level) {
  {
    auto& adj_list_1{non_tree_adjacency_lists_[level][edge.first]};
    adj_list_1.erase(adj_list_1.find(edge.second));
    if (adj_list_1.empty()) {
      spanning_forests_[level].MarkVertex(edge.first, false);
    }
  }
  {
    auto& adj_list_2{non_tree_adjacency_lists_[level][edge.second]};
    adj_list_2.erase(adj_list_2.find(edge.first));
    if (adj_list_2.empty()) {
      spanning_forests_[level].MarkVertex(edge.second, false);
    }
  }
}

// Add edge `edge` as a level-0 non-tree edge.
void DynamicConnectivity::AddNonTreeEdge(const UndirectedEdge& edge) {
  const EdgeInfo edge_info{
    .level = 0,
    .type = EdgeType::kNonTree,
  };
  edges_.emplace(edge, std::move(edge_info));
  AddEdgeToAdjacencyList(edge, 0);
}

// Add edge `edge` as a level-0 tree edge.
void DynamicConnectivity::AddTreeEdge(const UndirectedEdge& edge) {
  const EdgeInfo edge_info{
    .level = 0,
    .type = EdgeType::kTree,
  };
  edges_.emplace(edge, std::move(edge_info));
  spanning_forests_[0].AddEdge(edge);
  // We mark level-i edges in `spanning_forests_[i]`.
  spanning_forests_[0].MarkEdge(edge, true);
}

void DynamicConnectivity::AddEdge(const UndirectedEdge& edge) {
  ValidateEdge(edge, num_vertices_);
  ASSERT_MSG(edge.first != edge.second, edge << " is a self-loop edge");
  ASSERT_MSG(!HasEdge(edge), "Edge " << edge << " is already in the graph");

  if (IsConnected(edge.first, edge.second)) {
    AddNonTreeEdge(edge);
  } else {
    AddTreeEdge(edge);
  }
}

// Searches on levels `level` and lower for a non-tree edge of maximum level
// that reconnects the endpoints of `edge`. Converts that non-tree edge into a
// tree edge if any such edge is found.
void
DynamicConnectivity::ReplaceTreeEdge(const UndirectedEdge& edge, Level level) {
  auto& spanning_forest{spanning_forests_[level]};
  Vertex u{edge.first};
  Vertex v{edge.second};
  if (spanning_forest.GetSizeOfTree(u) > spanning_forest.GetSizeOfTree(v)) {
    std::swap(u, v);
  }

  // `u` lives in a relatively small tree. We promote all of its level-`level`
  // tree edges to level (`level` + 1). Otherwise, we'll fail to maintain the
  // invariant that `spanning_forests_[level + 1]` is a spanning forest over all
  // edges of level at least (`level` + 1) once we promote non-tree edges.
  const Level next_level = level + 1;
  auto& next_spanning_forest{spanning_forests_[next_level]};
  while (true) {
    const std::optional<const UndirectedEdge> tree_edge{
      spanning_forest.GetMarkedEdgeInTree(u)};
    if (!tree_edge.has_value()) {
      break;
    }

    // Promote the tree edge -- increase its level by one.
    edges_[*tree_edge].level++;
    spanning_forest.MarkEdge(*tree_edge, false);
    next_spanning_forest.AddEdge(*tree_edge);
    next_spanning_forest.MarkEdge(*tree_edge, true);
  }

  // Look at level-`level` non-tree edges incident to u's tree for a replacement
  // edge.
  while (true) {
    const std::optional<const Vertex> vertex_with_incident_edges{
      spanning_forest.GetMarkedVertexInTree(u)
    };
    if (!vertex_with_incident_edges.has_value()) {
      break;
    }

    std::unordered_set<Vertex>& adj_list{
      non_tree_adjacency_lists_[level][*vertex_with_incident_edges]
    };
    while (!adj_list.empty()) {
      const auto& adj_it{adj_list.begin()};
      const Vertex endpoint{*adj_it};
      const UndirectedEdge replacement_candidate{
        *vertex_with_incident_edges, endpoint};

      if (spanning_forest.IsConnected(u, endpoint)) {
        // Candidate is not a replacement edge. Promote it to the next
        // level.
        edges_[replacement_candidate].level++;
        DeleteEdgeFromAdjacencyList(replacement_candidate, level);
        AddEdgeToAdjacencyList(replacement_candidate, next_level);
      } else {
        // Candidate must be a replacement edge connecting `u`'s tree to
        // `v`'s tree.  It cannot connect `u`'s tree to any other tree because
        // that would mean that `spanning_forest` was not actually a spanning
        // forest over edges of level at least `level` (`{u, endpoint}` could've
        // been added to the forest).
        // Change candidate from a non-tree edge to a tree edge.
        edges_[replacement_candidate].type = EdgeType::kTree;
        DeleteEdgeFromAdjacencyList(replacement_candidate, level);
        for (Level l = level; l >= 0; l--) {
          spanning_forests_[l].AddEdge(replacement_candidate);
        }
        spanning_forest.MarkEdge(replacement_candidate, true);
        return;  // Replacement edge found.
      }
    }
  }

  // No replacement edge on level `level` found.
  if (level > 0) {
    // Search on lower level.
    ReplaceTreeEdge(edge, level - 1);
  } else {
    // There is no replacement edge. `u` and `v` are disconnected.
  }
}

void DynamicConnectivity::DeleteEdge(const UndirectedEdge& edge) {
  ValidateEdge(edge, num_vertices_);
  const auto& edge_it{edges_.find(edge)};
  ASSERT_MSG_ALWAYS(
      edge_it != edges_.end(),
      "Edge " << edge << " is not in the graph");
  const EdgeInfo edge_info{edge_it->second};
  edges_.erase(edge_it);
  switch (edge_info.type) {
    case EdgeType::kNonTree:
      DeleteEdgeFromAdjacencyList(edge, edge_info.level);
      break;
    case EdgeType::kTree:
      for (Level l{edge_info.level}; l >= 0; l--) {
        spanning_forests_[l].DeleteEdge(edge);
      }
      ReplaceTreeEdge(edge, edge_info.level);
      break;
  }
}
