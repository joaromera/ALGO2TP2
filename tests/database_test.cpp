#include "../src/Database.h"
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
  iter end_b)
{
  for (auto it = begin_b; it != end_b; ++it)
  {
    auto it2 = begin_a;
    for (; it2 != end_a; ++it2)
    {
      if (*it2 == *it)
      {
        break;
      }
    }
    if (it2 == end_a)
    {
      return false;
    }
  }
  return true;
}

TEST(test_basicos, constructor)
{
  Database db;

  EXPECT_TRUE(db.tablas().empty());
  EXPECT_FALSE(db.uso_criterio(Database::Criterio()));
}

TEST(test_basicos, Table_alumnos)
{
  Database db;

  vector<string> campos_alumnos({ "LU", "Ano", "Nombre", "DNI" });
  vector<Datum> tipos_alumnos = { Datum{ 0 }, Datum{ 0 }, Datum{ "" }, Datum{ "" } };
  linear_set<string> claves_alumnos = { "LU", "Ano" };

  db.crearTabla("alumnos", claves_alumnos, campos_alumnos, tipos_alumnos);

  EXPECT_EQ(db.dameTabla("alumnos"),
    Table(claves_alumnos, campos_alumnos, tipos_alumnos));

  Record gerva =
    Record(campos_alumnos,
      { Datum(123), Datum(45), Datum("Gerva"), Datum("10001") });

  EXPECT_TRUE(db.registroValido(gerva, "alumnos"));
  db.agregarRegistro(gerva, "alumnos");
  EXPECT_FALSE(db.registroValido(gerva, "alumnos"));

  Record gerva2 =
    Record(campos_alumnos,
      { Datum(124), Datum(45), Datum("Gerva"), Datum("10001") });
  EXPECT_TRUE(db.registroValido(gerva2, "alumnos"));
  db.agregarRegistro(gerva2, "alumnos");
  EXPECT_FALSE(db.registroValido(gerva2, "alumnos"));

  Record gerva_clon =
    Record(campos_alumnos,
      { Datum(124), Datum(45), Datum("Gerva"), Datum("10001") });
  EXPECT_FALSE(db.registroValido(gerva_clon, "alumnos"));
}

class DBAlumnos : public ::testing::Test
{
protected:
  typedef linear_set<string> Claves;
  typedef vector<string> Campos;
  typedef vector<Datum> Tipos;

  struct DefinicionTable
  {
    DefinicionTable(Claves _claves, Campos _campos, Tipos _tipos)
      : claves(_claves), campos(_campos), tipos(_tipos) {}

    Claves claves;
    Campos campos;
    Tipos tipos;
  };


  Database db;
  DefinicionTable def_libretas;
  DefinicionTable def_alumnos;
  DefinicionTable def_materias;
  DefinicionTable def_exalumnos;
  DefinicionTable def_join_libretas_alumnos;
  DefinicionTable def_join_libretas_materias;
  Table libretas;
  Table alumnos;
  Table materias;
  Table ex_alumnos;
  Table join_libretas_alumnos;
  Table join_libretas_materias;

  Table def_a_Table(const DefinicionTable &def)
  {
    return Table(def.claves, def.campos, def.tipos);
  }

  void definir_Table(const string &nombre, const DefinicionTable &def)
  {
    db.crearTabla(nombre, def.claves, def.campos, def.tipos);
  }

  void iniciarTables()
  {
    definir_Table("alumnos", def_alumnos);
    definir_Table("libretas", def_libretas);
    definir_Table("materias", def_materias);
    definir_Table("ex_alumnos", def_exalumnos);
  }

  void cargarAlumno(const int &lu_n, const int &lu_a, const string &nombre, const string &editor, const string &os, const vector<string> &mts)
  {
    string lu = to_string(lu_n) + '/' + to_string(lu_a);
    Record r_libretas = Record(
      { "LU_N", "LU_A", "LU" }, { Datum(lu_n), Datum(lu_a), Datum(lu) });
    libretas.addRecord(r_libretas);
    db.agregarRegistro(r_libretas, "libretas");

    Record r_alumno = Record(
      { "LU", "Nombre", "Editor", "OS" },
      { Datum(lu), Datum(nombre), Datum(editor), Datum(os) });
    alumnos.addRecord(r_alumno);
    db.agregarRegistro(r_alumno, "alumnos");

    Record r_join_libretas_alumnos = Record(
      { "LU_N", "LU_A", "LU", "Nombre", "Editor", "OS" },
      { Datum(lu_n), Datum(lu_a), Datum(lu), Datum(nombre), Datum(editor), Datum(os) });
    join_libretas_alumnos.addRecord(r_join_libretas_alumnos);

    for (auto mat : mts)
    {
      Record r_materia = Record(
        { "LU", "Materia" }, { Datum(lu), Datum(mat) });
      materias.addRecord(r_materia);
      db.agregarRegistro(r_materia, "materias");

      Record r_join_libretas_materias = Record(
        { "LU_N", "LU_A", "LU", "Materia" },
        { Datum(lu_n), Datum(lu_a), Datum(lu), Datum(mat) });
      join_libretas_materias.addRecord(r_join_libretas_materias);
    }
  }

