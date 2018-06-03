#include "gtest/gtest.h"
#include "../src/Tabla.h"
#include "../src/utils.h"

using namespace std;

template<class T>
bool pertenece(const T &t, const linear_set<T> &s) {
    return s.count(t);
}

linear_set<Registro> to_set(Tabla::const_iterador_registros begin,
                            Tabla::const_iterador_registros end) {
    linear_set<Registro> res;
    for (auto it = begin; it != end; ++it) {
        res.insert(*it);
    }
    return res;
}

class TablaTests : public ::testing::Test {

protected:
    TablaTests() : t({"LU", "LU_A"},
                     {"LU", "LU_A", "Nombre", "Carrera"},
                     {datoNat(0), datoNat(0), datoStr(""), datoStr("")}),
                   t2({"Cod"},
                      {"Cod", "Carrera"},
                      {datoNat(0), datoStr("")}) {};

    Tabla t;
    Tabla t2;

};

TEST(tabla_test, crear) {
    Tabla t({"LU", "LU_A"},
            {"LU", "LU_A", "Nombre", "Carrera"},
            {datoNat(0), datoNat(0), datoStr(""), datoStr("")});
    Tabla t2({"Cod"},
             {"Cod", "Carrera"},
             {datoNat(0), datoStr("")});
};

TEST_F(TablaTests, campos) {
    EXPECT_EQ(t.campos(),
              linear_set<string>({"LU", "LU_A", "Nombre", "Carrera"}));
    EXPECT_EQ(t2.campos(), linear_set<string>({"Cod", "Carrera"}));
};

TEST_F(TablaTests, claves) {
    EXPECT_EQ(t.claves(), linear_set<string>({"LU", "LU_A"}));
    EXPECT_EQ(t2.claves(), linear_set<string>({"Cod"}));
}

TEST_F(TablaTests, tipoCampo) {
    EXPECT_EQ(t.tipoCampo("LU").esNat(), true);
    EXPECT_EQ(t.tipoCampo("LU").esString(), false);
    EXPECT_EQ(t.tipoCampo("Nombre").esNat(), false);
    EXPECT_EQ(t.tipoCampo("Nombre").esString(), true);
}

TEST_F(TablaTests, registros) {
    Tabla t3({}, {}, {});
    EXPECT_EQ(t3.cant_registros(), 0);

    vector<string> campos = {"LU", "LU_A", "Nombre", "Carrera"};
    Registro r1(campos, vector<Dato>({datoNat(181), datoNat(2017), datoStr("March"), datoStr("Comp")}));
    Registro r2(campos, {datoNat(182), datoNat(2015), datoStr("Ariana"), datoStr("Mate")});
    Registro r3(campos, {datoNat(12), datoNat(2005), datoStr("Juan"), datoStr("Biol")});

    EXPECT_EQ(t.cant_registros(), 0);
    t.agregarRegistro(r1);
    EXPECT_EQ(t.cant_registros(), 1);
    EXPECT_EQ(*(t.registros().begin()), r1);
    t.agregarRegistro(r2);
    EXPECT_EQ(t.cant_registros(), 2);
    EXPECT_TRUE(pertenece(r1, t.registros()));
    EXPECT_TRUE(pertenece(r2, t.registros()));
    EXPECT_FALSE(pertenece(r3, t.registros()));
    t.agregarRegistro(r3);
    EXPECT_EQ(t.cant_registros(), 3);
    EXPECT_TRUE(pertenece(r1, t.registros()));
    EXPECT_TRUE(pertenece(r2, t.registros()));
    EXPECT_TRUE(pertenece(r3, t.registros()));
}

TEST_F(TablaTests, agregarRegistro) {
    EXPECT_TRUE(t2.registros().empty());
    EXPECT_EQ(t2.registros_begin(), t2.registros_end());
    auto r1 = Registro({"Cod", "Carrera"}, {Dato(15), Dato("A")});
    auto rIt1 = t2.agregarRegistro(r1);
    EXPECT_EQ(*rIt1, r1);
    EXPECT_EQ(rIt1, t2.registros_begin());
    EXPECT_NE(t2.registros_begin(), t2.registros_end());
    EXPECT_FALSE(t2.registros().empty());
    EXPECT_EQ(++rIt1, t2.registros_end());
}

