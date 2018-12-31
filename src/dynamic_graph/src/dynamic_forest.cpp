// This is implemented using a variant of Euler tour trees described in the
// following paper:
//   Robert E. Tarjan. "Dynamic trees as search trees via Euler tours, applied
//   to the network simplex algorithm." Mathematical Programming, 78(2), 1997.
//
// The idea of Euler tour trees is that given a forest, we can represent each
// tree in the forest by replacing each edge with two directed edges, taking an
// Euler tour on the tree, breaking the cyclic Euler tour at an arbitrary point,
// and storing the tour as a linear sequence. Adding and deleting edges
// corresponds to a small number of splits and concatenations on the tours.
// Tarjan's variant adds self-loop edges to each vertex and includes them in the
// Euler tour to make it convenient to look up where a vertex is in the tours.
#include <dynamic_forest.hpp>

#include <stdexcept>

#include <utilities/assert.hpp>

using Element = sequence::Element;
using UndirectedEdgeElements = detail::UndirectedEdgeElements;
using std::pair;

namespace {

constexpr int32_t kEdgeMark{0};
constexpr int32_t kVertexMark{1};

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

namespace detail {

UndirectedEdgeElements::UndirectedEdgeElements(
      sequence::Element* _forward_edge,
      sequence::Element* _backward_edge)
  : forward_edge(_forward_edge), backward_edge(_backward_edge) {}

}  // namespace detail

DynamicForest::DynamicForest(int64_t num_vertices)
    : num_vertices_(num_vertices) {
  ASSERT_MSG_ALWAYS(
      num_vertices_ > 0,
      "The number of vertices must be positive");

  vertices_.reserve(num_vertices_);
  for (int64_t i = 0; i < num_vertices_; i++) {
    vertices_.emplace_back(std::make_pair(i, i));
  }

  const int64_t max_num_edges{2 * (num_vertices_ - 1)};
  edge_elements_ = std::vector<Element>{
    static_cast<std::size_t>(max_num_edges),
    Element{std::make_pair(-1, -1)}};
  free_edge_elements_.reserve(max_num_edges);
  for (auto& edge : edge_elements_) {
    free_edge_elements_.emplace_back(&edge);
  }
  edges_.reserve(max_num_edges);
}

DynamicForest::~DynamicForest() {}

DynamicForest::DynamicForest(const DynamicForest& other)
  : DynamicForest{other.num_vertices_} {
  ASSERT_MSG_ALWAYS(other.edges_.empty(), "Copied forest must have no edges");
}

DynamicForest::DynamicForest(DynamicForest&& other) noexcept
    : num_vertices_{other.num_vertices_}
    , vertices_{std::move(other.vertices_)}
    , edge_elements_{std::move(other.edge_elements_)}
    , free_edge_elements_{std::move(other.free_edge_elements_)}
    , edges_{std::move(other.edges_)} {}

// Allocates Euler tour sequence elements for an edge in the forest.
UndirectedEdgeElements DynamicForest::AllocateEdgeElements(
    const UndirectedEdge& edge) {
  UndirectedEdgeElements edge_elements{
    free_edge_elements_[free_edge_elements_.size() - 1],
    free_edge_elements_[free_edge_elements_.size() - 2]
  };
  free_edge_elements_.pop_back();
  free_edge_elements_.pop_back();
  edge_elements.forward_edge->id_ = std::make_pair(edge.first, edge.second);
  edge_elements.backward_edge->id_ = std::make_pair(edge.second, edge.first);
  return edge_elements;
}

void DynamicForest::FreeEdgeElements(
    const UndirectedEdgeElements& edge_elements) {
  edge_elements.forward_edge->id_ = std::make_pair(-1, -1);
  edge_elements.backward_edge->id_ = std::make_pair(-1, -1);
  edge_elements.forward_edge->Mark(kEdgeMark, false);
  edge_elements.backward_edge->Mark(kEdgeMark, false);
  free_edge_elements_.emplace_back(edge_elements.forward_edge);
  free_edge_elements_.emplace_back(edge_elements.backward_edge);
}

bool DynamicForest::IsConnected(Vertex u, Vertex v) const {
  ValidateVertex(u, num_vertices_);
  ValidateVertex(v, num_vertices_);
  return vertices_[u].GetRepresentative() == vertices_[v].GetRepresentative();
}

bool DynamicForest::HasEdge(const UndirectedEdge& edge) const {
  ValidateEdge(edge, num_vertices_);
  return edges_.find(edge) != edges_.end();
}

void DynamicForest::AddEdge(const UndirectedEdge& edge) {
  ValidateEdge(edge, num_vertices_);

  UndirectedEdgeElements edge_elements{AllocateEdgeElements(edge)};
  edges_.emplace(edge, edge_elements);

  const Vertex u{edge.first};
  const Vertex v{edge.second};
  Element* const uv{edge_elements.forward_edge};
  Element* const vu{edge_elements.backward_edge};
  Element& u_element{vertices_[u]};
  Element* const u_successor{vertices_[u].Split()};
  Element& v_element{vertices_[v]};
  Element* const& v_successor{vertices_[v].Split()};
  Element::Join(&u_element, uv);
  Element::Join(&u_element, v_successor);
  Element::Join(&u_element, &v_element);
  Element::Join(&u_element, vu);
  Element::Join(&u_element, u_successor);
}

void DynamicForest::DeleteEdge(const UndirectedEdge& edge) {
  const auto& edge_it{edges_.find(edge)};
  ASSERT_MSG(
      edge_it != edges_.end(),
      "Edge " << edge << " is not in the forest.");
  UndirectedEdgeElements edge_elements{edge_it->second};
  Element* const uv{edge_elements.forward_edge};
  Element* const vu{edge_elements.backward_edge};
  edges_.erase(edge_it);

  Element* const uv_successor{uv->Split()};
  // After splitting the tour, we'll need to know whether edge (u, v) appeared
  // before (v, u) or not in the tour in order to know how to join everything
  // back together.
  const bool is_uv_before_vu_in_tour{
    uv->GetRepresentative() != vu->GetRepresentative()};
  Element* const vu_successor{vu->Split()};
  Element* const uv_predecessor{uv->GetPredecessor()};
  if (uv_predecessor != nullptr) {
    uv_predecessor->Split();
  }
  Element* const vu_predecessor{vu->GetPredecessor()};
  if (vu_predecessor != nullptr) {
    vu_predecessor->Split();
  }
  if (is_uv_before_vu_in_tour) {
    Element::Join(uv_predecessor, vu_successor);
  } else {
    Element::Join(vu_predecessor, uv_successor);
  }
  // We're freeing `uv` and `vu` here. How do we know that none of
  // `uv_predecessor`, `vu_predecessor`, `uv_successor`, and `vu_successor`
  // point to either of them?
  // Answer: Edge (u, v) can't immediately precede (v, u) in the tour because
  // the element for vertex v must fall somewhere in between. Likewise for
  // (v, u) preceding (u, v). Thus the two edge elements are not adjacent.
  FreeEdgeElements(edge_elements);
}

int64_t DynamicForest::GetSizeOfTree(Vertex v) const {
  ValidateVertex(v, num_vertices_);
  return vertices_[v].GetSize();
}


void DynamicForest::MarkEdge(const UndirectedEdge& edge, bool mark) {
  ValidateEdge(edge, num_vertices_);
  const auto& edge_it{edges_.find(edge)};
  ASSERT_MSG(
      edge_it != edges_.end(),
      "Edge " << edge << " is not in the forest.");
  edge_it->second.forward_edge->Mark(kEdgeMark, mark);
  edge_it->second.backward_edge->Mark(kEdgeMark, mark);
}

void DynamicForest::MarkVertex(Vertex v, bool mark) {
  ValidateVertex(v, num_vertices_);
  vertices_[v].Mark(kVertexMark, mark);
}

std::optional<UndirectedEdge>
DynamicForest::GetMarkedEdgeInTree(Vertex v) const {
  ValidateVertex(v, num_vertices_);
  std::optional<Element*> edge{vertices_[v].FindMarkedElement(kEdgeMark)};
  if (edge.has_value()) {
    const auto [edge_endpoint, edge_endpoint2]{(*edge)->id_};
    return UndirectedEdge{edge_endpoint, edge_endpoint2};
  } else {
    return {};
  }
}

std::optional<Vertex> DynamicForest::GetMarkedVertexInTree(Vertex v) const {
  ValidateVertex(v, num_vertices_);
  std::optional<Element*> edge{vertices_[v].FindMarkedElement(kVertexMark)};
  if (edge.has_value()) {
    return (*edge)->id_.first;
  } else {
    return {};
  }
}
