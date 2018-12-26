#include "sequence.hpp"

#include <gtest/gtest.h>

namespace seq = sequence;

TEST(Sequence, JoinAndSplit) {
  static constexpr int kNumElements = 4;
  seq::Element elements[kNumElements];
  for (int i = 1; i < kNumElements; i++) {
    EXPECT_NE(elements[0].GetRepresentative(), elements[i].GetRepresentative());
    seq::Element::Join(&elements[0], &elements[i]);
  }
  for (int i = 1; i < kNumElements; i++) {
    EXPECT_EQ(elements[0].GetRepresentative(), elements[i].GetRepresentative());
  }

  static constexpr int kSplitIndex = (kNumElements - 1) / 2;
  std::pair<seq::Element*, seq::Element*> split_result =
    elements[kSplitIndex].Split();
  EXPECT_NE(
      elements[0].GetRepresentative(),
      elements[kNumElements - 1].GetRepresentative());
  for (int i = 0; i <= kSplitIndex; i++) {
    EXPECT_EQ(
        elements[i].GetRepresentative(),
        split_result.first->GetRepresentative());
  }
  for (int i = kSplitIndex + 1; i < kNumElements; i++) {
    EXPECT_EQ(
        elements[i].GetRepresentative(),
        split_result.second->GetRepresentative());
  }
}

TEST(Sequence, JoinAndSplitEmptySequences) {
  // Check that joining with empty sequences and splitting at the end of a
  // sequence doesn't cause errors.

  seq::Element::Join(nullptr, nullptr);

  seq::Element elements[2];
  seq::Element::Join(&elements[0], &elements[1]);
  seq::Element::Join(&elements[1], nullptr);
  seq::Element::Join(nullptr, &elements[1]);
  elements[1].Split();
  EXPECT_EQ(elements[0].GetRepresentative(), elements[1].GetRepresentative());
}

TEST(Sequence, GetPredecessor) {
  static constexpr int kNumElements = 10;
  seq::Element elements[kNumElements];
  for (int i = 1; i < kNumElements; i++) {
    seq::Element::Join(&elements[0], &elements[i]);
  }
  EXPECT_EQ(elements[0].GetPredecessor(), nullptr);
  for (int i = 1; i < kNumElements; i++) {
    EXPECT_EQ(elements[i].GetPredecessor(), &elements[i - 1]);
  }
}
