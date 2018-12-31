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

using namespace detail;

namespace {

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
        && other.children_[Direction::kLeft] == nullptr
        && other.children_[Direction::kRight] == nullptr,
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
    if (parent_->children_[Direction::kLeft] == &other) {
      parent_->children_[Direction::kLeft] = this;
    } else {
      parent_->children_[Direction::kRight] = this;
    }
  }
  if (children_[Direction::kLeft] != nullptr) {
    children_[Direction::kLeft]->parent_ = this;
  }
  if (children_[Direction::kRight] != nullptr) {
    children_[Direction::kRight]->parent_ = this;
  }
}

SubtreeData Element::GetChildSubtreeData(Direction direction) const {
  if (children_[direction] == nullptr) {
    constexpr static SubtreeData empty_subtree_data{
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
  const SubtreeData left_subtree_data{GetChildSubtreeData(Direction::kLeft)};
  const SubtreeData right_subtree_data{GetChildSubtreeData(Direction::kRight)};
  subtree_data_.size = 1 + left_subtree_data.size + right_subtree_data.size;
  for (std::size_t i = 0; i < subtree_data_.has_marked.size(); i++) {
    subtree_data_.has_marked[i] =
      node_data_.marked[i]
      || left_subtree_data.has_marked[i]
      || right_subtree_data.has_marked[i];
  }
}

void Element::AssignChild(Direction direction, Element* child) {
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
  if (current->children_[Direction::kLeft] == nullptr) {
    // No left child. The predecessor is the first ancestor for which the start
    // element falls in the ancestor's right subtree.
    while (true) {
      if (current->parent_ == nullptr) {
        return nullptr;
      } else if (current->parent_->children_[Direction::kRight] == current) {
        return current->parent_;
      } else {
        current = current->parent_;
      }
    }
  } else {
    // The element has a left child. The predecessor is the right-most node in
    // the left child's subtree.
    current = current->children_[Direction::kLeft];
    while (current->children_[Direction::kRight] != nullptr) {
      current = current->children_[Direction::kRight];
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
    lesser->AssignChild(
        Direction::kRight,
        JoinRoots(lesser->children_[Direction::kRight], greater));
    lesser->UpdateSubtreeData();
    return lesser;
  } else {
    greater->AssignChild(
        Direction::kLeft,
        JoinRoots(lesser, greater->children_[Direction::kLeft]));
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
  Element* greater{children_[Direction::kRight]};
  if (children_[Direction::kRight] != nullptr) {
    children_[Direction::kRight]->parent_ = nullptr;
    AssignChild(Direction::kRight, nullptr);
  }

  Element* current{this};
  bool traversed_up_from_left{false};
  bool current_is_left_child{false};
  while (current != nullptr) {
    Element* parent{current->parent_};
    if (parent != nullptr) {
      current_is_left_child = parent->children_[Direction::kLeft] == current;
      parent->AssignChild(current_is_left_child ? kLeft : kRight, nullptr);
      current->parent_ = nullptr;
    }
    if (traversed_up_from_left) {
      greater = JoinWithRootReturned(greater, current);
    } else {
      lesser = JoinWithRootReturned(current, lesser);
    }

    traversed_up_from_left = current_is_left_child;
    current->UpdateSubtreeData();
    current = parent;
  }

  // The former successor of `this` is the leftmost descendent of `greater`.
  Element* successor = greater;
  while (successor != nullptr
      && successor->children_[Direction::kLeft] != nullptr) {
    successor = successor->children_[Direction::kLeft];
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
      current->GetChildSubtreeData(Direction::kLeft).has_marked[index]};
    const bool right_subtree_has_marked{
      current->GetChildSubtreeData(Direction::kRight).has_marked[index]};
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
      (current->children_[Direction::kLeft] != nullptr
       && current->children_[Direction::kLeft]->subtree_data_.has_marked[index])
      ? current->children_[Direction::kLeft]
      : current->children_[Direction::kRight];
  }
}

void Element::SequenceIds(std::vector<Id>* output) const {
  if (children_[Direction::kLeft] != nullptr) {
    children_[Direction::kLeft]->SequenceIds(output);
  }
  (*output).push_back(id_);
  if (children_[Direction::kRight] != nullptr) {
    children_[Direction::kRight]->SequenceIds(output);
  }
}

std::vector<std::pair<int64_t, int64_t>> Element::SequenceIds() const {
  const Element* root = GetRoot();
  std::vector<Id> output;
  root->SequenceIds(&output);
  return output;
}

}  // namespace sequence
