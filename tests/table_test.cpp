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
  EXPECT_EQ(t.campos(),
    linear_set<std::string>({ "LU", "LU_A", "Nombre", "Carrera" }));
  EXPECT_EQ(t2.campos(), linear_set<std::string>({ "Cod", "Carrera" }));
}

TEST_F(TablaTests, claves)
{
  EXPECT_EQ(t.claves(), linear_set<std::string>({ "LU", "LU_A" }));
  EXPECT_EQ(t2.claves(), linear_set<std::string>({ "Cod" }));
}

TEST_F(TablaTests, tipoCampo)
{
  EXPECT_EQ(t.tipoCampo("LU").esNat(), true);
  EXPECT_EQ(t.tipoCampo("LU").esString(), false);
  EXPECT_EQ(t.tipoCampo("Nombre").esNat(), false);
  EXPECT_EQ(t.tipoCampo("Nombre").esString(), true);
}

TEST_F(TablaTests, registros)
{
  Table t3({}, {}, {});
  EXPECT_EQ(t3.cant_registros(), 0);

  std::vector<std::string> campos = { "LU", "LU_A", "Nombre", "Carrera" };
  Record r1(campos, std::vector<Datum>({ Datum(181), Datum(2017), Datum("March"), Datum("Comp") }));
  Record r2(campos, { Datum(182), Datum(2015), Datum("Ariana"), Datum("Mate") });
  Record r3(campos, { Datum(12), Datum(2005), Datum("Juan"), Datum("Biol") });

  EXPECT_EQ(t.cant_registros(), 0);
  t.agregarRegistro(r1);
  EXPECT_EQ(t.cant_registros(), 1);
  EXPECT_EQ(*(t.registros().begin()), r1);
  t.agregarRegistro(r2);
  EXPECT_EQ(t.cant_registros(), 2);
  EXPECT_TRUE(std::find(t.registros().begin(), t.registros().end(), r1) != t.registros().end());
  EXPECT_TRUE(std::find(t.registros().begin(), t.registros().end(), r2) != t.registros().end());
  EXPECT_FALSE(std::find(t.registros().begin(), t.registros().end(), r3) != t.registros().end());
  t.agregarRegistro(r3);
  EXPECT_EQ(t.cant_registros(), 3);
  EXPECT_TRUE(std::find(t.registros().begin(), t.registros().end(), r1) != t.registros().end());
  EXPECT_TRUE(std::find(t.registros().begin(), t.registros().end(), r2) != t.registros().end());
  EXPECT_TRUE(std::find(t.registros().begin(), t.registros().end(), r3) != t.registros().end());
}

TEST_F(TablaTests, agregarRegistro)
{
  EXPECT_TRUE(t2.registros().empty());
  EXPECT_EQ(t2.registros_begin(), t2.registros_end());
  auto r1 = Record({ "Cod", "Carrera" }, { Datum(15), Datum("A") });
  auto rIt1 = t2.agregarRegistro(r1);
  EXPECT_EQ(*rIt1, r1);
  EXPECT_EQ(rIt1, t2.registros_begin());
  EXPECT_NE(t2.registros_begin(), t2.registros_end());
  EXPECT_FALSE(t2.registros().empty());
  EXPECT_EQ(++rIt1, t2.registros_end());
}

