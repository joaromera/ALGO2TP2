#pragma once
#include "Record.h"
#include "Filter.h"
#include "Table.h"
#include <utility>
#include <list>
#include <string>
#include "linear_map.h"
#include "linear_set.h"
#include "string_map.h"
#include <map>

using namespace Db::Types;

typedef string_map<string_map<string_map<linear_set<Record>>>> Index;
typedef string_map<string_map<std::map<int, linear_set<Record>>>> IndexInt;

class Database
{
public:
  class join_iterator
  {
  public:
    join_iterator()
    {
      it1 = nullptr;
      it2 = nullptr;
      cant_reg_en_tabla = 0;
      cant_reg_por_clave = 0;
      diccClavesStr = nullptr;
      diccClavesNat = nullptr;
      campo = "";
      final = true;
      orden = true;
      tipo = 0;
    }

    join_iterator(const join_iterator &n)
    {
      it1 = nullptr;
      it2 = nullptr;
      if (n.final)
      {
        it1 = nullptr;
        it2 = nullptr;
        cant_reg_en_tabla = 0;
        cant_reg_por_clave = 0;
        diccClavesStr = nullptr;
        diccClavesNat = nullptr;
        campo = "";
        final = true;
        orden = true;
        tipo = 0;
      }
      else
      {
        if (it1 != nullptr)
        {
          it1 = nullptr;
        }
        if (it2 != nullptr)
        {
          it2 = nullptr;
        }
        it1 = std::make_shared<linear_set<Record>::const_iterator>(*n.it1);
        it2 = std::make_shared<Table::const_iterator>(*n.it2);
        cant_reg_por_clave = n.cant_reg_por_clave;
        cant_reg_en_tabla = n.cant_reg_en_tabla;
        diccClavesStr = n.diccClavesStr;
        diccClavesNat = n.diccClavesNat;
        campo = n.campo;
        final = n.final;
        orden = n.orden;
        tipo = n.tipo;
      }
    }

    join_iterator &operator=(const join_iterator &n)
    {
      if (n.final == true)
      {
        it1 = nullptr;
        it2 = nullptr;
        diccClavesNat = nullptr;
        diccClavesStr = nullptr;
        final = true;
      }
      else
      {
        if (it1 != nullptr)
        {
          it1 = nullptr;
        }
        if (it2 != nullptr)
        {
          it2 = nullptr;
        }
        it1 = std::make_shared<linear_set<Record>::const_iterator>(*n.it1);
        it2 = std::make_shared<Table::const_iterator>(*n.it2);
        ;
        diccClavesStr = n.diccClavesStr;
        diccClavesNat = n.diccClavesNat;
        campo = n.campo;
        final = n.final;
        cant_reg_en_tabla = n.cant_reg_en_tabla;
        cant_reg_por_clave = n.cant_reg_por_clave;
        orden = n.orden;
        tipo = n.tipo;
      }
      return *this;
    }

    ~join_iterator()
    {
      it1 = nullptr;
      it2 = nullptr;
      diccClavesStr = nullptr;
      diccClavesNat = nullptr;
    }

    bool operator==(const join_iterator &j) const
    {
      if (final == j.final)
      {
        return true;
      }
      else
      {
        return it1 == j.it1 && it2 == j.it2 && diccClavesStr == j.diccClavesStr && diccClavesNat == j.diccClavesNat && cant_reg_en_tabla == j.cant_reg_en_tabla && cant_reg_por_clave == j.cant_reg_por_clave && campo == j.campo && orden == j.orden && tipo == j.tipo;
      }
    }

    bool operator!=(const join_iterator &j) const
    {
      return !(*this == j);
    }

    join_iterator &operator++()
    {
      avanzarItConIndice();
      if (cant_reg_por_clave == 0)
      {
        avanzarItSinIndice();
        if (cant_reg_en_tabla == 0)
        {
          ponerEnFin();
          return *this;
        }
        else
        {
          if (tipo == 0)
          {
            std::string clave = (*it2)->dato(campo).stringValue();
            buscarProxCoincidenciaStr(clave);
            return *this;
          }
          else
          {
            int clave = (*it2)->dato(campo).integerValue();
            buscarProxCoincidenciaNat(clave);
            return *this;
          }
        }
      }
      else
      {
        return *this;
      }
    }

    join_iterator operator++(int)
    {
      auto copia = *this;
      ++*this;
      return copia;
    }

    const Record operator*()
    {
      if (orden)
      {
        Record nuevo = combinarRegistro(**it1, **it2);
        return nuevo;
      }
      else
      {
        Record nuevo = combinarRegistro(**it2, **it1);
        return nuevo;
      }
    }

    friend class Database;
    friend class Table;

  private:
    join_iterator(linear_set<Record>::const_iterator a,
      int ind,
      Table::const_iterator c,
      int sin,
      const string_map<linear_set<Record>> *e,
      const std::map<int, linear_set<Record>> *g,
      const std::string &f,
      const bool &o,
      int t) : cant_reg_por_clave(ind),
               cant_reg_en_tabla(sin),
               diccClavesStr(e), diccClavesNat(g),
               campo(f), final(false), orden(o), tipo(t)
    {
      if (t == 0)
      {// es string
        it1 = std::make_shared<linear_set<Record>::const_iterator>(a);
        it2 = std::make_shared<Table::const_iterator>(c);
      }
      else
      {// es nat
        it1 = std::make_shared<linear_set<Record>::const_iterator>(a);
        it2 = std::make_shared<Table::const_iterator>(c);
      }
    };

