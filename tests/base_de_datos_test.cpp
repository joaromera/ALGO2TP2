#include "../src/BaseDeDatos.h"
#include "../src/Datum.h"
#include "gtest/gtest.h"
#include <algorithm>
#include <vector>
#include <string>

using namespace std;

template<class iter>
bool incluye(iter begin_a,
             iter end_a,
             iter begin_b,
             iter end_b) {
    for (auto it = begin_b; it != end_b; ++it) {
        auto it2 = begin_a;
        for (; it2 != end_a; ++it2) {
            if (*it2 == *it) {
                break;
            }
        }
        if (it2 == end_a) {
            return false;
        }
    }
    return true;
}

TEST(test_basicos, constructor) {
    BaseDeDatos db;

    EXPECT_TRUE(db.tablas().empty());
    EXPECT_FALSE(db.uso_criterio(BaseDeDatos::Criterio()));
}

TEST(test_basicos, tabla_alumnos) {
    BaseDeDatos db;

    vector<string> campos_alumnos({"LU", "Ano", "Nombre", "DNI"});
    vector<Datum> tipos_alumnos = {tipoNat, tipoNat, tipoStr, tipoStr};
    linear_set<string> claves_alumnos = {"LU", "Ano"};

    db.crearTabla("alumnos", claves_alumnos, campos_alumnos, tipos_alumnos);

    EXPECT_EQ(db.dameTabla("alumnos"),
              Tabla(claves_alumnos, campos_alumnos, tipos_alumnos));

    Registro gerva =
            Registro(campos_alumnos,
                     {datoNat(123), datoNat(45), datoStr("Gerva"), datoStr("10001")});

    EXPECT_TRUE(db.registroValido(gerva, "alumnos"));
    db.agregarRegistro(gerva, "alumnos");
    EXPECT_FALSE(db.registroValido(gerva, "alumnos"));

    Registro gerva2 =
            Registro(campos_alumnos,
                     {datoNat(124), datoNat(45), datoStr("Gerva"), datoStr("10001")});
    EXPECT_TRUE(db.registroValido(gerva2, "alumnos"));
    db.agregarRegistro(gerva2, "alumnos");
    EXPECT_FALSE(db.registroValido(gerva2, "alumnos"));

    Registro gerva_clon =
            Registro(campos_alumnos,
                     {datoNat(124), datoNat(45), datoStr("Gerva"), datoStr("10001")});
    EXPECT_FALSE(db.registroValido(gerva_clon, "alumnos"));
}

class DBAlumnos : public ::testing::Test {
protected:
    typedef linear_set<string> Claves;
    typedef vector<string> Campos;
    typedef vector<Datum> Tipos;

    struct DefinicionTabla {
        DefinicionTabla(Claves _claves, Campos _campos, Tipos _tipos)
                : claves(_claves), campos(_campos), tipos(_tipos) {}

        Claves claves;
        Campos campos;
        Tipos tipos;
    };


    BaseDeDatos db;
    DefinicionTabla def_libretas;
    DefinicionTabla def_alumnos;
    DefinicionTabla def_materias;
    DefinicionTabla def_exalumnos;
    DefinicionTabla def_join_libretas_alumnos;
    DefinicionTabla def_join_libretas_materias;
    Tabla libretas;
    Tabla alumnos;
    Tabla materias;
    Tabla ex_alumnos;
    Tabla join_libretas_alumnos;
    Tabla join_libretas_materias;

    Tabla def_a_tabla(const DefinicionTabla &def) {
        return Tabla(def.claves, def.campos, def.tipos);
    }

    void definir_tabla(const string &nombre, const DefinicionTabla &def) {
        db.crearTabla(nombre, def.claves, def.campos, def.tipos);
    }

    void iniciarTablas() {
        definir_tabla("alumnos", def_alumnos);
        definir_tabla("libretas", def_libretas);
        definir_tabla("materias", def_materias);
        definir_tabla("ex_alumnos", def_exalumnos);
    }

    void cargarAlumno(const int &lu_n, const int &lu_a, const string &nombre,
                      const string &editor, const string &os,
                      const vector<string> &mts) {
        string lu = to_string(lu_n) + '/' + to_string(lu_a);
        Registro r_libretas = Registro(
                {"LU_N", "LU_A", "LU"}, {datoNat(lu_n), datoNat(lu_a), datoStr(lu)});
        libretas.agregarRegistro(r_libretas);
        db.agregarRegistro(r_libretas, "libretas");

        Registro r_alumno = Registro(
                {"LU", "Nombre", "Editor", "OS"},
                {datoStr(lu), datoStr(nombre), datoStr(editor), datoStr(os)});
        alumnos.agregarRegistro(r_alumno);
        db.agregarRegistro(r_alumno, "alumnos");

        Registro r_join_libretas_alumnos = Registro(
                {"LU_N", "LU_A", "LU", "Nombre", "Editor", "OS"},
                {Datum(lu_n), Datum(lu_a), Datum(lu), Datum(nombre), Datum(editor),
                 Datum(os)});
        join_libretas_alumnos.agregarRegistro(r_join_libretas_alumnos);

        for (auto mat : mts) {
            Registro r_materia = Registro(
                    {"LU", "Materia"}, {datoStr(lu), datoStr(mat)});
            materias.agregarRegistro(r_materia);
            db.agregarRegistro(r_materia, "materias");

            Registro r_join_libretas_materias = Registro(
                    {"LU_N", "LU_A", "LU", "Materia"},
                    {Datum(lu_n), Datum(lu_a), Datum(lu), Datum(mat)});
            join_libretas_materias.agregarRegistro(r_join_libretas_materias);

        }
    }

