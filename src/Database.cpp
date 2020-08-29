#include "Database.h"
#include <list>
#include <tuple>
#include <algorithm>

using namespace Db::Types;

Database::Database() {}

void Database::createTable(const std::string &name, const linear_set<std::string> &keys, const std::vector<std::string> &columns, const std::vector<Datum> &types)
{
  _tableNames.fast_insert(name);
  _tables.insert(make_pair(name, Table(keys, columns, types)));
}

void Database::addRecord(const Record &record, const std::string &name)
{
  Table &t = _tables.at(name);
  t.addRecord(record);
  for (const auto& c : record.columns())
  {
    if (hasIndex(name, c))
    {
      if (record.value(c).isInteger())
      {
        _indexesInt[name][c][record.value(c).value<int>()].insert(record);
      }
      else
      {
        _indexes[name][c][record.value(c).value<std::string>()].insert(record);
      }
    }
  }
}

const linear_set<std::string> &Database::tableNames() const { return _tableNames; }

const Table &Database::getTable(const std::string &name) const
{
  return _tables.at(name);
}

int Database::useFilter(const Database::Filters &filter) const
{
  if (_uso_criterios.count(filter)) return _uso_criterios.at(filter);

  return 0;
}

bool Database::isValidRecord(const Record &record, const std::string &name) const
{
  const Table &t = _tables.at(name);

  if (t.columns().size() != record.columns().size()) return false;

  for (const auto& c : record.columnValues())
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
      if (record.value(c) == rt.value(c)) coincidencias++;
    }

    if (claves_en_t - coincidencias == 0) return false;
  }

  return true;
}

std::list<Record> &Database::_filterRecords(const std::string &column, const Datum &value, std::list<Record> &records) const
{
  return _filterRecords(column, value, records, true);
}

