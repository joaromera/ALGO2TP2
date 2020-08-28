#include "gtest/gtest.h"
#include "../src/Datum.h"

TEST(DatumTests, ConstructionSucceedsWithValidArguments)
{
  Datum(42);
  Datum(3.14);
  Datum("Hello Datum!");
}

TEST(DatumTests, IsIntegerReturnsTrueWhenDatumHoldsIntValues)
{
  EXPECT_EQ(Datum(5).isInteger(), true);
  EXPECT_EQ(Datum(100).isInteger(), true);
  EXPECT_EQ(Datum(-100).isInteger(), true);
}

TEST(DatumTests, IsIntegerReturnsFalseWhenDatumDoesNotHoldIntValues)
{
  EXPECT_EQ(Datum(3.5).isInteger(), false);
  EXPECT_EQ(Datum("").isInteger(), false);
  EXPECT_EQ(Datum("Hello").isInteger(), false);
}

TEST(DatumTests, IsStringReturnsTrueWhenDatumHoldsStrings)
{
  EXPECT_EQ(Datum("").isString(), true);
  EXPECT_EQ(Datum("Hello").isString(), true);
  EXPECT_EQ(Datum("Datum").isString(), true);
}

TEST(DatumTests, IsStringReturnsFalseWhenDatumDoesNotHoldStrings)
{
  EXPECT_EQ(Datum(0).isString(), false);
  EXPECT_EQ(Datum(42).isString(), false);
  EXPECT_EQ(Datum(3.14).isString(), false);
}

TEST(DatumTests, IntValueIsCorrectlyRetrieved)
{
  EXPECT_EQ(Datum("").value<std::string>(), "");
  EXPECT_EQ(Datum("Hello").value<std::string>(), "Hello");
  EXPECT_EQ(Datum("Datum").value<std::string>(), "Datum");
}

TEST(DatumTests, StringValueIsCorrectlyRetrieved)
{
  EXPECT_EQ(Datum("").value<std::string>(), "");
  EXPECT_EQ(Datum("Hello").value<std::string>(), "Hello");
  EXPECT_EQ(Datum("Datum").value<std::string>(), "Datum");
}

TEST(DatumTests, ObservingEqualityBetweenIntegers)
{
  EXPECT_EQ(Datum(5), Datum(5));
  EXPECT_NE(Datum(10), Datum(5));
}

TEST(DatumTests, ObservingEqualityBetweenStrings)
{
  EXPECT_EQ(Datum("hola"), Datum("hola"));
  EXPECT_NE(Datum("holas"), Datum("hola"));
}

TEST(DatumTests, ImplicitCasting)
{
  EXPECT_EQ(Datum(42), (Datum) 42);
  EXPECT_EQ(Datum("Hola"), (Datum) "Hola");
}
