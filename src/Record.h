#pragma once
#include <vector>
#include <iostream>
#include "Datum.h"
#include "linear_set.h"
#include "linear_map.h"
#include "string_map.h"

using namespace Db::Types;

class Record
{
public:
  Record(const std::vector<std::string> &columns, const std::vector<Datum> &values);

  const Datum &value(const std::string &column) const;
  const linear_set<std::string> &columns() const;
  const string_map<Datum> &columnValues() const;

private:
  linear_set<std::string> _columns;
  string_map<Datum> _values;
  friend std::ostream &operator<<(std::ostream &, const Record &);
};

bool operator==(const Record &lhs, const Record &rhs);
std::ostream &operator<<(std::ostream &, const Record &);
