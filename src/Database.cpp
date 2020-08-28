#include "Database.h"
#include <list>
#include <tuple>
#include <algorithm>

using namespace Db::Types;

Database::Database() {}

void Database::crearTabla(const std::string &nombre,
  const linear_set<std::string> &claves,
  const std::vector<std::string> &campos,
  const std::vector<Datum> &tipos)
{
  _nombres_tablas.fast_insert(nombre);
  _tablas.insert(make_pair(nombre, Table(claves, campos, tipos)));
}

void Database::agregarRegistro(const Record &r, const std::string &nombre)
{
  Table &t = _tablas.at(nombre);
  t.addRecord(r);
  for (const auto& c : r.columns())
  {
    if (tieneIndice(nombre, c))
    {
      if (r.value(c).isInteger())
      {
        _indicesNum[nombre][c][r.value(c).value<int>()].insert(r);
      }
      else
      {
        _indices[nombre][c][r.value(c).value<std::string>()].insert(r);
      }
    }
  }
}

const linear_set<std::string> &Database::tablas() const { return _nombres_tablas; }

const Table &Database::dameTabla(const std::string &nombre) const
{
  return _tablas.at(nombre);
}

int Database::uso_criterio(const Database::Criterio &criterio) const
{
  if (_uso_criterios.count(criterio))
  {
    return _uso_criterios.at(criterio);
  }
  else
  {
    return 0;
  }
}

bool Database::registroValido(const Record &r,
  const std::string &nombre) const
{
  const Table &t = _tablas.at(nombre);
  if (t.columns().size() != r.columns().size()) return false;
  for (const auto& c : r.columnValues())
  {
    if (t.columns().count(c.first) == 0) return false;
    if (t.columnType(c.first).isInteger() != c.second.isInteger()) return false;
  }
  for (const auto& rt : t.records())
  {
    int coincidencias = 0;
    int claves_en_t = t.keys().size();
    for (const auto& c : t.keys())
    {
      if (r.value(c) == rt.value(c)) coincidencias++;
    }
    if (claves_en_t - coincidencias == 0) return false;
  }
  return true;
}

std::list<Record> &
  Database::_filtrar_registros(const std::string &campo, const Datum &valor, std::list<Record> &registros) const
{
  return _filtrar_registros(campo, valor, registros, true);
}

std::list<Record> &Database::_filtrar_registros(const std::string &campo,
  const Datum &valor,
  std::list<Record> &registros,
  bool igualdad) const
{
  auto iter = registros.begin();
  while (iter != registros.end())
  {
    auto now = iter;
    iter++;
    if ((!igualdad) ^ now->value(campo) != valor)
    {
      registros.erase(now);
    }
  }

  return registros;
}

std::pair<std::vector<std::string>, std::vector<Datum>> Database::_tipos_tabla(const Table &t)
{
  std::vector<std::string> res_campos;
  std::vector<Datum> res_tipos;
  for (const auto& c : t.columns())
  {
    res_campos.push_back(c);
    res_tipos.push_back(t.columnType(c));
  }
  return std::make_pair(res_campos, res_tipos);
}

bool Database::criterioValido(const Criterio &c, const std::string &nombre) const
{
  const Table &t = _tablas.at(nombre);
  for (const auto& restriccion : c)
  {
    if (!t.columns().count(restriccion.column())) return false;
    if (t.columnType(restriccion.column()).isInteger() != restriccion.datum().isInteger()) return false;
  }
  return true;
}

Table Database::busqueda(const Database::Criterio &c, const std::string &nombre)
{
  if (_uso_criterios.count(c))
  {
    _uso_criterios.at(c)++;
  }
  else
  {
    _uso_criterios.fast_insert(std::make_pair(c, 1));
  }

  const Table &ref = dameTabla(nombre);
  auto campos_datos = _tipos_tabla(ref);
  Table res(ref.keys(), campos_datos.first, campos_datos.second);
  std::list<Record> regs(ref.records().begin(), ref.records().end());
  for (const auto& restriccion : c)
  {
    _filtrar_registros(restriccion.column(), restriccion.datum(), regs, restriccion.equals());
  }
  for (const auto& r : regs)
  {
    res.addRecord(r);
  }
  return res;
}

linear_set<Database::Criterio> Database::top_criterios() const
{
  linear_set<Criterio> ret;
  int max = 0;
  for (const auto& crit_count : _uso_criterios)
  {
    if (crit_count.second >= max)
    {
      if (crit_count.second > max)
      {
        ret = linear_set<Criterio>();
        max = crit_count.second;
      }
      ret.fast_insert(crit_count.first);
    }
  }
  return ret;
}