    Registro registro(const DefinicionTabla &def,
                      const vector<Datum> &valores) {
        return Registro(def.campos, valores);
    }

    DBAlumnos() :
            def_libretas({"LU_N", "LU_A", "LU"},
                         {"LU_N", "LU_A", "LU"},
                         {tipoNat, tipoNat, tipoStr}),
            def_alumnos({"LU"},
                        {"LU", "Nombre", "Editor", "OS"},
                        {tipoStr, tipoStr, tipoStr, tipoStr}),
            def_materias({"LU", "Materia"},
                         {"LU", "Materia"},
                         {tipoStr, tipoStr}),
            def_exalumnos({"LU", "Egreso"},
                          {"LU"},
                          {tipoStr, tipoNat}),
            def_join_libretas_alumnos({"LU_N", "LU_A", "LU", "Nombre",
                                       "Editor", "OS"},
                                      {"LU"},
                                      {tipoNat, tipoNat, tipoStr, tipoStr,
                                       tipoStr, tipoStr, tipoStr}),
            def_join_libretas_materias({"LU_N", "LU_A", "LU", "Materia"},
                                       {"LU"},
                                       {tipoNat, tipoNat, tipoStr, tipoStr}),
            libretas(def_a_tabla(def_libretas)),
            alumnos(def_a_tabla(def_alumnos)),
            materias(def_a_tabla(def_materias)),
            ex_alumnos(def_a_tabla(def_exalumnos)),
            join_libretas_alumnos(def_a_tabla(def_join_libretas_alumnos)),
            join_libretas_materias(def_a_tabla(def_join_libretas_materias)) {
        iniciarTablas();
        cargarAlumno(1, 90, "March", "Vim", "Linux", {"AED2"});
        cargarAlumno(2, 80, "Old-school", "Vim", "Linux", {"OOP", "AED2"});
        cargarAlumno(3, 90, "GenY", "Code-blocks", "Win", {"AED1"});
        cargarAlumno(4, 1, "Crack", "CLion", "Win", {"AED2", "AED1"});
        cargarAlumno(5, 80, "Tron", "Vim", "macOS", {"Orga2"});
        cargarAlumno(6, 80, "CLU", "Vim", "macOS", {"Orga1"});
        cargarAlumno(5, 2, "Hipster", "Vim", "macOS", {"AED3"});

        db.agregarRegistro(Registro({"LU", "Egreso"},
                                    {Datum("100/70"), Datum(75)}),
                           "ex_alumnos");
        db.agregarRegistro(Registro({"LU", "Egreso"},
                                    {Datum("102/70"), Datum(75)}),
                           "ex_alumnos");
    }

};

// Lista de tests:
// ## Agregar registro
// * Agregar un, dos, tres registros (✓)
// ## Validar registro
// * Registro inválido por campos (✓)
// * Registro inválido por tipos (✓)
// * Registro invalido por claves (✓)
// * Registro valido si todo ok (✓)
// ## Búsqueda 
// * Búsqueda devuelve tabla equivalente (✓)
// * Búsqueda por igual (✓)
// * Búsqueda por distinto (✓)
// * Búsqueda doble igual (✓)
// * Búsqueda doble distinto (✓)
// * Búsqueda igual distinto (✓)
// * Búsqueda doble igual distinto (✓)
// ## Criterio Válido
// * Criterio simple, inválido por nombre (✓)
// * Criterio simple, inválido por tipo (✓)
// * Criterio doble, inválido por nombre (✓)
// * Criterio doble, inválido por tipo (✓)
// ## Uso Criterio
// * Uso un criterio (✓)
// * Uso criterio perm (✓)
// * Uso criterio parecido pero distinto bool (✓)
// * Uso criterio doble parecido pero distinto bool (✓)
// ## Indice 
// * Crear índice
// * Observador índice
// ## Join
// * Join vacío (✓)
// * Join sin repetidos (✓)
// * Join repetidos un lado (✓)
// * Join repetidos dos lados (✓)
// * Join campos repetidos (✓) 

TEST_F(DBAlumnos, init) {
    EXPECT_EQ(libretas, db.dameTabla("libretas"));
    EXPECT_EQ(alumnos, db.dameTabla("alumnos"));
    EXPECT_EQ(materias, db.dameTabla("materias"));
}

