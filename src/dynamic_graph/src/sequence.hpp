// This is a data structure for storing lists of elements.
//
// It is augmented to be used for representing tours in Euler tour trees,
// which are in turn specialized for use in Holm et al.'s dynamic connectivity
// algorithm. This use-case-specific augmentation could be abstracted out, but
// it's not worth the effort for this small project.
#pragma once

#include <cstdint>
#include <array>
#include <optional>
#include <utility>
#include <vector>

namespace sequence {

namespace detail {

enum Direction {
  kLeft = 0,
  kRight
};

// Augmented info for each node about the node's subtree.
// Specialized for use in Holm et al.'s dynamic connectivity algorithm.
struct SubtreeData {
  // Size of this node's subtree.
  int64_t size{1};

  // `has_marked[i]` stores whether any node in this subtree has `marked[i]`
  // true.
  std::array<bool, 2> has_marked{{false, false}};
};
// Specialized for use in Holm et al.'s dynamic connectivity algorithm.
struct NodeData {
  std::array<bool, 2> marked{{false, false}};
};

}  // namespace detail

typedef std::pair<int64_t, int64_t> Id;

// Usage: create single-element sequences with the `Element()` constructor, and
// build bigger sequences from there.
class Element {
 public:
  // Initializes a single sequence element.
  explicit Element(const std::pair<int64_t, int64_t>& id);
  Element();

  ~Element();

  // Copies elements that are in single-element sequences. Copying will throw an
  // exception if attempted on an element that lives in a sequence of several
  // elements.
  Element(const Element &other);
  Element& operator=(const Element& other) = delete;  // unimplemented
  // Moves element. Other elements that point to the moved element will be
  // changed to point to the new element.
  Element(Element&& other) noexcept;
  Element& operator=(Element&& other) noexcept = delete;  // unimplemented

  // Returns a representative of the sequence that the element lives in.
  //
  // Two elements are in the same sequence if and only if their representatives
  // are the same. Representatives are invalidated after the sequence is
  // modified.
  //
  // Efficiency: logarithmic in the size of the element's sequence.
  Element* GetRepresentative() const;

  // Get element immediately preceding this element in the sequence. Returns
  // null if this element is the first element in the sequence.
  //
  // Efficiency: logarithmic in the size of the element's sequence.
  Element* GetPredecessor() const;

  // Concatenates the sequence containing `lesser` and the sequence containing
  // `greater`.
  //
  // `lesser` and `greater` must not live in the same sequence.
  //
  // Efficiency: logarithmic in the sum of the sizes of `lesser` and `greater`'s
  // sequences.
  static void Join(Element* lesser, Element* greater);

  // Splits the sequence that this element lives in immediately after the
  // element.
  //
  // After splitting, this element's sequence contains itself and all elements
  // that were before this element, and the returned element's sequence contains
  // all elements that were after the calling element.
  //
  // Returns what was formerly the successor of this element.
  //
  // Efficiency: logarithmic in the size of the element's sequence.
  Element* Split();

  // Returns size of the sequence that the element lives in.
  //
  // Efficiency: logarithmic in the size of the element's sequence.
  int64_t GetSize() const;

  // Mark (if `mark` is true) or unmark (if `mark` is false) the element at
  // index `index`. See `FindMarkedElement`.
  //
  // Efficiency: logarithmic in the size of the element's sequence.
  void Mark(int32_t index, bool mark);
  // Return an element in the calling element's sequence that is marked at its
  // `index`-th index if such an element exists.
  //
  // Efficiency: logarithmic in the size of the element's sequence.
  std::optional<Element*> FindMarkedElement(int32_t index) const;

  // Returns the ids of the elements of the sequence in which this
  // element lives.
  std::vector<Id> SequenceIds() const;

  // Identifier for the element.
  //
  // This is specialized for storing Euler tour elements. The identifier can
  // store what edge this element represents.
  Id id_{-1, -1};

 private:
  void AssignChild(detail::Direction direction, Element* child);
  detail::SubtreeData GetChildSubtreeData(detail::Direction direction) const;
  Element* GetRoot() const;
  static Element* JoinRoots(Element* lesser, Element* greater);
  static Element* JoinWithRootReturned(Element* lesser, Element* greater);
  void SequenceIds(std::vector<Id>* output) const;
  void UpdateSubtreeData();

  std::array<Element*, 2> children_{nullptr, nullptr};
  Element* parent_{nullptr};
  // Treap invariant: the priority of a node must be at least as great as the
  // priority of each of its children.
  int64_t priority_;
  detail::NodeData node_data_{};
  detail::SubtreeData subtree_data_{};
};

}  // namespace sequence
