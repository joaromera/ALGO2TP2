#include "gtest/gtest.h"
#include "../src/Table.h"

linear_set<Record> to_set(Table::const_iterator begin, Table::const_iterator end)
{
  linear_set<Record> res;
  for (auto it = begin; it != end; ++it)
  {
    res.insert(*it);
  }
  return res;
}

class TablaTests : public ::testing::Test
{
protected:
  TablaTests() : t({ "LU", "LU_A" }, { "LU", "LU_A", "Nombre", "Carrera" }, { Datum(0), Datum(0), Datum(""), Datum("") }), t2({ "Cod" }, { "Cod", "Carrera" }, { Datum(0), Datum("") }){};

  Table t;
  Table t2;
};

TEST(tabla_test, crear)
{
  Table t({ "LU", "LU_A" },
    { "LU", "LU_A", "Nombre", "Carrera" },
    { Datum(0), Datum(0), Datum(""), Datum("") });
  Table t2({ "Cod" },
    { "Cod", "Carrera" },
    { Datum(0), Datum("") });
}

TEST_F(TablaTests, campos)
{
  EXPECT_EQ(t.columns(),
    linear_set<std::string>({ "LU", "LU_A", "Nombre", "Carrera" }));
  EXPECT_EQ(t2.columns(), linear_set<std::string>({ "Cod", "Carrera" }));
}

TEST_F(TablaTests, claves)
{
  EXPECT_EQ(t.keys(), linear_set<std::string>({ "LU", "LU_A" }));
  EXPECT_EQ(t2.keys(), linear_set<std::string>({ "Cod" }));
}

TEST_F(TablaTests, tipoCampo)
{
  EXPECT_EQ(t.columnType("LU").isInteger(), true);
  EXPECT_EQ(t.columnType("LU").isString(), false);
  EXPECT_EQ(t.columnType("Nombre").isInteger(), false);
  EXPECT_EQ(t.columnType("Nombre").isString(), true);
}

TEST_F(TablaTests, registros)
{
  Table t3({}, {}, {});
  EXPECT_EQ(t3.size(), (size_t) 0);

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

TEST_F(TablaTests, agregarRegistro)
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

TEST_F(TablaTests, igobs)
{
  Table t1({ "LU" }, { "LU" }, { Datum("") });
  Table t2({ "LU" }, { "LU" }, { Datum("") });
  EXPECT_EQ(t1, t2);// Claves == columns

  t1 = Table({ "LU" }, { "LU", "Nombre" }, { Datum(""), Datum("") });
  t2 = Table({ "LU" }, { "LU", "Nombre" }, { Datum(""), Datum("") });
  EXPECT_EQ(t1, t2);// Claves != columns

  t2 = Table({ "Nombre" }, { "LU", "Nombre" }, { Datum(""), Datum("") });
  EXPECT_NE(t1, t2);// Cambio key

  t1 = Table({ "LU" }, { "Nombre", "LU" }, { Datum(""), Datum("") });
  t2 = Table({ "LU" }, { "LU", "Nombre" }, { Datum(""), Datum("") });
  EXPECT_EQ(t1, t2);//  Campos distinto roden

  t1 = Table({ "Nombre" }, { "Nombre", "LU" }, { Datum(""), Datum("") });
  t2 = Table({ "Nombre" }, { "LU", "Nombre" }, { Datum(""), Datum("") });
  EXPECT_EQ(t1, t2);//  Campos distinto orden + otra key

  t1 = Table({ "Nombre" }, { "Nombre", "LU" }, { Datum(""), Datum(0) });
  t2 = Table({ "Nombre" }, { "LU", "Nombre" }, { Datum(""), Datum("") });
  EXPECT_NE(t1, t2);//  Campos distinto orden + distinto tipo no key

  t1 = Table({ "LU" }, { "Nombre", "LU" }, { Datum(""), Datum(0) });
  t2 = Table({ "LU" }, { "LU", "Nombre" }, { Datum(""), Datum("") });
  EXPECT_NE(t1, t2);//  Campos distinto orden + distinto tipo key

  t1 = Table({ "Nombre" }, { "Nombre", "LU" }, { Datum(""), Datum("") });
  t2 = Table({ "Nombre" }, { "LU", "Nombre" }, { Datum(""), Datum("") });
  t1.addRecord(Record({ "Nombre", "LU" }, { Datum("March"), Datum("64/9") }));
  EXPECT_NE(t1, t2);// Distintos records

  t2.addRecord(Record({ "Nombre", "LU" }, { Datum("March"), Datum("64/9") }));
  EXPECT_EQ(t1, t2);// Igualo records

  t1.addRecord(Record({ "Nombre", "LU" }, { Datum("Gerva"), Datum("65/9") }));
  EXPECT_NE(t1, t2);// Distintos records

  t2.addRecord(Record({ "LU", "Nombre" }, { Datum("65/9"), Datum("Gerva") }));
  EXPECT_EQ(t1, t2);// Igualo records pero con columns al reves

  t1.addRecord(Record({ "Nombre", "LU" }, { Datum("Ana"), Datum("100/10") }));
  t1.addRecord(Record({ "Nombre", "LU" }, { Datum("Luis"), Datum("101/10") }));
  t2.addRecord(Record({ "Nombre", "LU" }, { Datum("Luis"), Datum("101/10") }));
  t2.addRecord(Record({ "Nombre", "LU" }, { Datum("Ana"), Datum("100/10") }));
  EXPECT_EQ(t1, t2);// Agrego records en otro orden
}

TEST(tabla_test, it_registro)
{
  // Estos tests surgieron por un sigsev al crear el iterador de registro
  Table t({ "LU" }, { "LU", "Nombre" }, { Datum{ "" }, Datum{ "" } });
  Record r1({ "LU", "Nombre" }, { Datum("123/01"), Datum("March") });
  t.addRecord(r1);

  EXPECT_EQ(*t.begin(), r1);
  EXPECT_EQ(t.begin()->dato("LU"), Datum("123/01"));
  EXPECT_EQ(++t.begin(), t.end());

  Record r2({ "LU", "Nombre" }, { Datum("123/02"), Datum("Gerva") });
  Record r3({ "LU", "Nombre" }, { Datum("123/03"), Datum("Analía") });

  t.addRecord(r2);
  t.addRecord(r3);
  linear_set<Record> r_set = to_set(t.begin(), t.end());
  EXPECT_EQ(r_set, t.records());
}

TEST(tabla_test, iteraremos)
{
  Table t({ "LU" }, { "LU", "Nombre" }, { Datum{ "" }, Datum{ "" } });
  Record r1({ "LU", "Nombre" }, { Datum("123/01"), Datum("March") });
  t.addRecord(r1);
  Record r2({ "LU", "Nombre" }, { Datum("123/02"), Datum("Gerva") });
  Record r3({ "LU", "Nombre" }, { Datum("123/03"), Datum("Analía") });
  t.addRecord(r2);
  t.addRecord(r3);
  linear_set<Record> r_set = to_set(t.begin(), t.end());

//  for (auto it = t.begin(); it != t.end(); ++it)
//  {
//    std::cout << it->datum("LU") << std::endl;
//  }
}