  Record registro(const DefinicionTable &def,
    const vector<Datum> &valores)
  {
    return Record(def.campos, valores);
  }

  DBAlumnos() : def_libretas({ "LU_N", "LU_A", "LU" },
    { "LU_N", "LU_A", "LU" },
    { Datum{ 0 }, Datum{ 0 }, Datum{ "" } }),
                def_alumnos({ "LU" },
                  { "LU", "Nombre", "Editor", "OS" },
                  { Datum{ "" }, Datum{ "" }, Datum{ "" }, Datum{ "" } }),
                def_materias({ "LU", "Materia" },
                  { "LU", "Materia" },
                  { Datum{ "" }, Datum{ "" } }),
                def_exalumnos({ "LU", "Egreso" },
                  { "LU" },
                  { Datum{ "" }, Datum{ 0 } }),
                def_join_libretas_alumnos({ "LU_N", "LU_A", "LU", "Nombre", "Editor", "OS" },
                  { "LU" },
                  { Datum{ 0 }, Datum{ 0 }, Datum{ "" }, Datum{ "" }, Datum{ "" }, Datum{ "" }, Datum{ "" } }),
                def_join_libretas_materias({ "LU_N", "LU_A", "LU", "Materia" },
                  { "LU" },
                  { Datum{ 0 }, Datum{ 0 }, Datum{ "" }, Datum{ "" } }),
                libretas(def_a_Table(def_libretas)),
                alumnos(def_a_Table(def_alumnos)),
                materias(def_a_Table(def_materias)),
                ex_alumnos(def_a_Table(def_exalumnos)),
                join_libretas_alumnos(def_a_Table(def_join_libretas_alumnos)),
                join_libretas_materias(def_a_Table(def_join_libretas_materias))
  {
    iniciarTables();
    cargarAlumno(1, 90, "March", "Vim", "Linux", { "AED2" });
    cargarAlumno(2, 80, "Old-school", "Vim", "Linux", { "OOP", "AED2" });
    cargarAlumno(3, 90, "GenY", "Code-blocks", "Win", { "AED1" });
    cargarAlumno(4, 1, "Crack", "CLion", "Win", { "AED2", "AED1" });
    cargarAlumno(5, 80, "Tron", "Vim", "macOS", { "Orga2" });
    cargarAlumno(6, 80, "CLU", "Vim", "macOS", { "Orga1" });
    cargarAlumno(5, 2, "Hipster", "Vim", "macOS", { "AED3" });

    db.agregarRegistro(Record({ "LU", "Egreso" },
                         { Datum("100/70"), Datum(75) }),
      "ex_alumnos");
    db.agregarRegistro(Record({ "LU", "Egreso" },
                         { Datum("102/70"), Datum(75) }),
      "ex_alumnos");
  }
};

TEST_F(DBAlumnos, init)
{
  EXPECT_EQ(libretas, db.dameTabla("libretas"));
  EXPECT_EQ(alumnos, db.dameTabla("alumnos"));
  EXPECT_EQ(materias, db.dameTabla("materias"));
}

