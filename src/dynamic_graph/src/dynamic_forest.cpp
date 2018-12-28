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
#include <dynamic_graph/dynamic_forest.hpp>

#include <stdexcept>

#include <utilities/assert.hpp>

using Element = sequence::Element;
using std::pair;

DynamicForest::DynamicForest(uint64_t num_vertices)
    : num_vertices_(num_vertices) {
  vertices_ = std::vector<Element>(num_vertices_);
  const std::size_t max_num_edges{2 * (num_vertices_ - 1)};
  edge_elements_ = std::vector<Element>(max_num_edges);
  free_edge_elements_.reserve(max_num_edges);
  for (std::size_t i = 0; i < max_num_edges; i++) {
    free_edge_elements_.emplace_back(&edge_elements_[i]);
  }
  edges_.reserve(max_num_edges);
}

DynamicForest::~DynamicForest() {}

bool DynamicForest::IsConnected(uint64_t u, uint64_t v) {
  return vertices_[u].GetRepresentative() == vertices_[v].GetRepresentative();
}

void DynamicForest::AddEdge(uint64_t u, uint64_t v) {
  ASSERT_MSG(
      !IsConnected(u, v),
      "Vertices " << u << " and " << v << "are already connected");
  Element* uv{free_edge_elements_.back()};
  free_edge_elements_.pop_back();
  Element* vu{free_edge_elements_.back()};
  free_edge_elements_.pop_back();
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

void DynamicForest::DeleteEdge(uint64_t u, uint64_t v) {
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
  free_edge_elements_.emplace_back(uv_element);
  Element* vu_predecessor{vu_element->GetPredecessor()};
  vu_predecessor->Split();
  free_edge_elements_.emplace_back(vu_element);
  if (is_uv_before_vu_in_tour) {
    Element::Join(uv_predecessor, vu_successor);
  } else {
    Element::Join(vu_predecessor, uv_successor);
  }
}
