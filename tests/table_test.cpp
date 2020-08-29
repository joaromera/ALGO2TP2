#include "gtest/gtest.h"
#include "../src/Table.h"
#include <exception>

TEST(TableBasicTests, Construction)
{
  Table t1({ "LU", "LU_A" }, { "LU", "LU_A", "Nombre", "Carrera" },{ Datum(0), Datum(0), Datum(""), Datum("") });
  Table t2({ "Cod" },{ "Cod", "Carrera" },{ Datum(0), Datum("") });
}

TEST(TableBasicTests, EmptyTableHasSizeZero)
{
  Table t3({}, {}, {});
  EXPECT_EQ(t3.size(), (size_t) 0);
}

TEST(TableBasicTests, ConstructionFailsWhenTypesAreMissing)
{
  EXPECT_THROW(Table t({ }, { "LU"},{ }), std::invalid_argument);
  EXPECT_THROW(Table t({ "LU" },{ },{ }), std::invalid_argument);
}

TEST(TableBasicTests, TwoTablesAreEqualWhenTheyHaveSameKeysColumnsAndTypes)
{
  Table t1({ "LU" }, { "LU" }, { Datum("") });
  Table t2({ "LU" }, { "LU" }, { Datum("") });
  EXPECT_EQ(t1, t2);
}

TEST(TableBasicTests, TwoTablesAreEqualWhenTheyHaveSameKeysColumnsAndTypesDespiteColumnOrder)
{
  Table t1({ "LU" }, { "Nombre", "LU" }, { Datum(""), Datum("") });
  Table t2({ "LU" }, { "LU", "Nombre" }, { Datum(""), Datum("") });
  EXPECT_EQ(t1, t2);
}

TEST(TableBasicTests, TwoTablesAreNotEqualWhenTheyDifferentKeys)
{
  Table t1({ "LU" }, { "LU", "Nombre" }, { Datum(""), Datum("") });
  Table t2({ "Nombre" }, { "LU", "Nombre" }, { Datum(""), Datum("") });
  EXPECT_NE(t1, t2);
}

TEST(TableBasicTests, TwoTablesAreNotEqualWhenTheyDifferentTypes)
{
  Table t1({ "Nombre" }, { "Nombre", "LU" }, { Datum(""), Datum(0) });
  Table t2({ "Nombre" }, { "LU", "Nombre" }, { Datum(""), Datum("") });
  EXPECT_NE(t1, t2);
}

TEST(TableBasicTests, TwoTablesAreNotEqualWhenTheyDifferentRecords)
{
  Table t1({ "Nombre" }, { "Nombre", "LU" }, { Datum(""), Datum("") });
  Table t2({ "Nombre" }, { "LU", "Nombre" }, { Datum(""), Datum("") });
  t1.addRecord(Record({ "Nombre", "LU" }, { Datum("March"), Datum("64/9") }));
  EXPECT_NE(t1, t2);
}

TEST(TableBasicTests, TwoTablesAreNotEqualWhenTheySameRecords)
{
  Table t1({ "Nombre" }, { "Nombre", "LU" }, { Datum(""), Datum("") });
  Table t2({ "Nombre" }, { "LU", "Nombre" }, { Datum(""), Datum("") });
  t1.addRecord(Record({ "Nombre", "LU" }, { Datum("March"), Datum("64/9") }));
  t2.addRecord(Record({ "Nombre", "LU" }, { Datum("March"), Datum("64/9") }));
  EXPECT_EQ(t1, t2);
}

class TableTests : public ::testing::Test
{
protected:
  TableTests()
    : t({ "LU", "LU_A" }, { "LU", "LU_A", "Nombre", "Carrera" }, { Datum(0), Datum(0), Datum(""), Datum("") })
    , t2({ "Cod" }, { "Cod", "Carrera" }, { Datum(0), Datum("") })
  {
  }

  Table t;
  Table t2;
};

