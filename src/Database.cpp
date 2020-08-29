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

void Database::addRecord(const Record &record, const std::string &tableName)
{
  Table &t = _tables.at(tableName);
  t.addRecord(record);
  for (const auto& c : record.columns())
  {
    if (hasIndex(tableName, c))
    {
      _indexRefs[tableName][c][record.value(c)].insert(record);
    }
  }
}

const linear_set<std::string> &Database::tableNames() const { return _tableNames; }

const Table &Database::getTable(const std::string &name) const
{
  return _tables.at(name);
}

int Database::filterUsageCount(const Database::Filters &filter) const
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

std::pair<std::vector<std::string>, std::vector<Datum>> Database::_tableTypes(const Table &t)
{
  std::vector<std::string> res_campos;
  std::vector<Datum> res_tipos;
  for (const auto& c : t.columns())
  {
    res_campos.push_back(c);
    res_tipos.push_back(t.columnType(c));
  }
  return { res_campos, res_tipos };
}

bool Database::isValidFilter(const Filters &c, const std::string &nombre) const
{
  const Table &t = _tables.at(nombre);
  for (const auto& restriccion : c)
  {
    if (!t.columns().count(restriccion.column())) return false;
    if (t.columnType(restriccion.column()).isInteger() != restriccion.datum().isInteger()) return false;
  }
  return true;
}

Table Database::search(const Database::Filters &c, const std::string &nombre)
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
  auto campos_datos = _tableTypes(ref);
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

linear_set<Database::Filters> Database::mostUsedFilter() const
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
  const linear_set<Record> &reg = getTable(table).records();

  for (const auto& r : reg)
  {
    _indexRefs[table][column][r.value(column)].insert(r);
  }
}

bool Database::hasIndex(const std::string &table, const std::string &column)
{
  return _indexRefs[table].count(column) >= 1;
}

Database::join_iterator Database::join(const std::string &table1, const std::string &table2, const std::string &columnName)
{
  const bool firstTableHasIndex = hasIndex(table1, columnName);
  const auto &indexedTable = firstTableHasIndex ? table1 : table2;
  const auto &nonIndexedTable = firstTableHasIndex ? table2 : table1;

  return join_helper(indexedTable, nonIndexedTable, columnName, firstTableHasIndex);
}

Database::join_iterator Database::join_helper(const std::string &tabla1, const std::string &tabla2, const std::string &campo, const bool &orden)
{
  const Table &t2 = getTable(tabla2);
  auto it2 = t2.begin();
  int cant_reg_it2 = t2.size();
  auto clave = (*it2).value(campo);

  auto it = _indexRefs[tabla1].at(campo).find(clave)->second.begin();
  auto it_end = _indexRefs[tabla1].at(campo).find(clave)->second.end();

  // Busca primer coincidencia entre las dos tableNames
  while (cant_reg_it2 != 0 && it == it_end)
  {
    clave = (*it2).value(campo);
    it = _indexRefs[tabla1].at(campo).find(clave)->second.begin();
    it_end = _indexRefs[tabla1].at(campo).find(clave)->second.end();
    if (it == it_end)
    {
      ++it2;
      cant_reg_it2--;
    }
  }

  if (cant_reg_it2 == 0) return join_iterator();

  auto diccClaves = std::make_shared<std::map<Datum, linear_set<Record>>>(_indexRefs[tabla1].at(campo));
  auto &tab = _indexRefs[tabla1];
  auto &col = tab.at(campo);

  if (col.find(clave) == col.end()) return join_iterator();

  auto &dat = col.at(clave);

  auto it_tabla_con_indice = dat.begin();
  unsigned long cant_reg_por_indice = _indexRefs[tabla1].at(campo).at(clave).size();
  auto it_tabla_sin_indice = t2.begin();

  return join_iterator(it_tabla_con_indice, it_tabla_sin_indice, cant_reg_por_indice, cant_reg_it2, diccClaves, campo, orden);
}

Database::join_iterator Database::join_end()
{
  return join_iterator();
}