void Database::crearIndice(const std::string &tabla, const std::string &campo)
{
  linear_set<Record> reg = dameTabla(tabla).records();
  for (const auto& r : reg)
  {
    if (r.value(campo).isInteger())
    {
      _indicesNum[tabla][campo][r.value(campo).value<int>()].insert(r);
    }
    else
    {
      _indices[tabla][campo][r.value(campo).value<std::string>()].insert(r);
    }
  }
}

bool Database::tieneIndice(const std::string &tabla, const std::string &campo)
{
  return _indices[tabla].count(campo) >= 1 || _indicesNum[tabla].count(campo) >= 1;
}

Database::join_iterator Database::join(const std::string &tabla1, const std::string &tabla2, const std::string &campo)
{
  bool mismoOrden = true;
  bool primeraConIndice = tieneIndice(tabla1, campo);
  if (!primeraConIndice)
  {
    mismoOrden = false;
    if (dameTabla(tabla1).columnType(campo).isInteger())
    {
      return join_helper_int(tabla2, tabla1, campo, mismoOrden);
    }
    else
    {
      return join_helper_str(tabla2, tabla1, campo, mismoOrden);
    }
  }
  if (dameTabla(tabla1).columnType(campo).isInteger())
  {
    return join_helper_int(tabla1, tabla2, campo, mismoOrden);
  }
  else
  {
    return join_helper_str(tabla1, tabla2, campo, mismoOrden);
  }
}

Database::join_iterator Database::join_helper_str(const std::string &tabla1, const std::string &tabla2, const std::string &campo, const bool &orden)
{
  const Table &t2 = dameTabla(tabla2);
  int tipo = 0;
  auto it2 = t2.begin();
  int cant_reg_it2 = t2.size();
  std::string clave = (*it2).value(campo).value<std::string>();

  auto it = _indices[tabla1].at(campo).find(clave);
  auto it_end = _indices[tabla1].at(campo).end();

  // Busca primer coincidencia entre las dos tablas
  while (cant_reg_it2 != 0 && it == it_end)
  {
    clave = (*it2).value(campo).value<std::string>();
    it = _indices[tabla1].at(campo).find(clave);
    it_end = _indices[tabla1].at(campo).end();
    if (it == it_end)
    {
      ++it2;
      cant_reg_it2--;
    }
  }

  if (cant_reg_it2 == 0) return join_iterator();

  auto diccClaves = &_indices[tabla1].at(campo);
  auto it_tabla_con_indice = _indices[tabla1].at(campo).at(clave).begin();
  unsigned long cant_reg_por_indice = _indices[tabla1].at(campo).at(clave).size();
  auto it_tabla_sin_indice = t2.begin();

  return join_iterator(it_tabla_con_indice, cant_reg_por_indice, it_tabla_sin_indice, cant_reg_it2, diccClaves, nullptr, campo, orden, tipo);
}

Database::join_iterator Database::join_helper_int(const std::string &tabla1, const std::string &tabla2, const std::string &campo, const bool &orden)
{
  const Table &t2 = dameTabla(tabla2);
  int tipo = 1;
  auto it2 = t2.begin();
  int cant_reg_it2 = t2.size();
  int clave = (*it2).value(campo).value<int>();

  auto it = _indicesNum[tabla1].at(campo).find(clave)->second.begin();
  auto it_end = _indicesNum[tabla1].at(campo).find(clave)->second.end();

  // Busca primer coincidencia entre las dos tablas
  while (cant_reg_it2 != 0 && it == it_end)
  {
    clave = (*it2).value(campo).value<int>();
    it = _indicesNum[tabla1].at(campo).find(clave)->second.begin();
    it_end = _indicesNum[tabla1].at(campo).find(clave)->second.end();
    if (it == it_end)
    {
      ++it2;
      cant_reg_it2--;
    }
  }

  if (cant_reg_it2 == 0) return join_iterator();

  auto diccClaves = &_indicesNum[tabla1].at(campo);
  auto it_tabla_con_indice = _indicesNum[tabla1].at(campo).at(clave).begin();
  unsigned long cant_reg_por_indice = _indicesNum[tabla1].at(campo).at(clave).size();
  auto it_tabla_sin_indice = t2.begin();

  return join_iterator(it_tabla_con_indice, cant_reg_por_indice, it_tabla_sin_indice, cant_reg_it2, nullptr, diccClaves, campo, orden, tipo);
}

Database::join_iterator Database::join_end()
{
  return join_iterator();
}