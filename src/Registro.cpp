#include <iostream>
#include "Registro.h"

Registro::Registro(const std::vector<std::string> &campos, const std::vector<Datum> &datos) : _campos(campos.begin(), campos.end())
{
  for (size_t i = 0; i < campos.size(); i++)
  {
    _datos.insert(make_pair(campos[i], datos[i]));
  }
}

const Datum& Registro::dato(const std::string &campo) const
{
  return _datos.at(campo);
}

const linear_set<std::string> &Registro::campos() const
{
  return _campos;
}

const string_map<Datum> &Registro::camposDatos() const
{
  return _datos;
}

bool operator==(const Registro &r1, const Registro &r2)
{
  return r1.camposDatos() == r2.camposDatos();
}

std::ostream &operator<<(std::ostream &os, const Registro &r)
{
  os << "{ ";
  int count = r.campos().size();
  for (const auto& v : r.campos())
  {
    os << r._datos.at(v);
    count--;
    if (count)
    {
      os << ", ";
    }
  }
  os << " }";
  return os;
}