// ## Agregar registro
TEST_F(DBAlumnos, agregar_registro) {
    Registro alu1 = registro(def_libretas,
                             {datoNat(10), datoNat(10), datoStr("10/10")});
    Registro alu2 = registro(def_libretas,
                             {datoNat(11), datoNat(10), datoStr("11/10")});
    Registro alu3 = registro(def_libretas,
                             {datoNat(12), datoNat(10), datoStr("12/10")});

    int cant_libretas = db.dameTabla("libretas").registros().size();

    vector<Registro> regs = {alu1, alu2, alu3};

    for (size_t i = 0; i < regs.size(); i++) { // Voy agregando de a uno
        EXPECT_EQ(db.dameTabla("libretas").registros().size(), cant_libretas + i);
        for (size_t j = i; j < regs.size(); j++) { // Antes de agregar faltan el resto
            EXPECT_FALSE(db.dameTabla("libretas").registros().count(regs[j]));
        }
        db.agregarRegistro(regs[i], "libretas");
        // Ahora no falta el agregado
        EXPECT_TRUE(db.dameTabla("libretas").registros().count(regs[i]));
        // Pero siguen faltando los otros
        for (size_t j = i + 1; j < regs.size(); j++) {
            EXPECT_FALSE(db.dameTabla("libretas").registros().count(regs[j]));
        }
        // Se actualizó la cantidad de registros
        EXPECT_EQ(db.dameTabla("libretas").registros().size(),
                  cant_libretas + 1 + i);
    }
}

// ## Validar registro
TEST_F(DBAlumnos, validar_registro) {
    // Registro inválido por faltar campos
    EXPECT_FALSE(db.registroValido(Registro({"LU_N", "LU_A"},
                                            {tipoNat, tipoNat}),
                                   "libretas"));
    EXPECT_FALSE(db.registroValido(Registro({"LU", "LU_N"},
                                            {tipoNat, tipoNat}),
                                   "libretas"));
    EXPECT_FALSE(db.registroValido(Registro({"LU", "Nombre"},
                                            {tipoStr, tipoStr}),
                                   "alumnos"));
    EXPECT_FALSE(db.registroValido(Registro({"Editor", "Nombre"},
                                            {tipoStr, tipoStr}),
                                   "alumnos"));

    // Registro inválido por distintos tipos
    EXPECT_FALSE(db.registroValido(Registro({"LU_N", "LU_A", "LU"},
                                            {tipoNat, tipoStr, tipoStr}),
                                   "libretas"));
    EXPECT_FALSE(db.registroValido(Registro({"LU", "LU_A", "LU_N"},
                                            {tipoNat, tipoNat, tipoNat}),
                                   "libretas"));
    EXPECT_FALSE(db.registroValido(Registro({"LU", "Nombre", "Editor", "OS"},
                                            {tipoStr, tipoStr, tipoNat, tipoStr}),
                                   "alumnos"));

    // Registro invalido por claves
    EXPECT_FALSE(
            db.registroValido(Registro({"LU_N", "LU_A", "LU"},
                                       {datoNat(1), datoNat(90), datoStr("1/90")}),
                              "libretas"));
    EXPECT_FALSE(
            db.registroValido(Registro({"LU", "LU_N", "LU_A"},
                                       {datoStr("1/90"), datoNat(1), datoNat(90)}),
                              "libretas"));
    EXPECT_FALSE(db.registroValido(
            Registro({"LU", "Nombre", "Editor", "OS"},
                     {datoStr("1/90"), datoStr("March-Orga2"), datoStr("Nano"), datoStr("Win")}),
            "alumnos"));

    // Registro válido
    // Clave múltiple, 1 dif
    EXPECT_TRUE(db.registroValido(
            Registro({"LU_N", "LU_A", "LU"},
                     {datoNat(1), datoNat(90), datoStr("??")}),
            "libretas"));
    EXPECT_TRUE(db.registroValido(
            Registro({"LU", "Materia"},
                     {datoStr("1/90"), datoStr("Orga2")}),
            "materias"));
    // Clave múltiple, 1 dif, otro orden
    EXPECT_TRUE(
            db.registroValido(Registro({"LU_N", "LU", "LU_A"},
                                       {datoNat(1), datoStr("??"), datoNat(90)}),
                              "libretas"));
    EXPECT_TRUE(db.registroValido(Registro({"Materia", "LU"},
                                           {datoStr("Orga2"), datoStr("1/90")}),
                                  "materias"));
    // Clave simple
    EXPECT_TRUE(db.registroValido(
            Registro({"LU", "Nombre", "Editor", "OS"},
                     {datoStr("2/90"), datoStr("March"), datoStr("Vim"), datoStr("Linux")}),
            "alumnos"));
}

