#include <sequence.hpp>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace seq = sequence;

using ::testing::Optional;

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

TEST(Sequence, GetPredecessor) {
  seq::Element elements[10];
  for (int32_t i = 1; i < 10; i++) {
    seq::Element::Join(&elements[0], &elements[i]);
  }
  EXPECT_EQ(elements[0].GetPredecessor(), nullptr);
  for (int32_t i = 1; i < 10; i++) {
    EXPECT_EQ(elements[i].GetPredecessor(), &elements[i - 1]);
  }
}

TEST(Sequence, JoinAndSplitAndGetSize) {
  seq::Element elements[4];
  EXPECT_EQ(elements[0].GetSize(), 1);
  for (int32_t i = 1; i < 4; i++) {
    EXPECT_NE(elements[0].GetRepresentative(), elements[i].GetRepresentative());
    seq::Element::Join(&elements[0], &elements[i]);
    EXPECT_EQ(elements[0].GetSize(), i + 1);
  }
  for (int32_t i = 1; i < 4; i++) {
    EXPECT_EQ(elements[0].GetRepresentative(), elements[i].GetRepresentative());
  }

  seq::Element* split_successor{elements[1].Split()};
  EXPECT_EQ(split_successor, &elements[2]);
  EXPECT_NE(
      elements[0].GetRepresentative(),
      elements[3].GetRepresentative());
  EXPECT_EQ(elements[1].GetRepresentative(), elements[0].GetRepresentative());
  EXPECT_EQ(
      split_successor->GetRepresentative(),
      elements[2].GetRepresentative());
  EXPECT_EQ(
      split_successor->GetRepresentative(),
      elements[3].GetRepresentative());
  EXPECT_EQ(elements[0].GetSize(), 2);
  EXPECT_EQ(elements[3].GetSize(), 2);
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
  EXPECT_EQ(elements[0].GetSize(), 2);
}

TEST(Sequence, Mark) {
  seq::Element elements[2];
  EXPECT_FALSE(elements[0].FindMarkedElement(0).has_value());

  elements[0].Mark(0, true);
  EXPECT_THAT(elements[0].FindMarkedElement(0), Optional(&elements[0]));
  EXPECT_FALSE(elements[1].FindMarkedElement(0).has_value());

  seq::Element::Join(&elements[0], &elements[1]);
  EXPECT_THAT(elements[1].FindMarkedElement(0), Optional(&elements[0]));

  elements[1].Mark(1, true);
  EXPECT_THAT(elements[0].FindMarkedElement(1), Optional(&elements[1]));
  EXPECT_THAT(elements[1].FindMarkedElement(1), Optional(&elements[1]));

  elements[0].Mark(0, false);
  EXPECT_FALSE(elements[0].FindMarkedElement(0).has_value());
  EXPECT_FALSE(elements[1].FindMarkedElement(0).has_value());

  elements[0].Split();
  EXPECT_FALSE(elements[0].FindMarkedElement(1).has_value());
  EXPECT_THAT(elements[1].FindMarkedElement(1), Optional(&elements[1]));
}

TEST(Sequence, MoveConstructor) {
  seq::Element elements[3];
  seq::Element::Join(&elements[0], &elements[1]);
  seq::Element::Join(&elements[1], &elements[2]);

  seq::Element movedElement1{std::move(elements[1])};
  EXPECT_EQ(movedElement1.GetRepresentative(), elements[0].GetRepresentative());
  EXPECT_EQ(movedElement1.GetRepresentative(), elements[2].GetRepresentative());

  seq::Element movedElement0{std::move(elements[0])};
  seq::Element movedElement2{std::move(elements[2])};
  EXPECT_EQ(
      movedElement1.GetRepresentative(),
      movedElement0.GetRepresentative());
  EXPECT_EQ(
      movedElement1.GetRepresentative(),
      movedElement2.GetRepresentative());
}