TEST_F(TablaTests, igobs)
{
  Table t1({ "LU" }, { "LU" }, { Datum("") });
  Table t2({ "LU" }, { "LU" }, { Datum("") });
  EXPECT_EQ(t1, t2);// Claves == campos

  t1 = Table({ "LU" }, { "LU", "Nombre" }, { Datum(""), Datum("") });
  t2 = Table({ "LU" }, { "LU", "Nombre" }, { Datum(""), Datum("") });
  EXPECT_EQ(t1, t2);// Claves != campos

  t2 = Table({ "Nombre" }, { "LU", "Nombre" }, { Datum(""), Datum("") });
  EXPECT_NE(t1, t2);// Cambio clave

  t1 = Table({ "LU" }, { "Nombre", "LU" }, { Datum(""), Datum("") });
  t2 = Table({ "LU" }, { "LU", "Nombre" }, { Datum(""), Datum("") });
  EXPECT_EQ(t1, t2);//  Campos distinto roden

  t1 = Table({ "Nombre" }, { "Nombre", "LU" }, { Datum(""), Datum("") });
  t2 = Table({ "Nombre" }, { "LU", "Nombre" }, { Datum(""), Datum("") });
  EXPECT_EQ(t1, t2);//  Campos distinto orden + otra clave

  t1 = Table({ "Nombre" }, { "Nombre", "LU" }, { Datum(""), Datum(0) });
  t2 = Table({ "Nombre" }, { "LU", "Nombre" }, { Datum(""), Datum("") });
  EXPECT_NE(t1, t2);//  Campos distinto orden + distinto tipo no clave

  t1 = Table({ "LU" }, { "Nombre", "LU" }, { Datum(""), Datum(0) });
  t2 = Table({ "LU" }, { "LU", "Nombre" }, { Datum(""), Datum("") });
  EXPECT_NE(t1, t2);//  Campos distinto orden + distinto tipo clave

  t1 = Table({ "Nombre" }, { "Nombre", "LU" }, { Datum(""), Datum("") });
  t2 = Table({ "Nombre" }, { "LU", "Nombre" }, { Datum(""), Datum("") });
  t1.agregarRegistro(Record({ "Nombre", "LU" }, { Datum("March"), Datum("64/9") }));
  EXPECT_NE(t1, t2);// Distintos registros

  t2.agregarRegistro(Record({ "Nombre", "LU" }, { Datum("March"), Datum("64/9") }));
  EXPECT_EQ(t1, t2);// Igualo registros

  t1.agregarRegistro(Record({ "Nombre", "LU" }, { Datum("Gerva"), Datum("65/9") }));
  EXPECT_NE(t1, t2);// Distintos registros

  t2.agregarRegistro(Record({ "LU", "Nombre" }, { Datum("65/9"), Datum("Gerva") }));
  EXPECT_EQ(t1, t2);// Igualo registros pero con campos al reves

  t1.agregarRegistro(Record({ "Nombre", "LU" }, { Datum("Ana"), Datum("100/10") }));
  t1.agregarRegistro(Record({ "Nombre", "LU" }, { Datum("Luis"), Datum("101/10") }));
  t2.agregarRegistro(Record({ "Nombre", "LU" }, { Datum("Luis"), Datum("101/10") }));
  t2.agregarRegistro(Record({ "Nombre", "LU" }, { Datum("Ana"), Datum("100/10") }));
  EXPECT_EQ(t1, t2);// Agrego registros en otro orden
}

TEST(tabla_test, it_registro)
{
  // Estos tests surgieron por un sigsev al crear el iterador de registro
  Table t({ "LU" }, { "LU", "Nombre" }, { Datum{ "" }, Datum{ "" } });
  Record r1({ "LU", "Nombre" }, { Datum("123/01"), Datum("March") });
  t.agregarRegistro(r1);

  EXPECT_EQ(*t.registros_begin(), r1);
  EXPECT_EQ(t.registros_begin()->dato("LU"), Datum("123/01"));
  EXPECT_EQ(++t.registros_begin(), t.registros_end());

  Record r2({ "LU", "Nombre" }, { Datum("123/02"), Datum("Gerva") });
  Record r3({ "LU", "Nombre" }, { Datum("123/03"), Datum("Analía") });

  t.agregarRegistro(r2);
  t.agregarRegistro(r3);
  linear_set<Record> r_set = to_set(t.registros_begin(), t.registros_end());
  EXPECT_EQ(r_set, t.registros());
}

TEST(tabla_test, iteraremos)
{
  Table t({ "LU" }, { "LU", "Nombre" }, { Datum{ "" }, Datum{ "" } });
  Record r1({ "LU", "Nombre" }, { Datum("123/01"), Datum("March") });
  t.agregarRegistro(r1);
  Record r2({ "LU", "Nombre" }, { Datum("123/02"), Datum("Gerva") });
  Record r3({ "LU", "Nombre" }, { Datum("123/03"), Datum("Analía") });
  t.agregarRegistro(r2);
  t.agregarRegistro(r3);
  linear_set<Record> r_set = to_set(t.registros_begin(), t.registros_end());

//  for (auto it = t.registros_begin(); it != t.registros_end(); ++it)
//  {
//    std::cout << it->dato("LU") << std::endl;
//  }
}