// ## Búsqueda
TEST_F(DBAlumnos, busqueda_base) {
    // Incluye búsqueda igual simple
    Tabla res = db.busqueda({Rig("LU", "1/90")}, "alumnos");
    EXPECT_EQ(res.registros().size(), 1);
    EXPECT_EQ(res.campos(), alumnos.campos());
    EXPECT_EQ(res.claves(), alumnos.claves());
    EXPECT_TRUE(incluye(alumnos.registros().begin(),
                        alumnos.registros().end(),
                        res.registros().begin(),
                        res.registros().end()));

    res = db.busqueda({Rig("LU_A", 90)}, "libretas");
    EXPECT_EQ(res.registros().size(), 2);
    EXPECT_EQ(res.campos(), libretas.campos());
    EXPECT_EQ(res.claves(), libretas.claves());
    EXPECT_TRUE(incluye(libretas.registros().begin(),
                        libretas.registros().end(),
                        res.registros().begin(),
                        res.registros().end()));

    res = db.busqueda({Rig("Materia", "AED2")}, "materias");
    EXPECT_EQ(res.registros().size(), 3);
    EXPECT_EQ(res.campos(), materias.campos());
    EXPECT_EQ(res.claves(), materias.claves());
    EXPECT_TRUE(incluye(materias.registros().begin(),
                        materias.registros().end(),
                        res.registros().begin(),
                        res.registros().end()));
}

TEST_F(DBAlumnos, busqueda_distinto_simple) {
    Tabla res = db.busqueda({Rdif("LU_A", 80)}, "libretas");
    EXPECT_EQ(res.registros().size(), 4);
    EXPECT_TRUE(incluye(libretas.registros().begin(),
                        libretas.registros().end(),
                        res.registros().begin(),
                        res.registros().end()));

    res = db.busqueda({Rdif("Editor", "Vim")}, "alumnos");
    EXPECT_EQ(res.registros().size(), 2);
    EXPECT_TRUE(incluye(alumnos.registros().begin(),
                        alumnos.registros().end(),
                        res.registros().begin(),
                        res.registros().end()));
}

TEST_F(DBAlumnos, busqueda_igual_doble) {
    Tabla res = db.busqueda({Rig("LU_N", 1), Rig("LU_A", 90)},
                            "libretas");
    EXPECT_EQ(res.registros().size(), 1);
    EXPECT_TRUE(incluye(libretas.registros().begin(),
                        libretas.registros().end(),
                        res.registros().begin(),
                        res.registros().end()));

    // Otro orden
    res = db.busqueda({Rig("LU_A", 90), Rig("LU_N", 1)},
                      "libretas");
    EXPECT_EQ(res.registros().size(), 1);
    EXPECT_TRUE(incluye(libretas.registros().begin(),
                        libretas.registros().end(),
                        res.registros().begin(),
                        res.registros().end()));

    res = db.busqueda({Rig("Editor", "Vim"), Rig("OS", "Linux")},
                      "alumnos");
    EXPECT_EQ(res.registros().size(), 2);
    EXPECT_TRUE(incluye(alumnos.registros().begin(),
                        alumnos.registros().end(),
                        res.registros().begin(),
                        res.registros().end()));

    res = db.busqueda({Rig("OS", "macOS"), Rig("Editor", "Vim")},
                      "alumnos");
    EXPECT_EQ(res.registros().size(), 3);
    EXPECT_TRUE(incluye(alumnos.registros().begin(),
                        alumnos.registros().end(),
                        res.registros().begin(),
                        res.registros().end()));
}

TEST_F(DBAlumnos, busqueda_distinto_doble) {
    Tabla res = db.busqueda({Rdif("LU_N", 1), Rdif("LU_A", 90)},
                            "libretas");
    EXPECT_EQ(res.registros().size(), 5);
    EXPECT_TRUE(incluye(libretas.registros().begin(),
                        libretas.registros().end(),
                        res.registros().begin(),
                        res.registros().end()));

    // Otro orden
    res = db.busqueda({Rdif("LU_A", 90), Rdif("LU_N", 1)},
                      "libretas");
    EXPECT_EQ(res.registros().size(), 5);
    EXPECT_TRUE(incluye(libretas.registros().begin(),
                        libretas.registros().end(),
                        res.registros().begin(),
                        res.registros().end()));

    res = db.busqueda({Rdif("Editor", "Vim"), Rdif("OS", "Linux")},
                      "alumnos");
    EXPECT_EQ(res.registros().size(), 2);
    EXPECT_TRUE(incluye(alumnos.registros().begin(),
                        alumnos.registros().end(),
                        res.registros().begin(),
                        res.registros().end()));

    res = db.busqueda({Rdif("OS", "Win"), Rdif("Editor", "Vim")},
                      "alumnos");
    EXPECT_EQ(res.registros().size(), 0);
    EXPECT_TRUE(incluye(alumnos.registros().begin(),
                        alumnos.registros().end(),
                        res.registros().begin(),
                        res.registros().end()));

    res = db.busqueda({Rdif("OS", "macOS"), Rdif("Editor", "Vim")},
                      "alumnos");
    EXPECT_EQ(res.registros().size(), 2);
    EXPECT_TRUE(incluye(alumnos.registros().begin(),
                        alumnos.registros().end(),
                        res.registros().begin(),
                        res.registros().end()));
}

