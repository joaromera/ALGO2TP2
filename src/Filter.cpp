#include "Filter.h"
#include <tuple>

Filter::Filter(const std::string &column, const Datum &datum, bool equals)
  : mColumn(column)
  , mDatum(datum)
  , mEquals(equals)
{
}

const std::string &Filter::column() const
{
  return mColumn;
}

const Datum &Filter::datum() const
{
  return mDatum;
}

const bool &Filter::equals() const
{
  return mEquals;
}

bool operator==(const Filter &lhs, const Filter &rhs)
{
  return (lhs.column() == rhs.column() and lhs.datum() == rhs.datum() and lhs.equals() == rhs.equals());
}

bool operator<(const Filter &lhs, const Filter &rhs)
{
  return std::make_tuple(lhs.column(), lhs.datum(), lhs.equals()) < std::make_tuple(rhs.column(), rhs.datum(), rhs.equals());
}

Filter Rig(const std::string &column, const std::string &value)
{
  return Filter(column, Datum(value), true);
}

Filter Rig(const std::string &column, const int &value)
{
  return Filter(column, Datum(value), true);
}

Filter Rdif(const std::string &column, const std::string &value)
{
  return Filter(column, Datum(value), false);
}

Filter Rdif(const std::string &column, const int &value)
{
  return Filter(column, Datum(value), false);
}
