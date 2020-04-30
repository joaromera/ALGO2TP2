#pragma once

#include <string>

#include "Datum.h"
#include "linear_map.h"
#include "linear_set.h"
#include "Registro.h"
#include "string_map.h"

class Table
{
public:
  class const_iterador_registros
  {
  public:
    const_iterador_registros(const const_iterador_registros &o_it);
    const Registro &operator*() const;
    const Registro *operator->() const;
    const_iterador_registros &operator++();

    bool operator==(const const_iterador_registros &o_it) const;
    bool operator!=(const const_iterador_registros &o_it) const;

  private:
    friend class Table;

    const_iterador_registros(const linear_set<Registro>::const_iterator);

    linear_set<Registro>::const_iterator it_registro;
  };

  Table(const linear_set<std::string> &claves, const std::vector<std::string> &campos, const std::vector<Datum> &tipos);
  const_iterador_registros agregarRegistro(const Registro &r);
  const linear_set<std::string> &campos() const;
  const Datum &tipoCampo(const std::string &campo) const;
  const linear_set<std::string> &claves() const;
  const linear_set<Registro> &registros() const;
  int cant_registros() const;
  const_iterador_registros registros_begin() const;
  const_iterador_registros registros_end() const;

private:
  linear_set<std::string> _claves;
  linear_set<std::string> _campos;
  linear_map<std::string, Datum> _tipos;
  linear_set<Registro> _registros;
};

bool operator==(const Table &, const Table &);

bool operator!=(const Table &, const Table &);