// ## Agregar registro
TEST_F(DBAlumnos, agregar_registro)
{
  Record alu1 = registro(def_libretas,
    { Datum(10), Datum(10), Datum("10/10") });
  Record alu2 = registro(def_libretas,
    { Datum(11), Datum(10), Datum("11/10") });
  Record alu3 = registro(def_libretas,
    { Datum(12), Datum(10), Datum("12/10") });

  int cant_libretas = db.dameTabla("libretas").records().size();

  vector<Record> regs = { alu1, alu2, alu3 };

  for (size_t i = 0; i < regs.size(); i++)
  {// Voy agregando de a uno
    EXPECT_EQ(db.dameTabla("libretas").records().size(), cant_libretas + i);
    for (size_t j = i; j < regs.size(); j++)
    {// Antes de agregar faltan el resto
      EXPECT_FALSE(db.dameTabla("libretas").records().count(regs[j]));
    }
    db.agregarRegistro(regs[i], "libretas");
    // Ahora no falta el agregado
    EXPECT_TRUE(db.dameTabla("libretas").records().count(regs[i]));
    // Pero siguen faltando los otros
    for (size_t j = i + 1; j < regs.size(); j++)
    {
      EXPECT_FALSE(db.dameTabla("libretas").records().count(regs[j]));
    }
    // Se actualizó la cantidad de records
    EXPECT_EQ(db.dameTabla("libretas").records().size(),
      cant_libretas + 1 + i);
  }
}

// ## Validar registro
TEST_F(DBAlumnos, validar_registro)
{
  // Record inválido por faltar columns
  EXPECT_FALSE(db.registroValido(Record({ "LU_N", "LU_A" },
                                   { Datum{ 0 }, Datum{ 0 } }),
    "libretas"));
  EXPECT_FALSE(db.registroValido(Record({ "LU", "LU_N" },
                                   { Datum{ 0 }, Datum{ 0 } }),
    "libretas"));
  EXPECT_FALSE(db.registroValido(Record({ "LU", "Nombre" },
                                   { Datum{ "" }, Datum{ "" } }),
    "alumnos"));
  EXPECT_FALSE(db.registroValido(Record({ "Editor", "Nombre" },
                                   { Datum{ "" }, Datum{ "" } }),
    "alumnos"));

  // Record inválido por distintos tipos
  EXPECT_FALSE(db.registroValido(Record({ "LU_N", "LU_A", "LU" },
                                   { Datum{ 0 }, Datum{ "" }, Datum{ "" } }),
    "libretas"));
  EXPECT_FALSE(db.registroValido(Record({ "LU", "LU_A", "LU_N" },
                                   { Datum{ 0 }, Datum{ 0 }, Datum{ 0 } }),
    "libretas"));
  EXPECT_FALSE(db.registroValido(Record({ "LU", "Nombre", "Editor", "OS" },
                                   { Datum{ "" }, Datum{ "" }, Datum{ 0 }, Datum{ "" } }),
    "alumnos"));

  // Record invalido por keys
  EXPECT_FALSE(
    db.registroValido(Record({ "LU_N", "LU_A", "LU" },
                        { Datum(1), Datum(90), Datum("1/90") }),
      "libretas"));
  EXPECT_FALSE(
    db.registroValido(Record({ "LU", "LU_N", "LU_A" },
                        { Datum("1/90"), Datum(1), Datum(90) }),
      "libretas"));
  EXPECT_FALSE(db.registroValido(
    Record({ "LU", "Nombre", "Editor", "OS" },
      { Datum("1/90"), Datum("March-Orga2"), Datum("Nano"), Datum("Win") }),
    "alumnos"));

  // Record válido
  // Clave múltiple, 1 dif
  EXPECT_TRUE(db.registroValido(
    Record({ "LU_N", "LU_A", "LU" },
      { Datum(1), Datum(90), Datum("??") }),
    "libretas"));
  EXPECT_TRUE(db.registroValido(
    Record({ "LU", "Materia" },
      { Datum("1/90"), Datum("Orga2") }),
    "materias"));
  // Clave múltiple, 1 dif, otro orden
  EXPECT_TRUE(
    db.registroValido(Record({ "LU_N", "LU", "LU_A" },
                        { Datum(1), Datum("??"), Datum(90) }),
      "libretas"));
  EXPECT_TRUE(db.registroValido(Record({ "Materia", "LU" },
                                  { Datum("Orga2"), Datum("1/90") }),
    "materias"));
  // Clave simple
  EXPECT_TRUE(db.registroValido(
    Record({ "LU", "Nombre", "Editor", "OS" },
      { Datum("2/90"), Datum("March"), Datum("Vim"), Datum("Linux") }),
    "alumnos"));
}