TEST_F(TablaTests, igobs) {
    Tabla t1({"LU"}, {"LU"}, {datoStr("")});
    Tabla t2({"LU"}, {"LU"}, {datoStr("")});
    EXPECT_EQ(t1, t2); // Claves == campos

    t1 = Tabla({"LU"}, {"LU", "Nombre"}, {datoStr(""), datoStr("")});
    t2 = Tabla({"LU"}, {"LU", "Nombre"}, {datoStr(""), datoStr("")});
    EXPECT_EQ(t1, t2); // Claves != campos

    t2 = Tabla({"Nombre"}, {"LU", "Nombre"}, {datoStr(""), datoStr("")});
    EXPECT_NE(t1, t2); // Cambio clave

    t1 = Tabla({"LU"}, {"Nombre", "LU"}, {datoStr(""), datoStr("")});
    t2 = Tabla({"LU"}, {"LU", "Nombre"}, {datoStr(""), datoStr("")});
    EXPECT_EQ(t1, t2); //  Campos distinto roden

    t1 = Tabla({"Nombre"}, {"Nombre", "LU"}, {datoStr(""), datoStr("")});
    t2 = Tabla({"Nombre"}, {"LU", "Nombre"}, {datoStr(""), datoStr("")});
    EXPECT_EQ(t1, t2); //  Campos distinto orden + otra clave

    t1 = Tabla({"Nombre"}, {"Nombre", "LU"}, {datoStr(""), datoNat(0)});
    t2 = Tabla({"Nombre"}, {"LU", "Nombre"}, {datoStr(""), datoStr("")});
    EXPECT_NE(t1, t2); //  Campos distinto orden + distinto tipo no clave

    t1 = Tabla({"LU"}, {"Nombre", "LU"}, {datoStr(""), datoNat(0)});
    t2 = Tabla({"LU"}, {"LU", "Nombre"}, {datoStr(""), datoStr("")});
    EXPECT_NE(t1, t2); //  Campos distinto orden + distinto tipo clave

    t1 = Tabla({"Nombre"}, {"Nombre", "LU"}, {datoStr(""), datoStr("")});
    t2 = Tabla({"Nombre"}, {"LU", "Nombre"}, {datoStr(""), datoStr("")});
    t1.agregarRegistro(Registro({"Nombre", "LU"}, {datoStr("March"), datoStr("64/9")}));
    EXPECT_NE(t1, t2); // Distintos registros

    t2.agregarRegistro(Registro({"Nombre", "LU"}, {datoStr("March"), datoStr("64/9")}));
    EXPECT_EQ(t1, t2); // Igualo registros

    t1.agregarRegistro(Registro({"Nombre", "LU"}, {datoStr("Gerva"), datoStr("65/9")}));
    EXPECT_NE(t1, t2); // Distintos registros

    t2.agregarRegistro(Registro({"LU", "Nombre"}, {datoStr("65/9"), datoStr("Gerva")}));
    EXPECT_EQ(t1, t2); // Igualo registros pero con campos al reves

    t1.agregarRegistro(Registro({"Nombre", "LU"}, {datoStr("Ana"), datoStr("100/10")}));
    t1.agregarRegistro(Registro({"Nombre", "LU"}, {datoStr("Luis"), datoStr("101/10")}));
    t2.agregarRegistro(Registro({"Nombre", "LU"}, {datoStr("Luis"), datoStr("101/10")}));
    t2.agregarRegistro(Registro({"Nombre", "LU"}, {datoStr("Ana"), datoStr("100/10")}));
    EXPECT_EQ(t1, t2); // Agrego registros en otro orden
}

TEST(tabla_test, it_registro) {
    // Estos tests surgieron por un sigsev al crear el iterador de registro
    Tabla t({"LU"}, {"LU", "Nombre"}, {tipoStr, tipoStr});
    Registro r1({"LU", "Nombre"}, {datoStr("123/01"), datoStr("March")});
    t.agregarRegistro(r1);

    EXPECT_EQ(*t.registros_begin(), r1);
    EXPECT_EQ(t.registros_begin()->dato("LU"), datoStr("123/01"));
    EXPECT_EQ(++t.registros_begin(), t.registros_end());

    Registro r2({"LU", "Nombre"}, {datoStr("123/02"), datoStr("Gerva")});
    Registro r3({"LU", "Nombre"}, {datoStr("123/03"), datoStr("Analía")});

    t.agregarRegistro(r2);
    t.agregarRegistro(r3);
    linear_set<Registro> r_set = to_set(t.registros_begin(), t.registros_end());
    EXPECT_EQ(r_set, t.registros());
}

TEST(tabla_test, iteraremos) {
    Tabla t({"LU"}, {"LU", "Nombre"}, {tipoStr, tipoStr});
    Registro r1({"LU", "Nombre"}, {datoStr("123/01"), datoStr("March")});
    t.agregarRegistro(r1);
    Registro r2({"LU", "Nombre"}, {datoStr("123/02"), datoStr("Gerva")});
    Registro r3({"LU", "Nombre"}, {datoStr("123/03"), datoStr("Analía")});
    t.agregarRegistro(r2);
    t.agregarRegistro(r3);
    linear_set<Registro> r_set = to_set(t.registros_begin(), t.registros_end());
    for (auto it = t.registros_begin(); it != t.registros_end(); ++it) {
        cout << it->dato("LU") << endl;
    }
}