TEST_F(TableTests, ColumnsAreCorrectlyStoredAndRetrieved)
{
  EXPECT_EQ(t.columns(),linear_set<std::string>({ "LU", "LU_A", "Nombre", "Carrera" }));
  EXPECT_EQ(t2.columns(), linear_set<std::string>({ "Cod", "Carrera" }));
}

TEST_F(TableTests, KeysAreCorrectlyStoredAndRetrieved)
{
  EXPECT_EQ(t.keys(), linear_set<std::string>({ "LU", "LU_A" }));
  EXPECT_EQ(t2.keys(), linear_set<std::string>({ "Cod" }));
}

TEST_F(TableTests, TypesAreCorrectlyStoredAndRetrieved)
{
  EXPECT_EQ(t.columnType("LU").isInteger(), true);
  EXPECT_EQ(t.columnType("LU").isString(), false);
  EXPECT_EQ(t.columnType("Nombre").isInteger(), false);
  EXPECT_EQ(t.columnType("Nombre").isString(), true);
}

TEST_F(TableTests, AddingThreeRecords)
{
  std::vector<std::string> campos = { "LU", "LU_A", "Nombre", "Carrera" };
  Record r1(campos, std::vector<Datum>({ Datum(181), Datum(2017), Datum("March"), Datum("Comp") }));
  Record r2(campos, { Datum(182), Datum(2015), Datum("Ariana"), Datum("Mate") });
  Record r3(campos, { Datum(12), Datum(2005), Datum("Juan"), Datum("Biol") });

  EXPECT_EQ(t.size(), (size_t) 0);

  t.addRecord(r1);
  EXPECT_EQ(t.size(), (size_t) 1);
  EXPECT_EQ(*(t.records().begin()), r1);

  t.addRecord(r2);
  EXPECT_EQ(t.size(), (size_t) 2);
  EXPECT_TRUE(std::find(t.records().begin(), t.records().end(), r1) != t.records().end());
  EXPECT_TRUE(std::find(t.records().begin(), t.records().end(), r2) != t.records().end());
  EXPECT_FALSE(std::find(t.records().begin(), t.records().end(), r3) != t.records().end());

  t.addRecord(r3);
  EXPECT_EQ(t.size(), (size_t) 3);
  EXPECT_TRUE(std::find(t.records().begin(), t.records().end(), r1) != t.records().end());
  EXPECT_TRUE(std::find(t.records().begin(), t.records().end(), r2) != t.records().end());
  EXPECT_TRUE(std::find(t.records().begin(), t.records().end(), r3) != t.records().end());
}

TEST_F(TableTests, AddRecordAndTestIterators)
{
  EXPECT_TRUE(t2.records().empty());
  EXPECT_EQ(t2.begin(), t2.end());

  auto r1 = Record({ "Cod", "Carrera" }, { Datum(15), Datum("A") });
  auto rIt1 = t2.addRecord(r1);

  EXPECT_EQ(*rIt1, r1);
  EXPECT_EQ(rIt1, t2.begin());
  EXPECT_NE(t2.begin(), t2.end());
  EXPECT_FALSE(t2.records().empty());
  EXPECT_EQ(++rIt1, t2.end());
}

linear_set<Record> to_set(Table::const_iterator begin, Table::const_iterator end)
{
  linear_set<Record> res;
  while (begin != end) res.insert(*begin++);
  return res;
}

TEST(TablaIteratorTest, AddingRecordsAndIterating)
{
  Table t({ "LU" }, { "LU", "Nombre" }, { Datum{ "" }, Datum{ "" } });

  Record r1({ "LU", "Nombre" }, { Datum("123/01"), Datum("March") });
  t.addRecord(r1);

  Record r2({ "LU", "Nombre" }, { Datum("123/02"), Datum("Gerva") });
  t.addRecord(r2);

  Record r3({ "LU", "Nombre" }, { Datum("123/03"), Datum("Analía") });
  t.addRecord(r3);

  linear_set<Record> r_set = to_set(t.begin(), t.end());
  EXPECT_EQ(r_set, t.records());
}
