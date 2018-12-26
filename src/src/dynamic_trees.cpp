#include "dynamic_trees.hpp"

using Element = sequence::Element;
using std::pair;

DynamicTrees::DynamicTrees(uint32_t num_vertices) 
    : num_vertices_(num_vertices) {
  vertices_ = std::vector<Element>(num_vertices_);
  const std::size_t num_edges = 2 * (num_vertices_ - 1);
  edge_elements_ = std::vector<Element>(num_edges);
  free_edge_elements_.reserve(num_edges);
  for (std::size_t i = 0; i < num_edges; i++) {
    free_edge_elements_.emplace_back(&edge_elements_[i]);
  }
  edges_.reserve(num_edges);
}

DynamicTrees::~DynamicTrees() {}

bool DynamicTrees::IsConnected(uint32_t u, uint32_t v) {
  return vertices_[u].GetRepresentative() == vertices_[v].GetRepresentative();
}

void DynamicTrees::Link(uint32_t u, uint32_t v) {
  // TODO add asserts
  Element* uv = free_edge_elements_.back();
  free_edge_elements_.pop_back();
  Element* vu = free_edge_elements_.back();
  free_edge_elements_.pop_back();
  edges_[std::make_pair(u, v)] = uv;
  edges_[std::make_pair(v, u)] = vu;
  const auto& [u_left, u_right] = vertices_[u].Split();
  const auto& [v_left, v_right] = vertices_[v].Split();
  Element::Join(u_left, uv);
  Element::Join(u_left, v_right);
  Element::Join(u_left, v_left);
  Element::Join(u_left, vu);
  Element::Join(u_left, u_right);
}

void DynamicTrees::Cut(uint32_t u, uint32_t v) {
  // TODO implement
  (void)u; (void)v;
//  // TODO add asserts
//  auto uv_it = edges_.find(std::make_pair(u, v));
//  auto vu_it = edges_.find(std::make_pair(v, u));
//  const auto& [uv_left, uv_right] = uv_it->second->Split();
//  const bool uv_vu_in_order = uv_left->GetRepresentative() != vu_it->second->GetRepresentative();
//  const auto& [vu_left, vu_right] = vu_it->second->Split();
//  edges_.erase(uv_it);
//  edges_.erase(vu_it);
//  uv_left = uv_left->DeleteMax();
//  vu_left = vu_left->DeleteMax();
//  if (uv_vu_in_order) {
//    Element::Join(uv_left, vu_right);
//  } else {
//    Element::Join(vu_left, uv_right);
//  }
}
