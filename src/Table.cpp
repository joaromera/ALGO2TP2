#include "Table.h"

Table::Table(const linear_set<std::string> &claves, const std::vector<std::string> &campos, const std::vector<Datum> &tipos) : _claves(claves)
{
  for (size_t i = 0; i < campos.size(); i++)
  {
    _tipos.fast_insert(std::make_pair(campos[i], tipos[i]));
    _campos.fast_insert(campos[i]);
  }
}

Table::const_iterador_registros Table::agregarRegistro(const Registro &r)
{
  return Table::const_iterador_registros(linear_set<Registro>::const_iterator(_registros.fast_insert(r)));
}

const linear_set<std::string> &Table::campos() const
{
  return _campos;
}

const linear_set<std::string> &Table::claves() const
{
  return _claves;
}

const Datum &Table::tipoCampo(const std::string &campo) const
{
  return _tipos.at(campo);
}

const linear_set<Registro> &Table::registros() const
{
  return _registros;
}

Table::const_iterador_registros Table::registros_begin() const
{
  return Table::const_iterador_registros(_registros.begin());
}

Table::const_iterador_registros Table::registros_end() const
{
  return Table::const_iterador_registros(_registros.end());
}

int Table::cant_registros() const
{
  return _registros.size();
}

Table::const_iterador_registros::const_iterador_registros(const const_iterador_registros &o_it) : it_registro(o_it.it_registro) {}

const Registro &Table::const_iterador_registros::operator*() const
{
  return *it_registro;
}

const Registro *Table::const_iterador_registros::operator->() const
{
  return &(*it_registro);
}

Table::const_iterador_registros &Table::const_iterador_registros::operator++()
{
  ++it_registro;
  return *this;
}

bool Table::const_iterador_registros::operator==(const Table::const_iterador_registros &o_it) const
{
  return it_registro == o_it.it_registro;
}

bool Table::const_iterador_registros::operator!=(const Table::const_iterador_registros &o_it) const
{
  return not(it_registro == o_it.it_registro);
}

Table::const_iterador_registros::const_iterador_registros(const linear_set<Registro>::const_iterator _it_registro) : it_registro(_it_registro) {}

bool operator==(const Table &t1, const Table &t2)
{
  if (t1.campos() != t2.campos())
  {
    return false;
  }
  else if (t1.claves() != t2.claves())
  {
    return false;
  }
  else
  {
    for (const auto& c : t1.campos())
    {
      if (t1.tipoCampo(c).esNat() != t2.tipoCampo(c).esNat()) { return false; }
    }
    return t1.registros() == t2.registros();
  }
}

bool operator!=(const Table &t1, const Table &t2)
{
  return !(t1 == t2);
}