TEST_F(DBAlumnos, busqueda_igual_distinto) {
    Tabla res = db.busqueda({Rig("Editor", "Vim"), Rdif("OS", "macOS")},
                            "alumnos");
    EXPECT_EQ(res.registros().size(), 2);
    EXPECT_TRUE(incluye(alumnos.registros().begin(),
                        alumnos.registros().end(),
                        res.registros().begin(),
                        res.registros().end()));

    res = db.busqueda({Rig("OS", "Linux"), Rdif("Nombre", "March")},
                      "alumnos");
    EXPECT_EQ(res.registros().size(), 1);
    EXPECT_TRUE(incluye(alumnos.registros().begin(),
                        alumnos.registros().end(),
                        res.registros().begin(),
                        res.registros().end()));

    res = db.busqueda({Rdif("OS", "Linux"), Rig("Nombre", "March")},
                      "alumnos");
    EXPECT_EQ(res.registros().size(), 0);
    EXPECT_TRUE(incluye(alumnos.registros().begin(),
                        alumnos.registros().end(),
                        res.registros().begin(),
                        res.registros().end()));

    res = db.busqueda({Rdif("OS", "macOS"), Rig("Editor", "Vim")},
                      "alumnos");
    EXPECT_EQ(res.registros().size(), 2);
    EXPECT_TRUE(incluye(alumnos.registros().begin(),
                        alumnos.registros().end(),
                        res.registros().begin(),
                        res.registros().end()));
}

TEST_F(DBAlumnos, busqueda_igual_distinto_doble) {
    Tabla res = db.busqueda({Rig("Editor", "Vim"), Rdif("OS", "macOS"),
                             Rig("Nombre", "March")},
                            "alumnos");
    EXPECT_EQ(res.registros().size(), 1);
    EXPECT_TRUE(incluye(alumnos.registros().begin(),
                        alumnos.registros().end(),
                        res.registros().begin(),
                        res.registros().end()));

    res = db.busqueda({Rig("OS", "Win"), Rdif("Nombre", "March"),
                       Rig("Editor", "CLion")},
                      "alumnos");
    EXPECT_EQ(res.registros().size(), 1);
    EXPECT_TRUE(incluye(alumnos.registros().begin(),
                        alumnos.registros().end(),
                        res.registros().begin(),
                        res.registros().end()));

    res = db.busqueda({Rdif("Nombre", "Crack"), Rig("OS", "macOS"),
                       Rig("Editor", "Vim")},
                      "alumnos");
    EXPECT_EQ(res.registros().size(), 3);
    EXPECT_TRUE(incluye(alumnos.registros().begin(),
                        alumnos.registros().end(),
                        res.registros().begin(),
                        res.registros().end()));
}

// ## Criterio Válido
TEST_F(DBAlumnos, crit_simple_nombre) {
    // ==
    EXPECT_FALSE(db.criterioValido({{Rig("LU_X", 1)}}, "libretas"));
    EXPECT_FALSE(db.criterioValido({{Rig("LU_X", "")}}, "libretas"));
    EXPECT_FALSE(db.criterioValido({{Rig("LU_X", 1)}}, "alumnos"));
    EXPECT_FALSE(db.criterioValido({{Rig("LU_X", "")}}, "alumnos"));
    EXPECT_TRUE(db.criterioValido({{Rig("LU_N", 5)}}, "libretas"));
    EXPECT_TRUE(db.criterioValido({{Rig("LU", "")}}, "alumnos"));

    // !=
    EXPECT_FALSE(db.criterioValido({{Rdif("LU_X", 1)}}, "libretas"));
    EXPECT_FALSE(db.criterioValido({{Rdif("LU_X", "")}}, "libretas"));
    EXPECT_FALSE(db.criterioValido({{Rdif("LU_X", 1)}}, "alumnos"));
    EXPECT_FALSE(db.criterioValido({{Rdif("LU_X", "")}}, "alumnos"));
    EXPECT_TRUE(db.criterioValido({{Rdif("LU_N", 5)}}, "libretas"));
    EXPECT_TRUE(db.criterioValido({{Rdif("LU", "")}}, "alumnos"));
}