// ## Búsqueda
TEST_F(DBAlumnos, busqueda_base)
{
  // Incluye búsqueda equals simple
  Table res = db.busqueda({ Rig("LU", "1/90") }, "alumnos");
  EXPECT_EQ(res.records().size(), (size_t) 1);
  EXPECT_EQ(res.columns(), alumnos.columns());
  EXPECT_EQ(res.keys(), alumnos.keys());
  EXPECT_TRUE(incluye(alumnos.records().begin(),
    alumnos.records().end(),
    res.records().begin(),
    res.records().end()));

  res = db.busqueda({ Rig("LU_A", 90) }, "libretas");
  EXPECT_EQ(res.records().size(), (size_t) 2);
  EXPECT_EQ(res.columns(), libretas.columns());
  EXPECT_EQ(res.keys(), libretas.keys());
  EXPECT_TRUE(incluye(libretas.records().begin(),
    libretas.records().end(),
    res.records().begin(),
    res.records().end()));

  res = db.busqueda({ Rig("Materia", "AED2") }, "materias");
  EXPECT_EQ(res.records().size(), (size_t) 3);
  EXPECT_EQ(res.columns(), materias.columns());
  EXPECT_EQ(res.keys(), materias.keys());
  EXPECT_TRUE(incluye(materias.records().begin(),
    materias.records().end(),
    res.records().begin(),
    res.records().end()));
}

TEST_F(DBAlumnos, busqueda_distinto_simple)
{
  Table res = db.busqueda({ Rdif("LU_A", 80) }, "libretas");
  EXPECT_EQ(res.records().size(), (size_t) 4);
  EXPECT_TRUE(incluye(libretas.records().begin(),
    libretas.records().end(),
    res.records().begin(),
    res.records().end()));

  res = db.busqueda({ Rdif("Editor", "Vim") }, "alumnos");
  EXPECT_EQ(res.records().size(), (size_t) 2);
  EXPECT_TRUE(incluye(alumnos.records().begin(),
    alumnos.records().end(),
    res.records().begin(),
    res.records().end()));
}

TEST_F(DBAlumnos, busqueda_igual_doble)
{
  Table res = db.busqueda({ Rig("LU_N", 1), Rig("LU_A", 90) },
    "libretas");
  EXPECT_EQ(res.records().size(), (size_t) 1);
  EXPECT_TRUE(incluye(libretas.records().begin(),
    libretas.records().end(),
    res.records().begin(),
    res.records().end()));

  // Otro orden
  res = db.busqueda({ Rig("LU_A", 90), Rig("LU_N", 1) },
    "libretas");
  EXPECT_EQ(res.records().size(), (size_t) 1);
  EXPECT_TRUE(incluye(libretas.records().begin(),
    libretas.records().end(),
    res.records().begin(),
    res.records().end()));

  res = db.busqueda({ Rig("Editor", "Vim"), Rig("OS", "Linux") },
    "alumnos");
  EXPECT_EQ(res.records().size(), (size_t) 2);
  EXPECT_TRUE(incluye(alumnos.records().begin(),
    alumnos.records().end(),
    res.records().begin(),
    res.records().end()));

  res = db.busqueda({ Rig("OS", "macOS"), Rig("Editor", "Vim") },
    "alumnos");
  EXPECT_EQ(res.records().size(), (size_t) 3);
  EXPECT_TRUE(incluye(alumnos.records().begin(),
    alumnos.records().end(),
    res.records().begin(),
    res.records().end()));
}

TEST_F(DBAlumnos, busqueda_distinto_doble)
{
  Table res = db.busqueda({ Rdif("LU_N", 1), Rdif("LU_A", 90) },
    "libretas");
  EXPECT_EQ(res.records().size(), (size_t) 5);
  EXPECT_TRUE(incluye(libretas.records().begin(),
    libretas.records().end(),
    res.records().begin(),
    res.records().end()));

  // Otro orden
  res = db.busqueda({ Rdif("LU_A", 90), Rdif("LU_N", 1) },
    "libretas");
  EXPECT_EQ(res.records().size(), (size_t) 5);
  EXPECT_TRUE(incluye(libretas.records().begin(),
    libretas.records().end(),
    res.records().begin(),
    res.records().end()));

  res = db.busqueda({ Rdif("Editor", "Vim"), Rdif("OS", "Linux") },
    "alumnos");
  EXPECT_EQ(res.records().size(), (size_t) 2);
  EXPECT_TRUE(incluye(alumnos.records().begin(),
    alumnos.records().end(),
    res.records().begin(),
    res.records().end()));

  res = db.busqueda({ Rdif("OS", "Win"), Rdif("Editor", "Vim") },
    "alumnos");
  EXPECT_EQ(res.records().size(), (size_t) 0);
  EXPECT_TRUE(incluye(alumnos.records().begin(),
    alumnos.records().end(),
    res.records().begin(),
    res.records().end()));

  res = db.busqueda({ Rdif("OS", "macOS"), Rdif("Editor", "Vim") },
    "alumnos");
  EXPECT_EQ(res.records().size(), (size_t) 2);
  EXPECT_TRUE(incluye(alumnos.records().begin(),
    alumnos.records().end(),
    res.records().begin(),
    res.records().end()));
}

