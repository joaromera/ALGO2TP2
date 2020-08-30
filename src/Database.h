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
#include "Database_join_iterator.h"
#include <map>

using namespace Db::Types;

typedef string_map<string_map<std::map<Datum, linear_set<Record>>>> IndexRef;

class Database
{
public:
  typedef linear_set<Filter> Filters;

  void createTable(const std::string &name, const linear_set<std::string> &keys, const std::vector<std::string> &columns, const std::vector<Datum> &types);
  void addRecord(const Record &record, const std::string &tableName);
  const linear_set<std::string> &tableNames() const;
  const Table &getTable(const std::string &nombre) const;
  int filterUsageCount(const Filters &filter) const;
  bool isValidRecord(const Record &record, const std::string &name) const;
  bool isValidFilter(const Filters &filters, const std::string &name) const;
  Table search(const Filters &filters, const std::string &name);
  linear_set<Filters> mostUsedFilter() const;
  void createIndex(const std::string &table, const std::string &column);
  bool hasIndex(const std::string &table, const std::string &column);
  join_iterator join(const std::string &table1, const std::string &table2, const std::string &columnName);
  join_iterator join_end();

private:
  std::list<Record> &filterRecords(const std::string &column, const Datum &value, std::list<Record> &records, bool equals) const;
  std::pair<std::vector<std::string>, std::vector<Datum>> tableTypes(const Table &t);
  join_iterator join_helper(const std::string &leftTable, const std::string &rightTable, const std::string &joinColumn, const bool &order);
  void updateFilterUsageCount(const Filters &filters);

  linear_set<std::string> mTableNames;
  string_map<Table> mTables;
  linear_map<Filters, int> mFilterUsageCount;
  IndexRef mIndexRefs;

};
