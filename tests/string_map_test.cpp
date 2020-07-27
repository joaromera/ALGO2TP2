#include <gtest/gtest.h>
#include <list>
#include "../src/string_map.h"
#include "../src/Datum.h"

using namespace Db::Types;

TEST(string_map_test, memory_leak)
{
  string_map<int> m1;
  m1["A"] = 1;
  m1["Aab"] = 2;
  auto it = m1.find("Aab");
  it = m1.erase(it);
}

TEST(string_map_test, joaco_test)
{
  string_map<int> m1, m2;
  m1["!"] = 1;
  m1["!@"] = 1;
  m1["#"] = 2;
  EXPECT_FALSE(m1 == m2);
  m2["!"] = 1;
  m2["!@"] = 1;
  m2["#"] = 2;
  EXPECT_TRUE(m1 == m2);
  auto it = m1.begin();
  auto it2 = m1.erase(it);
  it++;
  EXPECT_TRUE(it == it2);
  m1.erase("!@");
}

TEST(string_map_test, test_constructor)
{
  string_map<int> m1, m2;
  string_map<int> m3(m1);
  string_map<string_map<std::string>> m4;
}

TEST(string_map_test, test_definir)
{
  string_map<int> m;
  EXPECT_TRUE(m.empty());
  EXPECT_EQ(m.size(), (size_t) 0);
  EXPECT_FALSE(m.count(""));
  m["hola"] = 3;
  EXPECT_FALSE(m.count(""));
  EXPECT_EQ(m.size(), (size_t) 1);
  EXPECT_FALSE(m.empty());
  EXPECT_TRUE(m.count("hola"));
  EXPECT_FALSE(m.count("hol"));
  EXPECT_FALSE(m.count("holas"));
  m["ho"] = 1;
  EXPECT_FALSE(m.count(""));
  EXPECT_EQ(m.size(), (size_t) 2);
  EXPECT_TRUE(m.count("hola"));
  EXPECT_TRUE(m.count("ho"));
  EXPECT_FALSE(m.count("hol"));
  EXPECT_FALSE(m.count("h"));
  m[""] = 0;
  EXPECT_EQ(m.size(), (size_t) 3);
  EXPECT_TRUE(m.count("hola"));
  EXPECT_TRUE(m.count("ho"));
  EXPECT_TRUE(m.count(""));

  string_map<std::string> m2;
  m2["mundo"] = "feliz";
  string_map<string_map<std::string>> m3;
  EXPECT_NE(m2, m3["hola"]);
  m3["hola"]["mundo"] = "feliz";
  EXPECT_EQ(m2, m3["hola"]);
}

TEST(string_map_test, test_obtener)
{
  string_map<int> m;
  m["hola"] = 3;
  EXPECT_EQ(m["hola"], 3);
  EXPECT_EQ(m.at("hola"), 3);
  m["ho"] = 1;
  EXPECT_EQ(m["ho"], 1);
  EXPECT_EQ(m.at("ho"), 1);

  // test version const de "at"
  const string_map<int> m2(m);
  EXPECT_EQ(m2.at("ho"), 1);
}

TEST(string_map_test, test_comparar)
{
  string_map<int> m1, m2;
  m1["hola"] = 123;
  m1["mundo"] = 456;
  EXPECT_FALSE(m1 == m2);
  m2["hola"] = 123;
  m2["mundo"] = 456;
  EXPECT_TRUE(m1 == m2);
  m2["mundo"] = 45;
  EXPECT_FALSE(m1 == m2);
}

TEST(string_map_test, test_copiar)
{
  string_map<int> m1;
  m1["hola"] = 123;
  m1["mundo"] = 456;
  string_map<int> m2(m1);
  EXPECT_TRUE(m1 == m2);
  string_map<int> m3;
  EXPECT_FALSE(m1 == m3);
}

TEST(string_map_test, test_asignar)
{
  string_map<int> m1, m2;
  m1["hola"] = 123;
  m1["mundo"] = 456;
  EXPECT_FALSE(m1 == m2);
  m2 = m1;
  EXPECT_TRUE(m1 == m2);
  m2 = string_map<int>();
  EXPECT_FALSE(m1 == m2);
}

TEST(string_map_test, test_eliminar)
{
  string_map<int> m1;
  EXPECT_TRUE(m1.empty());
  m1["hola"] = 123;
  EXPECT_TRUE(m1.count("hola"));
  m1.erase("hola");
  EXPECT_FALSE(m1.count("hola"));
  EXPECT_FALSE(m1.count("ho"));
  EXPECT_FALSE(m1.count(""));
  m1["hola"] = 1;
  m1["ho"] = 3;
  m1[""] = 4;
  m1.erase("");
  EXPECT_TRUE(m1.count("hola"));
  EXPECT_TRUE(m1.count("ho"));
  m1.erase("ho");
  EXPECT_TRUE(m1.count("hola"));
  EXPECT_FALSE(m1.empty());
  m1.erase("hola");
  EXPECT_FALSE(m1.count("hola"));
  EXPECT_TRUE(m1.empty());
}