TEST_F(DBAlumnos, busqueda_igual_distinto)
{
  Table res = db.busqueda({ Rig("Editor", "Vim"), Rdif("OS", "macOS") },
    "alumnos");
  EXPECT_EQ(res.records().size(), (size_t) 2);
  EXPECT_TRUE(incluye(alumnos.records().begin(),
    alumnos.records().end(),
    res.records().begin(),
    res.records().end()));

  res = db.busqueda({ Rig("OS", "Linux"), Rdif("Nombre", "March") },
    "alumnos");
  EXPECT_EQ(res.records().size(), (size_t) 1);
  EXPECT_TRUE(incluye(alumnos.records().begin(),
    alumnos.records().end(),
    res.records().begin(),
    res.records().end()));

  res = db.busqueda({ Rdif("OS", "Linux"), Rig("Nombre", "March") },
    "alumnos");
  EXPECT_EQ(res.records().size(), (size_t) 0);
  EXPECT_TRUE(incluye(alumnos.records().begin(),
    alumnos.records().end(),
    res.records().begin(),
    res.records().end()));

  res = db.busqueda({ Rdif("OS", "macOS"), Rig("Editor", "Vim") },
    "alumnos");
  EXPECT_EQ(res.records().size(), (size_t) 2);
  EXPECT_TRUE(incluye(alumnos.records().begin(),
    alumnos.records().end(),
    res.records().begin(),
    res.records().end()));
}

TEST_F(DBAlumnos, busqueda_igual_distinto_doble)
{
  Table res = db.busqueda({ Rig("Editor", "Vim"), Rdif("OS", "macOS"), Rig("Nombre", "March") },
    "alumnos");
  EXPECT_EQ(res.records().size(), (size_t) 1);
  EXPECT_TRUE(incluye(alumnos.records().begin(),
    alumnos.records().end(),
    res.records().begin(),
    res.records().end()));

  res = db.busqueda({ Rig("OS", "Win"), Rdif("Nombre", "March"), Rig("Editor", "CLion") },
    "alumnos");
  EXPECT_EQ(res.records().size(), (size_t) 1);
  EXPECT_TRUE(incluye(alumnos.records().begin(),
    alumnos.records().end(),
    res.records().begin(),
    res.records().end()));

  res = db.busqueda({ Rdif("Nombre", "Crack"), Rig("OS", "macOS"), Rig("Editor", "Vim") },
    "alumnos");
  EXPECT_EQ(res.records().size(), (size_t) 3);
  EXPECT_TRUE(incluye(alumnos.records().begin(),
    alumnos.records().end(),
    res.records().begin(),
    res.records().end()));
}

// ## Criterio Válido
TEST_F(DBAlumnos, crit_simple_nombre)
{
  // ==
  EXPECT_FALSE(db.criterioValido({ { Rig("LU_X", 1) } }, "libretas"));
  EXPECT_FALSE(db.criterioValido({ { Rig("LU_X", "") } }, "libretas"));
  EXPECT_FALSE(db.criterioValido({ { Rig("LU_X", 1) } }, "alumnos"));
  EXPECT_FALSE(db.criterioValido({ { Rig("LU_X", "") } }, "alumnos"));
  EXPECT_TRUE(db.criterioValido({ { Rig("LU_N", 5) } }, "libretas"));
  EXPECT_TRUE(db.criterioValido({ { Rig("LU", "") } }, "alumnos"));

  // !=
  EXPECT_FALSE(db.criterioValido({ { Rdif("LU_X", 1) } }, "libretas"));
  EXPECT_FALSE(db.criterioValido({ { Rdif("LU_X", "") } }, "libretas"));
  EXPECT_FALSE(db.criterioValido({ { Rdif("LU_X", 1) } }, "alumnos"));
  EXPECT_FALSE(db.criterioValido({ { Rdif("LU_X", "") } }, "alumnos"));
  EXPECT_TRUE(db.criterioValido({ { Rdif("LU_N", 5) } }, "libretas"));
  EXPECT_TRUE(db.criterioValido({ { Rdif("LU", "") } }, "alumnos"));
}

