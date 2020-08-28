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
    }

    join_iterator(const join_iterator &n)
    {
      if (n.isFinal)
      {
        advanceToEnd();
      }
      else
      {
        it1 = std::make_shared<linear_set<Record>::const_iterator>(*n.it1);
        it2 = std::make_shared<Table::const_iterator>(*n.it2);
        tableRecordCountByKey = n.tableRecordCountByKey;
        tableRecordCount = n.tableRecordCount;
        stringKeys = n.stringKeys;
        integerKeys = n.integerKeys;
        campo = n.campo;
        isFinal = n.isFinal;
        orden = n.orden;
        tipo = n.tipo;
      }
    }

    join_iterator &operator=(const join_iterator &n)
    {
      if (n.isFinal)
      {
        advanceToEnd();
      }
      else
      {
        it1 = std::make_shared<linear_set<Record>::const_iterator>(*n.it1);
        it2 = std::make_shared<Table::const_iterator>(*n.it2);
        stringKeys = n.stringKeys;
        integerKeys = n.integerKeys;
        campo = n.campo;
        isFinal = n.isFinal;
        tableRecordCount = n.tableRecordCount;
        tableRecordCountByKey = n.tableRecordCountByKey;
        orden = n.orden;
        tipo = n.tipo;
      }
      return *this;
    }

    ~join_iterator()
    {
    }

    bool operator==(const join_iterator &j) const
    {
      if (isFinal == j.isFinal) return true;

      return it1 == j.it1 && it2 == j.it2 && stringKeys == j.stringKeys && integerKeys == j.integerKeys && tableRecordCount == j.tableRecordCount && tableRecordCountByKey == j.tableRecordCountByKey && campo == j.campo && orden == j.orden && tipo == j.tipo;
    }

    bool operator!=(const join_iterator &j) const
    {
      return !(*this == j);
    }

    join_iterator &operator++()
    {
      incrementIteratorWithIndex();
      if (tableRecordCountByKey == 0)
      {
        incrementIteratorWithoutIndex();
        if (tableRecordCount == 0)
        {
          advanceToEnd();
          return *this;
        }
        else
        {
          if (tipo == 0)
          {
            auto key = (*it2)->value(campo).value<std::string>();
            findNextMatchByString(key);
            return *this;
          }
          else
          {
            auto key = (*it2)->value(campo).value<int>();
            findNextMatchByInteger(key);
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
      auto tmp = *this;
      ++*this;
      return tmp;
    }

    const Record operator*()
    {
      if (orden) return mergeRecords(**it1, **it2);
      return mergeRecords(**it2, **it1);
    }

    friend class Database;
    friend class Table;

  private:
    join_iterator(linear_set<Record>::const_iterator a,
      Table::const_iterator c,
      int ind,
      int sin,
      std::shared_ptr<string_map<linear_set<Record>>> e,
      std::shared_ptr<std::map<int, linear_set<Record>>> g,
      const std::string &f,
      const bool &o,
      int t)
      : tableRecordCountByKey(ind)
      , tableRecordCount(sin)
      , stringKeys(e)
      , integerKeys(g)
      , campo(f)
      , isFinal(false)
      , orden(o)
      , tipo(t)
    {
      if (t == 0) // string
      {
        it1 = std::make_shared<linear_set<Record>::const_iterator>(a);
        it2 = std::make_shared<Table::const_iterator>(c);
      }
      else // integer
      {
        it1 = std::make_shared<linear_set<Record>::const_iterator>(a);
        it2 = std::make_shared<Table::const_iterator>(c);
      }
    }

    std::shared_ptr<linear_set<Record>::const_iterator> it1 {nullptr};
    std::shared_ptr<Table::const_iterator> it2 {nullptr};
    int tableRecordCountByKey{0};
    int tableRecordCount{0};
    std::shared_ptr<string_map<linear_set<Record>>> stringKeys {nullptr};
    std::shared_ptr<std::map<int, linear_set<Record>>> integerKeys {nullptr};
    std::string campo {""};
    bool isFinal {true};
    bool orden {true};
    int tipo {0};

    Record mergeRecords(const Record & r1, const Record & r2)
    {
      std::vector<std::string> mergedColumns;
      std::vector<Datum> mergedValues;

      for (const auto& c : r1.columns())
      {
        mergedColumns.emplace_back(c);
        mergedValues.emplace_back(r1.value(c));
      }

      for (const auto& c : r2.columns())
      {
        if (r1.columns().count(c) == 0 && c != campo)
        {
          mergedColumns.emplace_back(c);
          mergedValues.emplace_back(r2.value(c));
        }
      }

      return { mergedColumns, mergedValues };
    }

    void advanceToEnd()
    {
      it1.reset();
      it2.reset();
      integerKeys.reset();
      stringKeys.reset();
      isFinal = true;
      campo = "";
    }

    void incrementIteratorWithIndex()
    {
      ++(*it1);
      tableRecordCountByKey--;
    }

    void incrementIteratorWithoutIndex()
    {
      ++(*it2);
      tableRecordCount--;
    }

    void findNextMatchByString(std::string &key)
    {
      while (tableRecordCount != 0 && stringKeys->count(key) == 0)
      {
        key = (*it2)->value(campo).value<std::string>();
        if (stringKeys->count(key) == 0) incrementIteratorWithoutIndex();
      }

      if (tableRecordCount == 0)
      {
        advanceToEnd();
      }
      else if (stringKeys->count(key) != 0)
      {
        setIteratorToNewStringKey(key);
      }
      else
      {
        advanceToEnd();
      }
    }

    void findNextMatchByInteger(int &key)
    {
      while (tableRecordCount != 0 && integerKeys->count(key) == 0)
      {
        key = (*it2)->value(campo).value<int>();
        if (integerKeys->count(key) == 0) incrementIteratorWithoutIndex();
      }

      if (tableRecordCount == 0)
      {
        advanceToEnd();
      }
      else if (integerKeys->count(key) != 0)
      {
        setIteratorToNewIntegerKey(key);
      }
      else
      {
        advanceToEnd();
      }
    }

    void setIteratorToNewStringKey(const std::string &key)
    {
      it1 = std::make_shared<linear_set<Record>::const_iterator>(stringKeys->at(key).begin());
      tableRecordCountByKey = stringKeys->at(key).size();
    }

    void setIteratorToNewIntegerKey(const int &key)
    {
      it1 = std::make_shared<linear_set<Record>::const_iterator>(integerKeys->at(key).begin());
      tableRecordCountByKey = integerKeys->at(key).size();
    }
  };

  typedef linear_set<Filter> Filters;

  Database();

  void crearTabla(const std::string &nombre, const linear_set<std::string> &claves, const std::vector<std::string> &campos, const std::vector<Datum> &tipos);

  void agregarRegistro(const Record &r, const std::string &nombre);

  const linear_set<std::string> &tablas() const;

  const Table &dameTabla(const std::string &nombre) const;

  int uso_criterio(const Filters &criterio) const;

  bool registroValido(const Record &r, const std::string &nombre) const;

  bool criterioValido(const Filters &c, const std::string &nombre) const;

  Table busqueda(const Filters &c, const std::string &nombre);

  linear_set<Filters> top_criterios() const;

  void crearIndice(const std::string &tabla, const std::string &campo);

  bool tieneIndice(const std::string &tabla, const std::string &campo);

  join_iterator join(const std::string &tabla1, const std::string &tabla2, const std::string &campo);

  join_iterator join_end();

private:
  linear_set<std::string> _nombres_tablas;
  string_map<Table> _tablas;
  linear_map<Filters, int> _uso_criterios;
  Index _indices;
  IndexInt _indicesNum;

  std::list<Record> &_filtrar_registros(const std::string &campo, const Datum &valor, std::list<Record> &registros, bool igualdad) const;

  std::list<Record> &_filtrar_registros(const std::string &campo, const Datum &valor, std::list<Record> &registros) const;

  std::pair<std::vector<std::string>, std::vector<Datum>> _tipos_tabla(const Table &t);

  join_iterator join_helper_str(const std::string &tabla1, const std::string &tabla2, const std::string &campo, const bool &orden);

  join_iterator join_helper_int(const std::string &tabla1, const std::string &tabla2, const std::string &campo, const bool &orden);
};
