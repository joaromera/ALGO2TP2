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
  class const_iterator
  {
  public:
    const_iterator(const const_iterator&);
    const Record &operator*() const;
    const Record *operator->() const;
    const_iterator &operator++();
    const_iterator operator++(int);

    bool operator==(const const_iterator&) const;
    bool operator!=(const const_iterator&) const;

  private:
    friend class Table;

    const_iterator(const linear_set<Record>::const_iterator);

    linear_set<Record>::const_iterator record_iterator;
  };

  Table(const linear_set<std::string> &keys, const std::vector<std::string> &columns, const std::vector<Datum> &types);
  const_iterator addRecord(const Record&);
  const linear_set<std::string> &columns() const;
  const Datum &columnType(const std::string &column) const;
  const linear_set<std::string> &keys() const;
  const linear_set<Record> &records() const;
  size_t size() const;
  const_iterator begin() const;
  const_iterator end() const;

private:
  linear_set<std::string> _keys;
  linear_set<std::string> _columns;
  linear_map<std::string, Datum> _types;
  linear_set<Record> _records;
};

bool operator==(const Table&, const Table&);
bool operator!=(const Table&, const Table&);