TEST_F(DBAlumnos, crit_simple_tipo)
{
  // ==
  EXPECT_FALSE(db.criterioValido({ { Rig("LU_A", "") } }, "libretas"));
  EXPECT_FALSE(db.criterioValido({ { Rig("LU", 1) } }, "alumnos"));
  EXPECT_TRUE(db.criterioValido({ { Rig("LU_A", 5) } }, "libretas"));
  EXPECT_TRUE(db.criterioValido({ { Rig("LU", "A") } }, "alumnos"));

  // !=
  EXPECT_FALSE(db.criterioValido({ { Rdif("LU_A", "") } }, "libretas"));
  EXPECT_FALSE(db.criterioValido({ { Rdif("LU", 1) } }, "alumnos"));
  EXPECT_TRUE(db.criterioValido({ { Rdif("LU_A", 5) } }, "libretas"));
  EXPECT_TRUE(db.criterioValido({ { Rdif("LU", "A") } }, "alumnos"));
}

TEST_F(DBAlumnos, crit_doble_nombre)
{
  // ==
  EXPECT_FALSE(db.criterioValido({ { Rig("LU_A", 5), Rig("LU_X", 5) } },
    "libretas"));
  EXPECT_TRUE(db.criterioValido({ { Rig("LU_A", 5), Rig("LU_N", 5) } },
    "libretas"));
  EXPECT_FALSE(db.criterioValido({ { Rig("LU", ""), Rig("os", "") } },
    "alumnos"));
  EXPECT_TRUE(db.criterioValido({ { Rig("LU", ""), Rig("OS", "") } },
    "alumnos"));

  // !=
  EXPECT_FALSE(db.criterioValido({ { Rdif("LU_A", 5), Rig("LU_X", 5) } },
    "libretas"));
  EXPECT_TRUE(db.criterioValido({ { Rdif("LU_A", 5), Rig("LU_N", 5) } },
    "libretas"));
  EXPECT_FALSE(db.criterioValido({ { Rdif("LU", ""), Rig("os", "") } },
    "alumnos"));
  EXPECT_TRUE(db.criterioValido({ { Rdif("LU", ""), Rig("OS", "") } },
    "alumnos"));
}

TEST_F(DBAlumnos, crit_doble_tipo)
{
  // ==
  EXPECT_FALSE(db.criterioValido({ { Rig("LU_A", 5), Rig("LU_N", "") } },
    "libretas"));
  EXPECT_TRUE(db.criterioValido({ { Rig("LU_A", 5), Rig("LU_N", 5) } },
    "libretas"));
  EXPECT_FALSE(db.criterioValido({ { Rig("LU", 4), Rig("OS", "") } },
    "alumnos"));
  EXPECT_TRUE(db.criterioValido({ { Rig("LU", ""), Rig("OS", "") } },
    "alumnos"));

  // !=
  EXPECT_FALSE(db.criterioValido({ { Rdif("LU_A", 5), Rig("LU_N", "") } },
    "libretas"));
  EXPECT_TRUE(db.criterioValido({ { Rdif("LU_A", 5), Rig("LU_N", 5) } },
    "libretas"));
  EXPECT_FALSE(db.criterioValido({ { Rdif("LU", 4), Rig("OS", "") } },
    "alumnos"));
  EXPECT_TRUE(db.criterioValido({ { Rdif("LU", ""), Rig("OS", "") } },
    "alumnos"));
}

// ## Uso Criterio
TEST_F(DBAlumnos, uso_un_criterio)
{
  db.busqueda({ Rig("OS", "A") }, "alumnos");
  EXPECT_EQ(db.uso_criterio({ Rig("OS", "A") }), 1);
  EXPECT_EQ(db.top_criterios(),
    linear_set<Database::Criterio>({ { Rig("OS", "A") } }));
  db.busqueda({ Rig("OS", "A") }, "alumnos");
  EXPECT_EQ(db.uso_criterio({ Rig("OS", "A") }), 2);
  EXPECT_EQ(db.top_criterios(),
    linear_set<Database::Criterio>({ { Rig("OS", "A") } }));

  db.busqueda({ Rig("LU_A", 1) }, "libretas");
  EXPECT_EQ(db.uso_criterio({ Rig("LU_A", 1) }), 1);
  EXPECT_EQ(db.top_criterios(),
    linear_set<Database::Criterio>({ { Rig("OS", "A") } }));

  db.busqueda({ Rig("LU_A", 1) }, "libretas");
  EXPECT_EQ(db.uso_criterio({ Rig("LU_A", 1) }), 2);
  EXPECT_EQ(db.top_criterios(),
    linear_set<Database::Criterio>({ { Rig("OS", "A") },
      { Rig("LU_A", 1) } }));
  db.busqueda({ Rig("LU_A", 1) }, "libretas");
  EXPECT_EQ(db.uso_criterio({ Rig("LU_A", 1) }), 3);
  EXPECT_EQ(db.top_criterios(),
    linear_set<Database::Criterio>({ { Rig("LU_A", 1) } }));
}

