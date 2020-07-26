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

    bool operator==(const const_iterator&) const;
    bool operator!=(const const_iterator&) const;

  private:
    friend class Table;

    const_iterator(const linear_set<Record>::const_iterator);

    linear_set<Record>::const_iterator it_registro;
  };

  Table(const linear_set<std::string> &claves, const std::vector<std::string> &campos, const std::vector<Datum> &tipos);
  const_iterator agregarRegistro(const Record &r);
  const linear_set<std::string> &campos() const;
  const Datum &tipoCampo(const std::string &campo) const;
  const linear_set<std::string> &claves() const;
  const linear_set<Record> &registros() const;
  int cant_registros() const;
  const_iterator registros_begin() const;
  const_iterator registros_end() const;

private:
  linear_set<std::string> _claves;
  linear_set<std::string> _campos;
  linear_map<std::string, Datum> _tipos;
  linear_set<Record> _registros;
};

bool operator==(const Table &, const Table &);

bool operator!=(const Table &, const Table &);
