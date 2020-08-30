#include <iostream>
#include "Record.h"

using namespace Db::Types;

Record::Record(const std::vector<std::string> &columns,
  const std::vector<Datum> &values)
  : mColumns(columns.begin(), columns.end())
{
  for (size_t i = 0; i < columns.size(); i++)
    mValues.insert(std::make_pair(columns[i], values[i]));
}

const Datum&Record::value(const std::string &column) const
{
  return mValues.at(column);
}

const linear_set<std::string> &Record::columns() const
{
  return mColumns;
}

const string_map<Datum> &Record::columnValues() const
{
  return mValues;
}

bool operator==(const Record &r1, const Record &r2)
{
  return r1.columnValues() == r2.columnValues();
}

std::ostream &operator<<(std::ostream &os, const Record &r)
{
  os << "{ ";
  int count = r.columns().size();
  for (const auto& v : r.columns())
  {
    os << r.mValues.at(v);
    count--;
    if (count)
    {
      os << ", ";
    }
  }
  os << " }";
  return os;
}
