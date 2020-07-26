#include "Table.h"

using namespace Db::Types;

Table::Table(const linear_set<std::string> &keys, const std::vector<std::string> &columns, const std::vector<Datum> &types)
  : _keys(keys)
{
  for (size_t i = 0; i < columns.size(); ++i)
  {
    _types.fast_insert(std::make_pair(columns[i], types[i]));
    _columns.fast_insert(columns[i]);
  }
}

Table::const_iterator Table::addRecord(const Record &r)
{
  return Table::const_iterator(linear_set<Record>::const_iterator(_records.fast_insert(r)));
}

const linear_set<std::string> &Table::columns() const
{
  return _columns;
}

const linear_set<std::string> &Table::keys() const
{
  return _keys;
}

const Datum &Table::columnType(const std::string &column) const
{
  return _types.at(column);
}

const linear_set<Record> &Table::records() const
{
  return _records;
}

Table::const_iterator Table::begin() const
{
  return Table::const_iterator(_records.begin());
}

Table::const_iterator Table::end() const
{
  return Table::const_iterator(_records.end());
}

size_t Table::size() const
{
  return _records.size();
}

Table::const_iterator::const_iterator(const const_iterator &o_it) : record_iterator(o_it.record_iterator) {}

const Record &Table::const_iterator::operator*() const
{
  return *record_iterator;
}

const Record *Table::const_iterator::operator->() const
{
  return &(*record_iterator);
}

Table::const_iterator &Table::const_iterator::operator++()
{
  ++record_iterator;
  return *this;
}

bool Table::const_iterator::operator==(const Table::const_iterator &it) const
{
  return record_iterator == it.record_iterator;
}

bool Table::const_iterator::operator!=(const Table::const_iterator &it) const
{
  return not(record_iterator == it.record_iterator);
}

Table::const_iterator::const_iterator(const linear_set<Record>::const_iterator record_it)
  : record_iterator(record_it)
{
}

bool operator==(const Table &lhs, const Table &rhs)
{
  if (lhs.columns() != rhs.columns())
  {
    return false;
  }
  else if (lhs.keys() != rhs.keys())
  {
    return false;
  }
  else
  {
    for (const auto& c : lhs.columns())
    {
      if (lhs.columnType(c).esNat() != rhs.columnType(c).esNat()) { return false; }
    }
  }
  return lhs.records() == rhs.records();
}

bool operator!=(const Table &lhs, const Table &rhs)
{
  return !(lhs == rhs);
}