TEST(string_map_test, test_vaciar)
{
  string_map<int> m1;
  m1["hola"] = 1;
  m1["ho"] = 3;
  m1[""] = 4;
  EXPECT_TRUE(m1.count("hola"));
  EXPECT_TRUE(m1.count("ho"));
  EXPECT_TRUE(m1.count(""));
  EXPECT_FALSE(m1.empty());
  m1.clear();
  EXPECT_FALSE(m1.count("hola"));
  EXPECT_FALSE(m1.count("ho"));
  EXPECT_FALSE(m1.count(""));
  EXPECT_TRUE(m1.empty());
}

TEST(string_map_test, test_tipo_complejo)
{
  struct punto2d
  {
    int x, y;
    punto2d() : x(0), y(0) {}
    punto2d(int a, int b) : x(a), y(b) {}
    bool operator==(const punto2d &otro) const { return x == otro.x && y == otro.y; }
    bool operator!=(const punto2d &otro) const { return !(*this == otro); }
  };
  string_map<punto2d> m;
  m["origen"] = { 0, 0 };
  EXPECT_EQ(m["origen"], m.at("origen"));
  m["origen"].x = 1;
  EXPECT_EQ(m["origen"], punto2d(1, 0));
  m["origen"].y = 2;
  EXPECT_NE(m["origen"], punto2d(1, 0));
  EXPECT_EQ(m["origen"], punto2d(1, 2));
}

TEST(string_map_test, test_iterator)
{
  string_map<int> m1;
  // 3 ramas disjuntas: las debe iterar en orden lexicográfico
  m1["mundo"] = 2;
  m1["aaaa"] = 0;
  m1["hola"] = 1;
  /**
     * m1:  .-a-a-a-a[0]
     *      |-h-o-l-a[1]
     *      \-m-u-n-d-o[2]
     */
  std::list<int> l1, l2 = { 0, 1, 2 };
  for (auto p : m1)
    l1.push_back(p.second);
  EXPECT_EQ(l1, l2);

  // Agrego subramas de "aa": debe iterar en orden correcto lexicográfico
  m1["aaaab"] = 12;
  m1["aaaba"] = 24;
  m1["aabba"] = 48;
  /**
     * m1:  .-a-a-a-a[0]-b[12]
     *      |    | \b-a[24]
     *      |    \b-b-a[48]
     *      |-h-o-l-a[1]
     *      \-m-u-n-d-o[2]
     */

  std::list<int> l3, l4 = { 0, 12, 24, 48, 1, 2 };
  for (auto p : m1)
    l3.push_back(p.second);
  EXPECT_EQ(l3, l4);

  // Borro 1ra key: debe apuntar a la siguiente (en este caso una subrama)
  EXPECT_TRUE(m1.erase("aaaa"));

  auto it = m1.find("aaaab");
  it = m1.erase(it);
  /**
     * m1:  .-a-a-a-b-a[24]
     *      |    \b-b-a[48]
     *      |-h-o-l-a[1]
     *      \-m-u-n-d-o[2]
     */
  EXPECT_EQ(it->first, "aaaba");
  EXPECT_EQ(it->second, 24);

  // borro ultima key de una subrama: debe devolver la de rama siguiente pasando por la raiz
  it = m1.find("aabba");
  it = m1.erase(it);
  /**
     * m1:  .-a-a-a-b-a[24]
     *      |-h-o-l-a[1]
     *      \-m-u-n-d-o[2]
     */
  EXPECT_EQ(it, m1.find("hola"));

  // borro ultima key: debe devolver end()
  EXPECT_EQ(m1.erase(m1.find("mundo")), m1.end());
  /**
     * m1:  .-a-a-a-b-a[24]
     *      \-h-o-l-a[1]
     */
  std::list<int> l5, l6 = { 24, 1 };
  for (auto p : m1)
    l5.push_back(p.second);
  EXPECT_EQ(l5, l6);
}

TEST(string_map_test, no_default)
{
  string_map<Datum> dato_map;
  dato_map.insert(std::make_pair("March", Datum("March")));
  dato_map.insert(std::make_pair("EvilMarch", Datum(-1000)));

  EXPECT_NE(dato_map.find("March"), dato_map.end());
  EXPECT_NE(dato_map.find("EvilMarch"), dato_map.end());
}