TEST_F(DBAlumnos, crit_simple_tipo) {
    // ==
    EXPECT_FALSE(db.criterioValido({{Rig("LU_A", "")}}, "libretas"));
    EXPECT_FALSE(db.criterioValido({{Rig("LU", 1)}}, "alumnos"));
    EXPECT_TRUE(db.criterioValido({{Rig("LU_A", 5)}}, "libretas"));
    EXPECT_TRUE(db.criterioValido({{Rig("LU", "A")}}, "alumnos"));

    // !=
    EXPECT_FALSE(db.criterioValido({{Rdif("LU_A", "")}}, "libretas"));
    EXPECT_FALSE(db.criterioValido({{Rdif("LU", 1)}}, "alumnos"));
    EXPECT_TRUE(db.criterioValido({{Rdif("LU_A", 5)}}, "libretas"));
    EXPECT_TRUE(db.criterioValido({{Rdif("LU", "A")}}, "alumnos"));
}

TEST_F(DBAlumnos, crit_doble_nombre) {
    // ==
    EXPECT_FALSE(db.criterioValido({{Rig("LU_A", 5), Rig("LU_X", 5)}},
                                   "libretas"));
    EXPECT_TRUE(db.criterioValido({{Rig("LU_A", 5), Rig("LU_N", 5)}},
                                  "libretas"));
    EXPECT_FALSE(db.criterioValido({{Rig("LU", ""), Rig("os", "")}},
                                   "alumnos"));
    EXPECT_TRUE(db.criterioValido({{Rig("LU", ""), Rig("OS", "")}},
                                  "alumnos"));

    // !=
    EXPECT_FALSE(db.criterioValido({{Rdif("LU_A", 5), Rig("LU_X", 5)}},
                                   "libretas"));
    EXPECT_TRUE(db.criterioValido({{Rdif("LU_A", 5), Rig("LU_N", 5)}},
                                  "libretas"));
    EXPECT_FALSE(db.criterioValido({{Rdif("LU", ""), Rig("os", "")}},
                                   "alumnos"));
    EXPECT_TRUE(db.criterioValido({{Rdif("LU", ""), Rig("OS", "")}},
                                  "alumnos"));
}

TEST_F(DBAlumnos, crit_doble_tipo) {
    // ==
    EXPECT_FALSE(db.criterioValido({{Rig("LU_A", 5), Rig("LU_N", "")}},
                                   "libretas"));
    EXPECT_TRUE(db.criterioValido({{Rig("LU_A", 5), Rig("LU_N", 5)}},
                                  "libretas"));
    EXPECT_FALSE(db.criterioValido({{Rig("LU", 4), Rig("OS", "")}},
                                   "alumnos"));
    EXPECT_TRUE(db.criterioValido({{Rig("LU", ""), Rig("OS", "")}},
                                  "alumnos"));

    // !=
    EXPECT_FALSE(db.criterioValido({{Rdif("LU_A", 5), Rig("LU_N", "")}},
                                   "libretas"));
    EXPECT_TRUE(db.criterioValido({{Rdif("LU_A", 5), Rig("LU_N", 5)}},
                                  "libretas"));
    EXPECT_FALSE(db.criterioValido({{Rdif("LU", 4), Rig("OS", "")}},
                                   "alumnos"));
    EXPECT_TRUE(db.criterioValido({{Rdif("LU", ""), Rig("OS", "")}},
                                  "alumnos"));
}

// ## Uso Criterio
TEST_F(DBAlumnos, uso_un_criterio) {
    db.busqueda({Rig("OS", "A")}, "alumnos");
    EXPECT_EQ(db.uso_criterio({Rig("OS", "A")}), 1);
    EXPECT_EQ(db.top_criterios(),
              linear_set<BaseDeDatos::Criterio>({{Rig("OS", "A")}}));
    db.busqueda({Rig("OS", "A")}, "alumnos");
    EXPECT_EQ(db.uso_criterio({Rig("OS", "A")}), 2);
    EXPECT_EQ(db.top_criterios(),
              linear_set<BaseDeDatos::Criterio>({{Rig("OS", "A")}}));

    db.busqueda({Rig("LU_A", 1)}, "libretas");
    EXPECT_EQ(db.uso_criterio({Rig("LU_A", 1)}), 1);
    EXPECT_EQ(db.top_criterios(),
              linear_set<BaseDeDatos::Criterio>({{Rig("OS", "A")}}));

    db.busqueda({Rig("LU_A", 1)}, "libretas");
    EXPECT_EQ(db.uso_criterio({Rig("LU_A", 1)}), 2);
    EXPECT_EQ(db.top_criterios(),
              linear_set<BaseDeDatos::Criterio>({{Rig("OS", "A")},
                                                 {Rig("LU_A", 1)}}));
    db.busqueda({Rig("LU_A", 1)}, "libretas");
    EXPECT_EQ(db.uso_criterio({Rig("LU_A", 1)}), 3);
    EXPECT_EQ(db.top_criterios(),
              linear_set<BaseDeDatos::Criterio>({{Rig("LU_A", 1)}}));
}

