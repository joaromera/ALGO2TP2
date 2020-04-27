#include "Tabla.h"

using namespace std;

Tabla::Tabla(const linear_set<string> &claves, const vector<string> &campos, const vector<Datum> &tipos) : _claves(claves)
{
  for (size_t i = 0; i < campos.size(); i++)
  {
    _tipos.fast_insert(make_pair(campos[i], tipos[i]));
    _campos.fast_insert(campos[i]);
  }
}

Tabla::const_iterador_registros Tabla::agregarRegistro(const Registro &r)
{
  return Tabla::const_iterador_registros(linear_set<Registro>::const_iterator(_registros.fast_insert(r)));
}

const linear_set<string> &Tabla::campos() const
{
  return _campos;
}

const linear_set<string> &Tabla::claves() const
{
  return _claves;
}

const Datum &Tabla::tipoCampo(const string &campo) const
{
  return _tipos.at(campo);
}

const linear_set<Registro> &Tabla::registros() const
{
  return _registros;
}

Tabla::const_iterador_registros Tabla::registros_begin() const
{
  return Tabla::const_iterador_registros(_registros.begin());
}

Tabla::const_iterador_registros Tabla::registros_end() const
{
  return Tabla::const_iterador_registros(_registros.end());
}

int Tabla::cant_registros() const
{
  return _registros.size();
}

bool operator==(const Tabla &t1, const Tabla &t2)
{
  if (not(t1.campos() == t2.campos()))
  {
    return false;
  }
  else if (not(t1.claves() == t2.claves()))
  {
    return false;
  }
  else
  {
    linear_set<string> cs = t1.campos();
    for (auto c : cs)
    {
      if (t1.tipoCampo(c).esNat() != t2.tipoCampo(c).esNat()) return false;
    }
    return t1.registros() == t2.registros();
  }
}

bool operator!=(const Tabla &t1, const Tabla &t2)
{
  return not(t1 == t2);
}
