#include "gtest/gtest.h"

#include "../src/Dato.h"

TEST(dato_test, generadores) {
    Dato(5);
    Dato("hola");
}

TEST(dato_test, esNat) {
    EXPECT_EQ(Dato(5).esNat(), true);
    EXPECT_EQ(Dato(100).esNat(), true);
    EXPECT_EQ(Dato("").esNat(), false);
    EXPECT_EQ(Dato("hola").esNat(), false);
}

TEST(dato_test, esString) {
    EXPECT_EQ(Dato("hola").esString(), true);
    EXPECT_EQ(Dato("chau").esString(), true);
    EXPECT_EQ(Dato(10).esString(), false);
    EXPECT_EQ(Dato(5).esString(), false);
}

TEST(dato_test, valores) {
    EXPECT_EQ(Dato(10).valorNat(), 10);
    EXPECT_EQ(Dato(5).valorNat(), 5);
    EXPECT_EQ(Dato(100).valorNat(), 100);
    EXPECT_EQ(Dato("").valorStr(), "");
    EXPECT_EQ(Dato("hola").valorStr(), "hola");
    EXPECT_EQ(Dato("chau").valorStr(), "chau");
}

TEST(dato_test, igobs) {
    EXPECT_EQ(Dato(5), Dato(5));
    EXPECT_NE(Dato(10), Dato(5));

    EXPECT_EQ(Dato("hola"), Dato("hola"));
    EXPECT_NE(Dato("holas"), Dato("hola"));
}

TEST(dato_test, shortcuts) {
    EXPECT_EQ(datoNat(5), Dato(5));
    EXPECT_EQ(datoStr("hola"), Dato("hola"));
}

TEST(dato_test, implicit_cast) {
    EXPECT_EQ(datoStr("Hola"), (Dato)"Hola");
}
