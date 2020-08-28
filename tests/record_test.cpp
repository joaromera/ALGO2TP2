#include "gtest/gtest.h"
#include "../src/Datum.h"
#include "../src/Record.h"

TEST(RecordTests, Construction)
{
  Record({ "LU", "LU_A", "Nombre", "Carrera" }, { Datum(182), Datum(18), Datum("March"), Datum("Computacion") });
  Record({}, {});
}

TEST(RecordTests, ColumnsAreCorrectlySet)
{
  Record r({ "A", "B", "C" }, { Datum(1), Datum(2), Datum(3) });
  linear_set<std::string>::size_type expectedSize = 3;
  EXPECT_EQ(expectedSize, r.columns().size());
  EXPECT_EQ(r.columns(), linear_set<std::string>({ "A", "B", "C" }));

  Record r2({}, {});
  expectedSize = 0;
  EXPECT_EQ(expectedSize, r2.columns().size());
  EXPECT_EQ(r2.columns(), linear_set<std::string>());
}

TEST(RecordTests, DataTypeAndValueAreCorrect)
{
  Record r({ "LU", "LU_A", "Nombre", "Carrera" }, { Datum(182), Datum(18), Datum("March"), Datum("Computacion") });

  EXPECT_EQ(r.value("LU").isInteger(), true);
  EXPECT_EQ(r.value("LU").isString(), false);
  EXPECT_EQ(r.value("LU").value<int>(), 182);

  EXPECT_EQ(r.value("Nombre").isInteger(), false);
  EXPECT_EQ(r.value("Nombre").isString(), true);
  EXPECT_EQ(r.value("Nombre").value<std::string>(), "March");
}

TEST(RecordTests, TwoRecordsWithSameColumnsAndValuesAreEqual)
{
  Record r1({ "LU", "LU_A" }, { Datum(1), Datum("a") });
  Record r2({ "LU", "LU_A" }, { Datum(1), Datum("a") });
  EXPECT_TRUE(r1 == r2);
}

TEST(RecordTests, TwoRecordsWithSameColumnsAndValuesAreEqualDespiteColumnOrder)
{
  Record r1({ "LU", "LU_A" }, { Datum(1), Datum("a") });
  Record r3({ "LU_A", "LU" }, { Datum("a"), Datum(1) });
  EXPECT_TRUE(r1 == r3);
}

TEST(RecordTests, TwoRecordsWithSameColumnsWithDifferentTypesAreNotEqual)
{
  Record r1({ "LU", "LU_A" }, { Datum(1), Datum("a") });
  Record r4({ "LU_A", "LU" }, { Datum(2), Datum(1) });
  EXPECT_FALSE(r1 == r4);
}

TEST(RecordTests, TwoRecordsWithSameColumnsAndDifferentValuesAreNotEqual)
{
  Record r1({ "LU", "LU_A" }, { Datum(1), Datum("a") });
  Record r5({ "LU", "LU_A" }, { Datum(2), Datum("a") });
  Record r6({ "LU", "LU_A" }, { Datum(1), Datum("") });
  EXPECT_FALSE(r1 == r5);
  EXPECT_FALSE(r1 == r6);
  EXPECT_FALSE(r5 == r6);
}

TEST(RecordTests, TwoRecordsWithDifferentColumnSizeAreNotEqual)
{
  Record r1({ "LU", "LU_A" }, { Datum(1), Datum("a") });
  Record r7({ "LU", "LU_A", "Nombre" }, { Datum(2), Datum("a"), Datum("b") });
  Record r8({ "LU" }, { Datum(2) });
  EXPECT_FALSE(r1 == r7);
  EXPECT_FALSE(r1 == r8);
  EXPECT_FALSE(r7 == r8);
}

TEST(RecordTests, TwoRecordsAreEqualWhenCopied)
{
  Record r({ "A", "B", "C" }, { Datum(1), Datum(2), Datum(3) });
  auto recordCopy(r);
  EXPECT_EQ(recordCopy, r);
}