TEST_F(DBAlumnos, uso_un_criterio_perm) {
    BaseDeDatos::Criterio c = {Rig("OS", "A"), Rig("Editor", "Vim")};
    BaseDeDatos::Criterio c_perm = {Rig("Editor", "Vim"), Rig("OS", "A")};
    BaseDeDatos::Criterio c_sim = {Rig("OS", "A")};
    db.busqueda(c, "alumnos");
    EXPECT_EQ(db.uso_criterio(c), 1);
    EXPECT_EQ(db.uso_criterio(c_sim), 0);
    EXPECT_EQ(db.top_criterios(),
              linear_set<BaseDeDatos::Criterio>({c}));

    db.busqueda(c_perm, "alumnos");
    EXPECT_EQ(db.uso_criterio(c), 2);
    EXPECT_EQ(db.uso_criterio(c_sim), 0);
    EXPECT_EQ(db.top_criterios(),
              linear_set<BaseDeDatos::Criterio>({c}));

    db.busqueda(c_sim, "alumnos");
    EXPECT_EQ(db.uso_criterio(c_sim), 1);
    EXPECT_EQ(db.top_criterios(),
              linear_set<BaseDeDatos::Criterio>({c}));
}

TEST_F(DBAlumnos, crit_otro_bool) {
    BaseDeDatos::Criterio c = {Rig("OS", "A")};
    BaseDeDatos::Criterio c_inv = {Rdif("OS", "A")};

    EXPECT_EQ(db.uso_criterio(c), 0);
    EXPECT_EQ(db.uso_criterio(c_inv), 0);

    db.busqueda(c, "alumnos");
    EXPECT_EQ(db.uso_criterio(c), 1);
    EXPECT_EQ(db.uso_criterio(c_inv), 0);

    db.busqueda(c_inv, "alumnos");
    EXPECT_EQ(db.uso_criterio(c), 1);
    EXPECT_EQ(db.uso_criterio(c_inv), 1);
}

TEST_F(DBAlumnos, crit_doble_otro_bool) {
    BaseDeDatos::Criterio c = {Rig("OS", "A"), Rig("Editor", "Vim")};
    BaseDeDatos::Criterio c_inv = {Rdif("OS", "A"), Rig("Editor", "Vim")};

    EXPECT_EQ(db.uso_criterio(c), 0);
    EXPECT_EQ(db.uso_criterio(c_inv), 0);

    db.busqueda(c, "alumnos");
    EXPECT_EQ(db.uso_criterio(c), 1);
    EXPECT_EQ(db.uso_criterio(c_inv), 0);

    db.busqueda(c_inv, "alumnos");
    EXPECT_EQ(db.uso_criterio(c), 1);
    EXPECT_EQ(db.uso_criterio(c_inv), 1);
}

// ## Join
// * Join vacío
// * Join sin repetidos
// * Join repetidos un lado
// * Join repetidos dos lados
// * Join campos repetidos

#ifdef POST_SOLUCION

TEST_F(DBAlumnos, join_vacio) {
    db.crearIndice("alumnos", "LU");
    auto begin = db.join("alumnos", "ex_alumnos", "LU");
    auto end = db.join_end();
    EXPECT_EQ(begin, end);
}

TEST_F(DBAlumnos, join_sin_repetidos) {
    db.crearIndice("alumnos", "LU");
    auto begin = db.join("libretas", "alumnos", "LU");
    auto end = db.join_end();
    linear_set<string> nuevos_campos({"LU_N", "LU_A", "LU", "Nombre", "Editor", "OS"});

    int count = 0;
    for (auto it = begin; it != end; it++) {
        EXPECT_EQ((*it).campos(), nuevos_campos);
        count++;
    }

    EXPECT_EQ(count, db.dameTabla("libretas").registros().size());
    begin = db.join("libretas", "alumnos", "LU");
    linear_set<Registro> join(begin, end);
    EXPECT_EQ(join, join_libretas_alumnos.registros());
}

TEST_F(DBAlumnos, join_repetidos_uno) {
    db.crearIndice("libretas", "LU");
    auto begin = db.join("libretas", "materias", "LU");
    auto end = db.join_end();

    linear_set<string> nuevos_campos({"LU_N", "LU_A", "LU", "Materia"});
    for (auto it = begin; it != end; it++) {
        EXPECT_EQ((*it).campos(), nuevos_campos);
    }

    linear_set<Registro> join(begin, end);
    EXPECT_EQ(join, join_libretas_materias.registros());
}