TEST_F(DBAlumnos, uso_un_criterio_perm)
{
  Database::Criterio c = { Rig("OS", "A"), Rig("Editor", "Vim") };
  Database::Criterio c_perm = { Rig("Editor", "Vim"), Rig("OS", "A") };
  Database::Criterio c_sim = { Rig("OS", "A") };
  db.busqueda(c, "alumnos");
  EXPECT_EQ(db.uso_criterio(c), 1);
  EXPECT_EQ(db.uso_criterio(c_sim), 0);
  EXPECT_EQ(db.top_criterios(),
    linear_set<Database::Criterio>({ c }));

  db.busqueda(c_perm, "alumnos");
  EXPECT_EQ(db.uso_criterio(c), 2);
  EXPECT_EQ(db.uso_criterio(c_sim), 0);
  EXPECT_EQ(db.top_criterios(),
    linear_set<Database::Criterio>({ c }));

  db.busqueda(c_sim, "alumnos");
  EXPECT_EQ(db.uso_criterio(c_sim), 1);
  EXPECT_EQ(db.top_criterios(),
    linear_set<Database::Criterio>({ c }));
}

TEST_F(DBAlumnos, crit_otro_bool)
{
  Database::Criterio c = { Rig("OS", "A") };
  Database::Criterio c_inv = { Rdif("OS", "A") };

  EXPECT_EQ(db.uso_criterio(c), 0);
  EXPECT_EQ(db.uso_criterio(c_inv), 0);

  db.busqueda(c, "alumnos");
  EXPECT_EQ(db.uso_criterio(c), 1);
  EXPECT_EQ(db.uso_criterio(c_inv), 0);

  db.busqueda(c_inv, "alumnos");
  EXPECT_EQ(db.uso_criterio(c), 1);
  EXPECT_EQ(db.uso_criterio(c_inv), 1);
}