    join_iterator(const bool &) : it1(nullptr), cant_reg_por_clave(0),
                                  it2(nullptr), cant_reg_en_tabla(0),
                                  diccClavesStr(nullptr), diccClavesNat(nullptr),
                                  campo(""), final(true), orden(true), tipo(0){};

    std::shared_ptr<linear_set<Record>::const_iterator> it1;
    int cant_reg_por_clave;
    std::shared_ptr<Table::const_iterator> it2;
    int cant_reg_en_tabla;
    const string_map<linear_set<Record>> *diccClavesStr;
    const std::map<int, linear_set<Record>> *diccClavesNat;
    std::string campo;
    bool final;
    bool orden;
    int tipo;

    Record combinarRegistro(const Record & r1, const Record & r2)
    {
      std::vector<std::string> combCampos;
      std::vector<Datum> combDatos;

      for (const auto& c : r1.campos())
      {
        combCampos.emplace_back(c);
        combDatos.emplace_back(r1.dato(c));
      }

      for (const auto& c : r2.campos())
      {
        if (r1.campos().count(c) == 0 && c != campo)
        {
          combCampos.emplace_back(c);
          combDatos.emplace_back(r2.dato(c));
        }
      }

      return Record(combCampos, combDatos);
    }

    void ponerEnFin()
    {
      it1 = nullptr;
      it2 = nullptr;
      campo = "";
      final = true;
      diccClavesNat = nullptr;
      diccClavesStr = nullptr;
    }

    void avanzarItConIndice()
    {
      ++(*it1);
      cant_reg_por_clave--;
    }

    void avanzarItSinIndice()
    {
      ++(*it2);
      cant_reg_en_tabla--;
    }

    void buscarProxCoincidenciaStr(std::string &clave)
    {
      while (cant_reg_en_tabla != 0 && diccClavesStr->count(clave) == 0)
      {
        clave = (*it2)->dato(campo).stringValue();
        if (diccClavesStr->count(clave) == 0)
        {
          avanzarItSinIndice();
        }
      }
      if (cant_reg_en_tabla == 0)
      {
        ponerEnFin();
      }
      else if (diccClavesStr->count(clave) != 0)
      {
        setItEnNuevaClaveStr(clave);
      }
      else
      {
        ponerEnFin();
      }
    }

    void buscarProxCoincidenciaNat(int &clave)
    {
      while (cant_reg_en_tabla != 0 && diccClavesNat->count(clave) == 0)
      {
        clave = (*it2)->dato(campo).integerValue();
        if (diccClavesNat->count(clave) == 0)
        {
          avanzarItSinIndice();
        }
      }
      if (cant_reg_en_tabla == 0)
      {
        ponerEnFin();
      }
      else if (diccClavesNat->count(clave) != 0)
      {
        setItEnNuevaClaveNat(clave);
      }
      else
      {
        ponerEnFin();
      }
    }

    void setItEnNuevaClaveStr(const std::string &clave)
    {
      it1 = std::make_shared<linear_set<Record>::const_iterator>(diccClavesStr->at(clave).begin());
      cant_reg_por_clave = diccClavesStr->at(clave).size();
    }

    void setItEnNuevaClaveNat(const int &clave)
    {
      it1 = std::make_shared<linear_set<Record>::const_iterator>(diccClavesNat->at(clave).begin());
      cant_reg_por_clave = diccClavesNat->at(clave).size();
    }
  };

  typedef linear_set<Filter> Criterio;

  Database();

  void crearTabla(const std::string &nombre, const linear_set<std::string> &claves, const std::vector<std::string> &campos, const std::vector<Datum> &tipos);

  void agregarRegistro(const Record &r, const std::string &nombre);

  const linear_set<std::string> &tablas() const;

  const Table &dameTabla(const std::string &nombre) const;

  int uso_criterio(const Criterio &criterio) const;

  bool registroValido(const Record &r, const std::string &nombre) const;

  bool criterioValido(const Criterio &c, const std::string &nombre) const;

  Table busqueda(const Criterio &c, const std::string &nombre);

  linear_set<Criterio> top_criterios() const;

  void crearIndice(const std::string &tabla, const std::string &campo);

  bool tieneIndice(const std::string &tabla, const std::string &campo);

  join_iterator join(const std::string &tabla1, const std::string &tabla2, const std::string &campo);

  join_iterator join_end();

private:
  linear_set<std::string> _nombres_tablas;
  string_map<Table> _tablas;
  linear_map<Criterio, int> _uso_criterios;
  Index _indices;
  IndexInt _indicesNum;

  std::list<Record> &_filtrar_registros(const std::string &campo, const Datum &valor, std::list<Record> &registros, bool igualdad) const;

  std::list<Record> &_filtrar_registros(const std::string &campo, const Datum &valor, std::list<Record> &registros) const;

  std::pair<std::vector<std::string>, std::vector<Datum>> _tipos_tabla(const Table &t);

  join_iterator join_helper_str(const std::string &tabla1, const std::string &tabla2, const std::string &campo, const bool &orden);

  join_iterator join_helper_int(const std::string &tabla1, const std::string &tabla2, const std::string &campo, const bool &orden);
};
