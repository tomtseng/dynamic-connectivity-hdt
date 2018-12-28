#include <sequence.hpp>

#include <gtest/gtest.h>

namespace seq = sequence;

TEST(Sequence, CopyConstructorMultipleElements) {
  seq::Element elements[2];
  seq::Element::Join(&elements[0], &elements[1]);
  EXPECT_DEATH(
      seq::Element newElement{elements[0]},
      "Copied element cannot live in a sequence of multiple elements");
}

TEST(Sequence, CopyConstructorSingleElement) {
  seq::Element element;
  seq::Element newElement{element};  // expect no death
}

TEST(Sequence, JoinAndSplit) {
  static constexpr std::size_t kNumElements = 4;
  seq::Element elements[kNumElements];
  for (std::size_t i = 1; i < kNumElements; i++) {
    EXPECT_NE(elements[0].GetRepresentative(), elements[i].GetRepresentative());
    seq::Element::Join(&elements[0], &elements[i]);
  }
  for (std::size_t i = 1; i < kNumElements; i++) {
    EXPECT_EQ(elements[0].GetRepresentative(), elements[i].GetRepresentative());
  }

  static constexpr std::size_t kSplitIndex = (kNumElements - 1) / 2;
  seq::Element* split_successor = elements[kSplitIndex].Split();
  EXPECT_EQ(split_successor, &elements[kSplitIndex + 1]);
  EXPECT_NE(
      elements[0].GetRepresentative(),
      elements[kNumElements - 1].GetRepresentative());
  for (std::size_t i = 0; i <= kSplitIndex; i++) {
    EXPECT_EQ(
        elements[i].GetRepresentative(),
        elements[kSplitIndex].GetRepresentative());
  }
  for (std::size_t i = kSplitIndex + 1; i < kNumElements; i++) {
    EXPECT_EQ(
        elements[i].GetRepresentative(),
        split_successor->GetRepresentative());
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
  static constexpr std::size_t kNumElements = 10;
  seq::Element elements[kNumElements];
  for (std::size_t i = 1; i < kNumElements; i++) {
    seq::Element::Join(&elements[0], &elements[i]);
  }
  EXPECT_EQ(elements[0].GetPredecessor(), nullptr);
  for (std::size_t i = 1; i < kNumElements; i++) {
    EXPECT_EQ(elements[i].GetPredecessor(), &elements[i - 1]);
  }
}

TEST(Sequence, MoveConstructor) {
  seq::Element elements[3];
  seq::Element::Join(&elements[0], &elements[1]);
  seq::Element::Join(&elements[1], &elements[2]);

  seq::Element movedElement1 = std::move(elements[1]);
  EXPECT_EQ(movedElement1.GetRepresentative(), elements[0].GetRepresentative());
  EXPECT_EQ(movedElement1.GetRepresentative(), elements[2].GetRepresentative());

  seq::Element movedElement0 = std::move(elements[0]);
  seq::Element movedElement2 = std::move(elements[2]);
  EXPECT_EQ(
      movedElement1.GetRepresentative(),
      movedElement0.GetRepresentative());
  EXPECT_EQ(
      movedElement1.GetRepresentative(),
      movedElement2.GetRepresentative());
}