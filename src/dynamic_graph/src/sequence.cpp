// This is implemented as a treap. The treap is a binary tree data structure
// that has logarithmic height with respect to the number of elements in the
// tree with high probability. Each element in the sequence is a treap node, and
// the in-order traversal of the treap gives the sequence elements in order.
#include <sequence.hpp>

#include <limits>
#include <random>
#include <stdexcept>

#include <utilities/assert.hpp>

namespace sequence {

namespace {

  constexpr bool kLeft{0};
  constexpr bool kRight{1};

  std::mt19937 random_generator{0};
  std::uniform_int_distribution<int64_t> priority_distribution{
    std::numeric_limits<int64_t>::min(), std::numeric_limits<int64_t>::max()};

}  // namespace

Element::Element(const std::pair<int64_t, int64_t>& id)
  : id_{id}
  , priority_{priority_distribution(random_generator)} {}
Element::Element()
  : priority_{priority_distribution(random_generator)} {}

Element::~Element() {}

Element::Element(const Element &other)
    : id_{other.id_}
    , children_{nullptr, nullptr}
    , parent_{nullptr}
    , priority_{priority_distribution(random_generator)}
    , node_data_{other.node_data_}
    , subtree_data_{other.subtree_data_} {
  ASSERT_MSG_ALWAYS(
      other.parent_ == nullptr
        && other.children_[kLeft] == nullptr
        && other.children_[kRight] == nullptr,
      "Copied element cannot live in a sequence of multiple elements");
}

Element::Element(Element&& other) noexcept
    : id_{other.id_}
    , children_{other.children_}
    , parent_{other.parent_}
    , priority_{other.priority_}
    , node_data_{other.node_data_}
    , subtree_data_{other.subtree_data_} {
  if (parent_ != nullptr) {
    if (parent_->children_[kLeft] == &other) {
      parent_->children_[kLeft] = this;
    } else {
      parent_->children_[kRight] = this;
    }
  }
  if (children_[kLeft] != nullptr) {
    children_[kLeft]->parent_ = this;
  }
  if (children_[kRight] != nullptr) {
    children_[kRight]->parent_ = this;
  }
}

detail::SubtreeData Element::GetChildSubtreeData(bool direction) const {
  if (children_[direction] == nullptr) {
    constexpr static detail::SubtreeData empty_subtree_data{
      .size = 0,
      .has_marked = {false, false},
    };
    return empty_subtree_data;
  } else {
    return children_[direction]->subtree_data_;
  }
}

// Recomputes subtree data for this node assuming that childrens' subtree data
// is correct.
void Element::UpdateSubtreeData() {
  const detail::SubtreeData left_subtree_data{GetChildSubtreeData(kLeft)};
  const detail::SubtreeData right_subtree_data{GetChildSubtreeData(kRight)};
  subtree_data_.size = 1 + left_subtree_data.size + right_subtree_data.size;
  for (std::size_t i = 0; i < subtree_data_.has_marked.size(); i++) {
    subtree_data_.has_marked[i] =
      node_data_.marked[i]
      || left_subtree_data.has_marked[i]
      || right_subtree_data.has_marked[i];
  }
}

void Element::AssignChild(bool direction, Element* child) {
  if (child != nullptr) {
    child->parent_ = this;
  }
  children_[direction] = child;
}

Element* Element::GetRoot() const {
  const Element* current{this};
  while (current->parent_ != nullptr) {
    current = current->parent_;
  }
  return const_cast<Element*>(current);
}

Element* Element::GetRepresentative() const {
  return GetRoot();
}

Element* Element::GetPredecessor() const {
  const Element* current{this};
  if (current->children_[kLeft] == nullptr) {
    // No left child. The predecessor is the first ancestor for which the start
    // element falls in the ancestor's right subtree.
    while (true) {
      if (current->parent_ == nullptr) {
        return nullptr;
      } else if (current->parent_->children_[kRight] == current) {
        return current->parent_;
      } else {
        current = current->parent_;
      }
    }
  } else {
    // The element has a left child. The predecessor is the right-most node in
    // the left child's subtree.
    current = current->children_[kLeft];
    while (current->children_[kRight] != nullptr) {
      current = current->children_[kRight];
    }
    return const_cast<Element*>(current);
  }
}

// Joins the tree rooted at `lesser` to the tree rooted at `greater`. Returns
// root of the joined tree.
Element* Element::JoinRoots(Element* lesser, Element* greater) {
  if (lesser == nullptr) {
    return greater;
  } else if (greater == nullptr) {
    return lesser;
  }

  if (lesser->priority_ > greater->priority_) {
    lesser->AssignChild(kRight, JoinRoots(lesser->children_[kRight], greater));
    lesser->UpdateSubtreeData();
    return lesser;
  } else {
    greater->AssignChild(kLeft, JoinRoots(lesser, greater->children_[kLeft]));
    greater->UpdateSubtreeData();
    return greater;
  }
}

// Joins the tree that `lesser` lives in with the tree that `greater` lives in
// and returns the root of the resulting tree.
Element* Element::JoinWithRootReturned(Element* lesser, Element* greater) {
  Element* const lesser_root = lesser == nullptr ? nullptr : lesser->GetRoot();
  Element* const greater_root =
    greater == nullptr ? nullptr : greater->GetRoot();
  ASSERT_MSG(
      lesser_root != greater_root || lesser_root == nullptr,
      "Input nodes live in the same sequence");
  return JoinRoots(
      lesser == nullptr ? nullptr : lesser->GetRoot(),
      greater == nullptr ? nullptr : greater->GetRoot());
}

void Element::Join(Element* lesser, Element* greater) {
  JoinWithRootReturned(lesser, greater);
}

Element* Element::Split() {
  // `lesser` is the root of a sequence that will contain `this` and elements
  // preceding `this`.
  Element* lesser{nullptr};
  // `greater`is the root of a sequence that will contain all elements
  // after `this`.
  Element* greater{children_[kRight]};
  if (children_[kRight] != nullptr) {
    children_[kRight]->parent_ = nullptr;
    AssignChild(kRight, nullptr);
  }

  Element* current{this};
  bool traversed_up_from_right{true};
  bool current_is_right_child{true};
  while (current != nullptr) {
    Element* parent{current->parent_};
    if (parent != nullptr) {
      current_is_right_child = parent->children_[kRight] == current;
      parent->AssignChild(current_is_right_child, nullptr);
      current->parent_ = nullptr;
    }
    if (traversed_up_from_right) {
      lesser = JoinWithRootReturned(current, lesser);
    } else {
      greater = JoinWithRootReturned(greater, current);
    }

    traversed_up_from_right = current_is_right_child;
    current->UpdateSubtreeData();
    current = parent;
  }

  // The former successor of `this` is the leftmost descendent of `greater`.
  Element* successor = greater;
  while (successor != nullptr && successor->children_[kLeft] != nullptr) {
    successor = successor->children_[kLeft];
  }
  return successor;
}

int64_t Element::GetSize() const {
  return GetRoot()->subtree_data_.size;
}

void Element::Mark(int32_t index, bool marked) {
  node_data_.marked[index] = marked;
  Element* current{this};
  while (current != nullptr) {
    const bool old_subtree_has_marked{
      current->subtree_data_.has_marked[index]};
    const bool left_subtree_has_marked{
      current->GetChildSubtreeData(kLeft).has_marked[index]};
    const bool right_subtree_has_marked{
      current->GetChildSubtreeData(kRight).has_marked[index]};
    current->subtree_data_.has_marked[index] =
      current->node_data_.marked[index]
      || left_subtree_has_marked
      || right_subtree_has_marked;
    if (current->subtree_data_.has_marked[index] == old_subtree_has_marked) {
      break;
    } else {
      current = current->parent_;
    }
  }
}

std::optional<Element*> Element::FindMarkedElement(int32_t index) const {
  const Element* current = GetRoot();
  if (!current->subtree_data_.has_marked[index]) {
    return {};
  }
  while (true) {
    if (current->node_data_.marked[index]) {
      return const_cast<Element*>(current);
    }
    current =
      (current->children_[kLeft] != nullptr
       && current->children_[kLeft]->subtree_data_.has_marked[index])
      ? current->children_[kLeft]
      : current->children_[kRight];
  }
}

}  // namespace sequence
