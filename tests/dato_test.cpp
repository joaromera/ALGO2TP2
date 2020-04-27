#include "gtest/gtest.h"

#include "../src/Datum.h"

TEST(dato_test, generadores) {
    Datum(5);
    Datum("hola");
}

TEST(dato_test, esNat) {
    EXPECT_EQ(Datum(5).esNat(), true);
    EXPECT_EQ(Datum(100).esNat(), true);
    EXPECT_EQ(Datum("").esNat(), false);
    EXPECT_EQ(Datum("hola").esNat(), false);
}

TEST(dato_test, esString) {
    EXPECT_EQ(Datum("hola").esString(), true);
    EXPECT_EQ(Datum("chau").esString(), true);
    EXPECT_EQ(Datum(10).esString(), false);
    EXPECT_EQ(Datum(5).esString(), false);
}

TEST(dato_test, valores) {
    EXPECT_EQ(Datum(10).valorNat(), 10);
    EXPECT_EQ(Datum(5).valorNat(), 5);
    EXPECT_EQ(Datum(100).valorNat(), 100);
    EXPECT_EQ(Datum("").valorStr(), "");
    EXPECT_EQ(Datum("hola").valorStr(), "hola");
    EXPECT_EQ(Datum("chau").valorStr(), "chau");
}

TEST(dato_test, igobs) {
    EXPECT_EQ(Datum(5), Datum(5));
    EXPECT_NE(Datum(10), Datum(5));

    EXPECT_EQ(Datum("hola"), Datum("hola"));
    EXPECT_NE(Datum("holas"), Datum("hola"));
}

TEST(dato_test, shortcuts) {
    EXPECT_EQ(datoNat(5), Datum(5));
    EXPECT_EQ(datoStr("hola"), Datum("hola"));
}

TEST(dato_test, implicit_cast) {
    EXPECT_EQ(datoStr("Hola"), (Datum)"Hola");
}