TEST_F(DBAlumnos, crit_doble_otro_bool)
{
  Database::Criterio c = { Rig("OS", "A"), Rig("Editor", "Vim") };
  Database::Criterio c_inv = { Rdif("OS", "A"), Rig("Editor", "Vim") };

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
// * Join columns repetidos

TEST_F(DBAlumnos, join_vacio)
{
  db.crearIndice("alumnos", "LU");
  auto begin = db.join("alumnos", "ex_alumnos", "LU");
  auto end = db.join_end();
  EXPECT_EQ(begin, end);
}

TEST_F(DBAlumnos, join_sin_repetidos)
{
  db.crearIndice("alumnos", "LU");
  auto begin = db.join("libretas", "alumnos", "LU");
  auto end = db.join_end();
  linear_set<string> nuevos_campos({ "LU_N", "LU_A", "LU", "Nombre", "Editor", "OS" });

  size_t count = 0;
  for (auto it = begin; it != end; it++)
  {
    EXPECT_EQ((*it).columns(), nuevos_campos);
    count++;
  }

  EXPECT_EQ(count, db.dameTabla("libretas").records().size());
  begin = db.join("libretas", "alumnos", "LU");
  linear_set<Record> join(begin, end);
  EXPECT_EQ(join, join_libretas_alumnos.records());
}

TEST_F(DBAlumnos, join_repetidos_uno)
{
  db.crearIndice("libretas", "LU");
  auto begin = db.join("libretas", "materias", "LU");
  auto end = db.join_end();

  linear_set<string> nuevos_campos({ "LU_N", "LU_A", "LU", "Materia" });
  for (auto it = begin; it != end; it++)
  {
    EXPECT_EQ((*it).columns(), nuevos_campos);
  }

  linear_set<Record> join(begin, end);
  EXPECT_EQ(join, join_libretas_materias.records());
}

TEST_F(DBAlumnos, join_repetidos_ambos)
{
  Database db2;
  db2.crearTabla("T1", { "X", "Y" }, { "X", "Y" }, { Datum{ 0 }, Datum{ 0 } });
  db2.crearTabla("T2", { "Y", "Z" }, { "Y", "Z" }, { Datum{ 0 }, Datum{ "" } });
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
  db2.agregarRegistro(Record({ "X", "Y" }, { Datum(1), Datum(1) }), "T1");
  db2.agregarRegistro(Record({ "X", "Y" }, { Datum(2), Datum(2) }), "T1");
  db2.agregarRegistro(Record({ "X", "Y" }, { Datum(3), Datum(2) }), "T1");
  db2.agregarRegistro(Record({ "X", "Y" }, { Datum(4), Datum(0) }), "T1");
  db2.agregarRegistro(Record({ "Y", "Z" }, { Datum(1), Datum("A") }), "T2");
  db2.agregarRegistro(Record({ "Y", "Z" }, { Datum(1), Datum("B") }), "T2");
  db2.agregarRegistro(Record({ "Y", "Z" }, { Datum(2), Datum("C") }), "T2");

  Table t_join = Table({ "X", "Y", "Z" }, { "X", "Y", "Z" }, { Datum{ 0 }, Datum{ 0 }, Datum{ "" } });
  t_join.addRecord(Record({ "X", "Y", "Z" },
    { Datum(1), Datum(1), Datum("A") }));
  t_join.addRecord(Record({ "X", "Y", "Z" },
    { Datum(1), Datum(1), Datum("B") }));
  t_join.addRecord(Record({ "X", "Y", "Z" },
    { Datum(2), Datum(2), Datum("C") }));
  t_join.addRecord(Record({ "X", "Y", "Z" },
    { Datum(3), Datum(2), Datum("C") }));

  db2.crearIndice("T1", "Y");
  db2.crearIndice("T2", "Y");
  auto begin = db2.join("T1", "T2", "Y");
  auto end = db2.join_end();

  linear_set<string> nuevos_campos({ "X", "Y", "Z" });
  for (auto it = begin; it != end; it++)
  {
    EXPECT_EQ((*it).columns(), nuevos_campos);
  }

  linear_set<Record> join(begin, end);
  EXPECT_EQ(join, t_join.records());
}

TEST_F(DBAlumnos, join_campos_repetidos)
{
  Database db2;
  db2.crearTabla("T1", { "X", "Y" }, { "X", "Y" }, { Datum{ 0 }, Datum{ 0 } });
  db2.crearTabla("T2", { "X", "Y", "Z" }, { "X", "Y", "Z" }, { Datum{ 0 }, Datum{ 0 }, Datum{ "" } });
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
  db2.agregarRegistro(Record({ "X", "Y" }, { Datum(1), Datum(1) }), "T1");
  db2.agregarRegistro(Record({ "X", "Y" }, { Datum(2), Datum(2) }), "T1");
  db2.agregarRegistro(Record({ "X", "Y", "Z" }, { Datum(1), Datum(1), Datum("A") }), "T2");
  db2.agregarRegistro(Record({ "X", "Y", "Z" }, { Datum(3), Datum(2), Datum("C") }), "T2");

  Table t_join_a = Table({ "X", "Y", "Z" }, { "X", "Y", "Z" }, { Datum{ 0 }, Datum{ 0 }, Datum{ "" } });
  t_join_a.addRecord(Record({ "X", "Y", "Z" }, { Datum(1), Datum(1), Datum("A") }));
  t_join_a.addRecord(Record({ "X", "Y", "Z" }, { Datum(2), Datum(2), Datum("C") }));

  Table t_join_b = Table({ "X", "Y", "Z" }, { "X", "Y", "Z" }, { Datum{ 0 }, Datum{ 0 }, Datum{ "" } });
  t_join_b.addRecord(Record({ "X", "Y", "Z" }, { Datum(1), Datum(1), Datum("A") }));
  t_join_b.addRecord(Record({ "X", "Y", "Z" }, { Datum(3), Datum(2), Datum("C") }));

  db2.crearIndice("T1", "X");
  db2.crearIndice("T2", "Z");
  db2.crearIndice("T2", "Y");
  auto begin = db2.join("T1", "T2", "Y");
  auto end = db2.join_end();

  linear_set<Record> join(begin, end);
  EXPECT_EQ(join, t_join_a.records());

  begin = db2.join("T2", "T1", "Y");
  end = db2.join_end();

  linear_set<Record> join_b(begin, end);
  EXPECT_EQ(join_b, t_join_b.records());
}
