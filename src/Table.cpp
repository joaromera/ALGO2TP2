#include <algorithm>
#include "Table.h"

using namespace Db::Types;

Table::Table(const linear_set<std::string> &keys, const std::vector<std::string> &columns, const std::vector<Datum> &types)
  : mKeys(keys)
{
  if (columns.size() > types.size())
    throw std::invalid_argument("Each table column must have a Datum type");

  if (keys.size() > types.size())
    throw std::invalid_argument("Each table key must have a Datum type");

  for (size_t i = 0; i < columns.size(); ++i)
  {
    mTypes.fast_insert(std::make_pair(columns[i], types[i]));
    mColumns.fast_insert(columns[i]);
  }
}

Table::const_iterator Table::addRecord(const Record &r)
{
  return Table::const_iterator(linear_set<Record>::const_iterator(mRecords.fast_insert(r)));
}

Table::const_iterator Table::begin() const
{
  return Table::const_iterator(mRecords.begin());
}

Table::const_iterator Table::end() const
{
  return Table::const_iterator(mRecords.end());
}

size_t Table::size() const
{
  return mRecords.size();
}

const linear_set<std::string> &Table::columns() const
{
  return mColumns;
}

const linear_set<std::string> &Table::keys() const
{
  return mKeys;
}

const Datum &Table::columnType(const std::string &column) const
{
  return mTypes.at(column);
}

const linear_set<Record> &Table::records() const
{
  return mRecords;
}

bool operator==(const Table &lhs, const Table &rhs)
{
  auto sameColumnTypes = [&] (const auto &c) {
    return lhs.columnType(c) == rhs.columnType(c);
  };

  return lhs.columns() == rhs.columns() &&
         lhs.keys() == rhs.keys() &&
         std::all_of(lhs.columns().begin(), lhs.columns().end(), sameColumnTypes) &&
         lhs.records() == rhs.records();
}

bool operator!=(const Table &lhs, const Table &rhs)
{
  return !(lhs == rhs);
}
