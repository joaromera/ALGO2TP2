#include "gtest/gtest.h"
#include "../src/Datum.h"
#include "../src/Record.h"

using namespace std;

TEST(registro_test, generadores)
{
  Record({ "LU", "LU_A", "Nombre", "Carrera" }, { Datum(182), Datum(18), Datum("March"), Datum("Computacion") });
  Record({}, {});
}

TEST(registro_test, campos)
{
  Record r({ "A", "B", "C" }, { Datum(1), Datum(2), Datum(3) });
  Record r2({}, {});

  EXPECT_EQ(r.campos(), linear_set<string>({ "A", "B", "C" }));
  EXPECT_EQ(r2.campos(), linear_set<string>());
}

TEST(registro_test, dato)
{
  Record r({ "LU", "LU_A", "Nombre", "Carrera" }, { Datum(182), Datum(18), Datum("March"), Datum("Computacion") });

  EXPECT_EQ(r.dato("LU").isInteger(), true);
  EXPECT_EQ(r.dato("LU").isString(), false);
  EXPECT_EQ(r.dato("LU").integerValue(), 182);
  EXPECT_EQ(r.dato("Nombre").isInteger(), false);
  EXPECT_EQ(r.dato("Nombre").isString(), true);
  EXPECT_EQ(r.dato("Nombre").stringValue(), "March");
}

TEST(registro_test, ig_obs)
{
  Record r1({ "LU", "LU_A" }, { Datum(1), Datum("a") });
  Record r2({ "LU", "LU_A" }, { Datum(1), Datum("a") });

  // Iguales
  EXPECT_TRUE(r1 == r2);

  // Distinto orden
  Record r3({ "LU_A", "LU" }, { Datum("a"), Datum(1) });
  EXPECT_TRUE(r1 == r3);

  // Distinto tipo
  Record r4({ "LU_A", "LU" }, { Datum(2), Datum(1) });
  EXPECT_FALSE(r1 == r4);

  // Distinto valor
  Record r5({ "LU", "LU_A" }, { Datum(2), Datum("a") });
  EXPECT_FALSE(r1 == r5);

  // Más columns
  Record r6({ "LU", "LU_A", "Nombre" },
    { Datum(2), Datum("a"), Datum("b") });
  EXPECT_FALSE(r1 == r6);
}

TEST(registro_test, copy)
{
  Record r({ "A", "B", "C" }, { Datum(1), Datum(2), Datum(3) });
  auto copy_r(r);
  EXPECT_EQ(copy_r, r);
}