TEST_F(DBAlumnos, join_repetidos_ambos) {
    BaseDeDatos db2;
    db2.crearTabla("T1", {"X", "Y"}, {"X", "Y"}, {tipoNat, tipoNat});
    db2.crearTabla("T2", {"Y", "Z"}, {"Y", "Z"}, {tipoNat, tipoStr});
    /*
     * T1           T2
     * | X | Y |    | Y | Z |
     * | 1 | 1 |    | 1 | A |
     * | 2 | 2 |    | 1 | B |
     * | 3 | 2 |    | 2 | C |
     * | 4 | 0 |
     *
     * T1 ~ T2
     * | X | Y | Z |
     * | 1 | 1 | A |
     * | 1 | 1 | B |
     * | 2 | 2 | C |
     * | 3 | 2 | C |
     */
    db2.agregarRegistro(Registro({"X", "Y"}, {Datum(1), Datum(1)}), "T1");
    db2.agregarRegistro(Registro({"X", "Y"}, {Datum(2), Datum(2)}), "T1");
    db2.agregarRegistro(Registro({"X", "Y"}, {Datum(3), Datum(2)}), "T1");
    db2.agregarRegistro(Registro({"X", "Y"}, {Datum(4), Datum(0)}), "T1");
    db2.agregarRegistro(Registro({"Y", "Z"}, {Datum(1), Datum("A")}), "T2");
    db2.agregarRegistro(Registro({"Y", "Z"}, {Datum(1), Datum("B")}), "T2");
    db2.agregarRegistro(Registro({"Y", "Z"}, {Datum(2), Datum("C")}), "T2");

    Tabla t_join = Tabla({"X", "Y", "Z"}, {"X", "Y", "Z"},
                         {tipoNat, tipoNat, tipoStr});
    t_join.agregarRegistro(Registro({"X", "Y", "Z"},
                                    {Datum(1), Datum(1), Datum("A")}));
    t_join.agregarRegistro(Registro({"X", "Y", "Z"},
                                    {Datum(1), Datum(1), Datum("B")}));
    t_join.agregarRegistro(Registro({"X", "Y", "Z"},
                                    {Datum(2), Datum(2), Datum("C")}));
    t_join.agregarRegistro(Registro({"X", "Y", "Z"},
                                    {Datum(3), Datum(2), Datum("C")}));

    db2.crearIndice("T1", "Y");
    db2.crearIndice("T2", "Y");
    auto begin = db2.join("T1", "T2", "Y");
    auto end = db2.join_end();

    linear_set<string> nuevos_campos({"X", "Y", "Z"});
    for (auto it = begin; it != end; it++) {
        EXPECT_EQ((*it).campos(), nuevos_campos);
    }

    linear_set<Registro> join(begin, end);
    EXPECT_EQ(join, t_join.registros());
}

TEST_F(DBAlumnos, join_campos_repetidos) {
    BaseDeDatos db2;
    db2.crearTabla("T1", {"X", "Y"}, {"X", "Y"}, {tipoNat, tipoNat});
    db2.crearTabla("T2", {"X", "Y", "Z"}, {"X", "Y", "Z"}, {tipoNat, tipoNat, tipoStr});
    /*
     * T1          T2
     * | X | Y |  | X | Y | Z |
     * | 1 | 1 |  | 1 | 1 | A |
     * | 2 | 2 |  | 3 | 2 | C |
     *
     * T1 ~ T2 (Y)
     * | X | Y | Z |
     * | 1 | 1 | A |
     * | 2 | 2 | C |
     *
     * T2 ~ T1 (Y)
     * | X | Y | Z |
     * | 1 | 1 | A |
     * | 3 | 2 | C |
     *
     */
    db2.agregarRegistro(Registro({"X", "Y"}, {Datum(1), Datum(1)}), "T1");
    db2.agregarRegistro(Registro({"X", "Y"}, {Datum(2), Datum(2)}), "T1");
    db2.agregarRegistro(Registro({"X", "Y", "Z"}, {Datum(1), Datum(1), Datum("A")}), "T2");
    db2.agregarRegistro(Registro({"X", "Y", "Z"}, {Datum(3), Datum(2), Datum("C")}), "T2");

    Tabla t_join_a = Tabla({"X", "Y", "Z"}, {"X", "Y", "Z"}, {tipoNat, tipoNat, tipoStr});
    t_join_a.agregarRegistro(Registro({"X", "Y", "Z"}, {Datum(1), Datum(1), Datum("A")}));
    t_join_a.agregarRegistro(Registro({"X", "Y", "Z"}, {Datum(2), Datum(2), Datum("C")}));

    Tabla t_join_b = Tabla({"X", "Y", "Z"}, {"X", "Y", "Z"}, {tipoNat, tipoNat, tipoStr});
    t_join_b.agregarRegistro(Registro({"X", "Y", "Z"}, {Datum(1), Datum(1), Datum("A")}));
    t_join_b.agregarRegistro(Registro({"X", "Y", "Z"}, {Datum(3), Datum(2), Datum("C")}));

    db2.crearIndice("T1", "X");
    db2.crearIndice("T2", "Z");
    db2.crearIndice("T2", "Y");
    auto begin = db2.join("T1", "T2", "Y");
    auto end = db2.join_end();

    linear_set<Registro> join(begin, end);
    EXPECT_EQ(join, t_join_a.registros());

    begin = db2.join("T2", "T1", "Y");
    end = db2.join_end();

    linear_set<Registro> join_b(begin, end);
    EXPECT_EQ(join_b, t_join_b.registros());
}

#endif // POST_SOLUCION
