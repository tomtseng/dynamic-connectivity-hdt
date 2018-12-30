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
using std::pair;

namespace {

  constexpr int32_t kEdgeMark = 0;
  constexpr int32_t kVertexMark = 1;

}  // namespace

DynamicForest::DynamicForest(int64_t num_vertices)
    : num_vertices_(num_vertices) {
  ASSERT_MSG(
      num_vertices_ > 0,
      "The number of vertices of be positive.");

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

sequence::Element* DynamicForest::AllocateEdgeElement(int64_t u, int64_t v) {
  sequence::Element* edge{free_edge_elements_.back()};
  free_edge_elements_.pop_back();
  edge->id_ = std::make_pair(u, v);
  return edge;
}

void DynamicForest::FreeEdgeElement(sequence::Element* edge) {
  edge->id_ = std::make_pair(-1, -1);
  edge->Mark(kEdgeMark, false);
  edge->Mark(kVertexMark, false);
  free_edge_elements_.emplace_back(edge);
}

bool DynamicForest::IsConnected(int64_t u, int64_t v) const {
  ASSERT_MSG(0 <= u && u < num_vertices_, "Vertex " << u << " out of bounds.");
  ASSERT_MSG(0 <= v && v < num_vertices_, "Vertex " << v << " out of bounds.");
  return vertices_[u].GetRepresentative() == vertices_[v].GetRepresentative();
}

void DynamicForest::AddEdge(int64_t u, int64_t v) {
  ASSERT_MSG(0 <= u && u < num_vertices_, "Vertex " << u << " out of bounds.");
  ASSERT_MSG(0 <= v && v < num_vertices_, "Vertex " << v << " out of bounds.");
  ASSERT_MSG(
      !IsConnected(u, v),
      "Vertices " << u << " and " << v << "are already connected");

  Element* uv{AllocateEdgeElement(u, v)};
  Element* vu{AllocateEdgeElement(v, u)};
  edges_[std::make_pair(u, v)] = uv;
  edges_[std::make_pair(v, u)] = vu;

  auto& u_element = vertices_[u];
  const auto& u_successor = vertices_[u].Split();
  auto& v_element = vertices_[v];
  const auto& v_successor = vertices_[v].Split();
  Element::Join(&u_element, uv);
  Element::Join(&u_element, v_successor);
  Element::Join(&u_element, &v_element);
  Element::Join(&u_element, vu);
  Element::Join(&u_element, u_successor);
}

void DynamicForest::DeleteEdge(int64_t u, int64_t v) {
  const auto uv_it{edges_.find(std::make_pair(u, v))};
  ASSERT_MSG(
      uv_it != edges_.end(),
      "No edge exists between vertices " << u << " and " << v);
  const auto vu_it{edges_.find(std::make_pair(v, u))};
  const auto& uv_element = uv_it->second;
  const auto& vu_element = vu_it->second;
  edges_.erase(uv_it);
  edges_.erase(vu_it);

  const auto& uv_successor = uv_element->Split();
  // After splitting the tour, we'll need to know whether edge (u, v) appeared
  // before (v, u) or not in the tour in order to know how to join everything
  // back together.
  const bool is_uv_before_vu_in_tour{
    uv_element->GetRepresentative() != vu_element->GetRepresentative()};
  const auto& vu_successor = vu_it->second->Split();
  // There's a few edge cases to think about here.
  //
  // - Q: How do we know `uv_predecessor` and `vu_predecessor` aren't null?
  // - A: The `AddEdge` and `DeleteEdge` functions maintain that a sequence
  // always starts with an element representing a vertex.
  //
  // - Q: We're marking `uv_element` and `vu_element` as unused. How do we know
  // that none of `uv_predecessor`, `vu_predecessor`, `uv_successor`, and
  // `vu_successor` point to either of them?
  // - A: Edge (u, v) can't immediately precede (v, u) in the tour because
  // the element for vertex v must fall somewhere in between. Likewise for
  // (v, u) preceding (u, v). Thus the two edge elements are not adjacent.
  Element* uv_predecessor{uv_element->GetPredecessor()};
  uv_predecessor->Split();
  FreeEdgeElement(uv_element);
  Element* vu_predecessor{vu_element->GetPredecessor()};
  vu_predecessor->Split();
  FreeEdgeElement(vu_element);
  if (is_uv_before_vu_in_tour) {
    Element::Join(uv_predecessor, vu_successor);
  } else {
    Element::Join(vu_predecessor, uv_successor);
  }
}

int64_t DynamicForest::GetSizeOfTree(int64_t v) const {
  return vertices_[v].GetSize();
}


void DynamicForest::MarkEdge(int64_t u, int64_t v, bool mark) {
  const auto uv_it{edges_.find(std::make_pair(u, v))};
  ASSERT_MSG(
      uv_it != edges_.end(),
      "No edge exists between vertices " << u << " and " << v);
  uv_it->second->Mark(kEdgeMark, mark);
}

void DynamicForest::MarkVertex(int64_t v, bool mark) {
  ASSERT_MSG(0 <= v && v < num_vertices_, "Vertex " << v << " out of bounds.");
  vertices_[v].Mark(kVertexMark, mark);
}

std::optional<std::pair<int64_t, int64_t>>
DynamicForest::GetMarkedEdgeInTree(int64_t v) const {
  ASSERT_MSG(0 <= v && v < num_vertices_, "Vertex " << v << " out of bounds.");
  std::optional<Element*> edge{vertices_[v].FindMarkedElement(kEdgeMark)};
  if (edge.has_value()) {
    return (*edge)->id_;
  } else {
    return {};
  }
}

std::optional<int64_t> DynamicForest::GetMarkedVertexInTree(int64_t v) const {
  ASSERT_MSG(0 <= v && v < num_vertices_, "Vertex " << v << " out of bounds.");
  std::optional<Element*> edge{vertices_[v].FindMarkedElement(kVertexMark)};
  if (edge.has_value()) {
    return (*edge)->id_.first;
  } else {
    return {};
  }
}
