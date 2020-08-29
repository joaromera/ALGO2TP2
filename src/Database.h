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

typedef string_map<string_map<std::map<Datum, linear_set<Record>>>> IndexRef;

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
        datumKeys = n.datumKeys;
        campo = n.campo;
        isFinal = n.isFinal;
        orden = n.orden;
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
        datumKeys = n.datumKeys;
        campo = n.campo;
        isFinal = n.isFinal;
        tableRecordCount = n.tableRecordCount;
        tableRecordCountByKey = n.tableRecordCountByKey;
        orden = n.orden;
      }
      return *this;
    }

    join_iterator(linear_set<Record>::const_iterator a, Table::const_iterator c, int ind, int sin, std::shared_ptr<std::map<Datum, linear_set<Record>>> e, const std::string &f, const bool &o)
      : tableRecordCountByKey(ind)
      , tableRecordCount(sin)
      , datumKeys(e)
      , campo(f)
      , isFinal(false)
      , orden(o)
    {
      it1 = std::make_shared<linear_set<Record>::const_iterator>(a);
      it2 = std::make_shared<Table::const_iterator>(c);
    }

    ~join_iterator()
    {
    }

    bool operator==(const join_iterator &j) const
    {
      if (isFinal == j.isFinal) return true;

      return it1 == j.it1 && it2 == j.it2 && datumKeys == j.datumKeys && tableRecordCount == j.tableRecordCount && tableRecordCountByKey == j.tableRecordCountByKey && campo == j.campo && orden == j.orden;
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
          auto key = (*it2)->value(campo);
          findNextMatchByDatum(key);
          return *this;
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
      datumKeys.reset();
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

    void findNextMatchByDatum(Datum datum)
    {
      while (tableRecordCount != 0 && datumKeys->count(datum) == 0)
      {
        datum = (*it2)->value(campo);
        if (datumKeys->count(datum) == 0) incrementIteratorWithoutIndex();
      }

      if (tableRecordCount == 0)
      {
        advanceToEnd();
      }
      else if (datumKeys->count(datum) != 0)
      {
        setIteratorToNewDatumKey(datum);
      }
      else
      {
        advanceToEnd();
      }
    }

    void setIteratorToNewDatumKey(const Datum &datum)
    {
      it1 = std::make_shared<linear_set<Record>::const_iterator>(datumKeys->at(datum).begin());
      tableRecordCountByKey = datumKeys->at(datum).size();
    }

    std::shared_ptr<linear_set<Record>::const_iterator> it1 {nullptr};
    std::shared_ptr<Table::const_iterator> it2 {nullptr};
    int tableRecordCountByKey{0};
    int tableRecordCount{0};
    std::shared_ptr<std::map<Datum, linear_set<Record>>> datumKeys {nullptr};
    std::string campo {""};
    bool isFinal {true};
    bool orden {true};
  };

  typedef linear_set<Filter> Filters;

  Database();

  void createTable(const std::string &name, const linear_set<std::string> &keys, const std::vector<std::string> &columns, const std::vector<Datum> &types);

  void addRecord(const Record &record, const std::string &tableName);

  const linear_set<std::string> &tableNames() const;

  const Table &getTable(const std::string &nombre) const;

  int filterUsageCount(const Filters &filter) const;

  bool isValidRecord(const Record &record, const std::string &name) const;

  bool isValidFilter(const Filters &c, const std::string &nombre) const;

  Table search(const Filters &c, const std::string &nombre);

  linear_set<Filters> mostUsedFilter() const;

  void createIndex(const std::string &table, const std::string &column);

  bool hasIndex(const std::string &table, const std::string &column);

  join_iterator join(const std::string &table1, const std::string &table2, const std::string &columnName);

  join_iterator join_end();

private:
  std::list<Record> &_filterRecords(const std::string &column, const Datum &value, std::list<Record> &records, bool equals) const;

  std::pair<std::vector<std::string>, std::vector<Datum>> _tableTypes(const Table &t);

  Database::join_iterator join_helper(const std::string &tabla1, const std::string &tabla2, const std::string &campo, const bool &orden);

  linear_set<std::string> _tableNames;
  string_map<Table> _tables;
  linear_map<Filters, int> _uso_criterios;
  IndexRef _indexRefs;
};
