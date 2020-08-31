#pragma once

#include <list>
#include <map>
#include <string>
#include <utility>

#include "Database_defines.h"
#include "Database_join_iterator.h"
#include "Filter.h"
#include "linear_map.h"
#include "linear_set.h"
#include "Record.h"
#include "string_map.h"
#include "Table.h"

using namespace Db::Types;

typedef string_map<DatumToRecordsStringMap> IndexRef;

class Database
{
public:
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
  join_iterator join_end() const;

private:
  Records &filterRecords(const std::string &column, const Datum &value, Records &records, bool equals) const;
  std::pair<std::vector<std::string>, std::vector<Datum>> tableTypes(const Table &t) const;
  join_iterator join_helper(const std::string &leftTable, const std::string &rightTable, const std::string &joinColumn, const bool &order);
  void updateFilterUsageCount(const Filters &filters);

  linear_set<std::string> mTableNames;
  string_map<Table> mTables;
  linear_map<Filters, int> mFilterUsageCount;
  IndexRef mIndexRefs;
};
