#pragma once

#include <string>
#include "Datum.h"
#include "linear_map.h"
#include "linear_set.h"
#include "Record.h"
#include "string_map.h"

using namespace Db::Types;

class Table
{
public:
  using const_iterator = linear_set<Record>::const_iterator;

  Table(const linear_set<std::string> &keys,
    const std::vector<std::string> &columns,
    const std::vector<Datum> &types);

  const_iterator addRecord(const Record&);
  const_iterator begin() const;
  const_iterator end() const;
  size_t size() const;
  const linear_set<std::string> &columns() const;
  const Datum &columnType(const std::string &column) const;
  const linear_set<std::string> &keys() const;
  const linear_set<Record> &records() const;

private:
  linear_set<std::string> mKeys;
  linear_set<std::string> mColumns;
  linear_map<std::string, Datum> mTypes;
  linear_set<Record> mRecords;
};

bool operator==(const Table&, const Table&);
bool operator!=(const Table&, const Table&);
