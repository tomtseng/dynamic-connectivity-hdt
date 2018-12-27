#include "dynamic_forest.hpp"

#include <stdexcept>

#include "assert.hpp"

using Element = sequence::Element;
using std::pair;

DynamicForest::DynamicForest(uint32_t num_vertices)
    : num_vertices_(num_vertices) {
  vertices_ = std::vector<Element>(num_vertices_);
  const std::size_t num_edges{2 * (num_vertices_ - 1)};
  edge_elements_ = std::vector<Element>(num_edges);
  free_edge_elements_.reserve(num_edges);
  for (std::size_t i = 0; i < num_edges; i++) {
    free_edge_elements_.emplace_back(&edge_elements_[i]);
  }
  edges_.reserve(num_edges);
}

DynamicForest::~DynamicForest() {}

bool DynamicForest::IsConnected(uint32_t u, uint32_t v) {
  return vertices_[u].GetRepresentative() == vertices_[v].GetRepresentative();
}

void DynamicForest::Link(uint32_t u, uint32_t v) {
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

void DynamicForest::Cut(uint32_t u, uint32_t v) {
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
  // TODO explain
  const bool uv_vu_in_order{
    uv_element->GetRepresentative() != vu_element->GetRepresentative()};
  const auto& vu_successor = vu_it->second->Split();
  // TODO comment here --- why is this OK? why do we know there are no nullptrs
  // after splitting, etc?
  Element* uv_predecessor{uv_element->GetPredecessor()};
  uv_predecessor->Split();
  free_edge_elements_.emplace_back(uv_element);
  Element* vu_predecessor{vu_element->GetPredecessor()};
  vu_predecessor->Split();
  free_edge_elements_.emplace_back(vu_element);
  if (uv_vu_in_order) {
    Element::Join(uv_predecessor, vu_successor);
  } else {
    Element::Join(vu_predecessor, uv_successor);
  }
}