std::list<Record> &Database::_filterRecords(const std::string &column, const Datum &value, std::list<Record> &records, bool equals) const
{
  auto iter = records.begin();

  while (iter != records.end())
  {
    auto now = iter++;
    if (!equals ^ (now->value(column) != value)) records.erase(now);
  }

  return records;
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

bool Database::criterioValido(const Filters &c, const std::string &nombre) const
{
  const Table &t = _tables.at(nombre);
  for (const auto& restriccion : c)
  {
    if (!t.columns().count(restriccion.column())) return false;
    if (t.columnType(restriccion.column()).isInteger() != restriccion.datum().isInteger()) return false;
  }
  return true;
}

Table Database::busqueda(const Database::Filters &c, const std::string &nombre)
{
  if (_uso_criterios.count(c))
  {
    _uso_criterios.at(c)++;
  }
  else
  {
    _uso_criterios.fast_insert(std::make_pair(c, 1));
  }

  const Table &ref = getTable(nombre);
  auto campos_datos = _tipos_tabla(ref);
  Table res(ref.keys(), campos_datos.first, campos_datos.second);
  std::list<Record> regs(ref.records().begin(), ref.records().end());
  for (const auto& restriccion : c)
  {
    _filterRecords(restriccion.column(), restriccion.datum(), regs, restriccion.equals());
  }
  for (const auto& r : regs)
  {
    res.addRecord(r);
  }
  return res;
}

linear_set<Database::Filters> Database::top_criterios() const
{
  linear_set<Filters> ret;
  int max = 0;
  for (const auto& crit_count : _uso_criterios)
  {
    if (crit_count.second >= max)
    {
      if (crit_count.second > max)
      {
        ret = linear_set<Filters>();
        max = crit_count.second;
      }
      ret.fast_insert(crit_count.first);
    }
  }
  return ret;
}

void Database::createIndex(const std::string &table, const std::string &column)
{
  linear_set<Record> reg = getTable(table).records();
  for (const auto& r : reg)
  {
    if (r.value(column).isInteger())
    {
      _indexesInt[table][column][r.value(column).value<int>()].insert(r);
    }
    else
    {
      _indexes[table][column][r.value(column).value<std::string>()].insert(r);
    }
  }
}

bool Database::hasIndex(const std::string &table, const std::string &column)
{
  return _indexes[table].count(column) >= 1 || _indexesInt[table].count(column) >= 1;
}

Database::join_iterator Database::join(const std::string &tabla1, const std::string &tabla2, const std::string &campo)
{
  bool mismoOrden = true;
  bool primeraConIndice = hasIndex(tabla1, campo);

  if (!primeraConIndice)
  {
    mismoOrden = false;
    if (getTable(tabla1).columnType(campo).isInteger())
    {
      return join_helper_int(tabla2, tabla1, campo, mismoOrden);
    }
    else
    {
      return join_helper_str(tabla2, tabla1, campo, mismoOrden);
    }
  }

  if (getTable(tabla1).columnType(campo).isInteger())
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
  const Table &t2 = getTable(tabla2);
  int tipo = 0;
  auto it2 = t2.begin();
  int cant_reg_it2 = t2.size();
  std::string clave = (*it2).value(campo).value<std::string>();

  auto it = _indexes[tabla1].at(campo).find(clave);
  auto it_end = _indexes[tabla1].at(campo).end();

  // Busca primer coincidencia entre las dos tableNames
  while (cant_reg_it2 != 0 && it == it_end)
  {
    clave = (*it2).value(campo).value<std::string>();
    it = _indexes[tabla1].at(campo).find(clave);
    it_end = _indexes[tabla1].at(campo).end();
    if (it == it_end)
    {
      ++it2;
      cant_reg_it2--;
    }
  }

  if (cant_reg_it2 == 0) return join_iterator();

  auto diccClaves = std::make_shared<string_map<linear_set<Record>>>(_indexes[tabla1].at(campo));
  auto it_tabla_con_indice = _indexes[tabla1].at(campo).at(clave).begin();
  unsigned long cant_reg_por_indice = _indexes[tabla1].at(campo).at(clave).size();
  auto it_tabla_sin_indice = t2.begin();

  return join_iterator(it_tabla_con_indice, it_tabla_sin_indice, cant_reg_por_indice, cant_reg_it2, diccClaves, nullptr, campo, orden, tipo);
}

Database::join_iterator Database::join_helper_int(const std::string &tabla1, const std::string &tabla2, const std::string &campo, const bool &orden)
{
  const Table &t2 = getTable(tabla2);
  int tipo = 1;
  auto it2 = t2.begin();
  int cant_reg_it2 = t2.size();
  int clave = (*it2).value(campo).value<int>();

  auto it = _indexesInt[tabla1].at(campo).find(clave)->second.begin();
  auto it_end = _indexesInt[tabla1].at(campo).find(clave)->second.end();

  // Busca primer coincidencia entre las dos tableNames
  while (cant_reg_it2 != 0 && it == it_end)
  {
    clave = (*it2).value(campo).value<int>();
    it = _indexesInt[tabla1].at(campo).find(clave)->second.begin();
    it_end = _indexesInt[tabla1].at(campo).find(clave)->second.end();
    if (it == it_end)
    {
      ++it2;
      cant_reg_it2--;
    }
  }

  if (cant_reg_it2 == 0) return join_iterator();

  auto diccClaves = std::make_shared<std::map<int, linear_set<Record>>>(_indexesInt[tabla1].at(campo));
  auto it_tabla_con_indice = _indexesInt[tabla1].at(campo).at(clave).begin();
  unsigned long cant_reg_por_indice = _indexesInt[tabla1].at(campo).at(clave).size();
  auto it_tabla_sin_indice = t2.begin();

  return join_iterator(it_tabla_con_indice, it_tabla_sin_indice, cant_reg_por_indice, cant_reg_it2, nullptr, diccClaves, campo, orden, tipo);
}

Database::join_iterator Database::join_end()
{
  return join_iterator();
}
