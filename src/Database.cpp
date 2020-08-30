#include "Database.h"
#include <list>
#include <tuple>
#include <algorithm>

using namespace Db::Types;

void Database::createTable(const std::string &name,
  const linear_set<std::string> &keys,
  const std::vector<std::string> &columns,
  const std::vector<Datum> &types)
{
  mTableNames.fast_insert(name);
  mTables.insert(make_pair(name, Table(keys, columns, types)));
}

void Database::addRecord(const Record &record, const std::string &tableName)
{
  Table &t = mTables.at(tableName);
  t.addRecord(record);
  for (const auto& c : record.columns())
  {
    if (hasIndex(tableName, c))
    {
      mIndexRefs[tableName][c][record.value(c)].insert(record);
    }
  }
}

const linear_set<std::string> &Database::tableNames() const
{
  return mTableNames;
}

const Table &Database::getTable(const std::string &name) const
{
  return mTables.at(name);
}

int Database::filterUsageCount(const Database::Filters &filter) const
{
  if (mFilterUsageCount.count(filter)) return mFilterUsageCount.at(filter);

  return 0;
}

bool Database::isValidRecord(const Record &record, const std::string &name) const
{
  const Table &t = mTables.at(name);

  if (t.columns().size() != record.columns().size())
    return false;

  for (const auto& c : record.columnValues())
  {
    if (t.columns().count(c.first) == 0)
      return false;

    if (t.columnType(c.first).isInteger() != c.second.isInteger())
      return false;
  }

  for (const auto& rt : t.records())
  {
    int coincidencias = 0;
    int claves_en_t = t.keys().size();

    for (const auto& c : t.keys())
    {
      if (record.value(c) == rt.value(c))
        coincidencias++;
    }

    if (claves_en_t - coincidencias == 0)
      return false;
  }

  return true;
}

std::list<Record> &Database::filterRecords(
  const std::string &column,
  const Datum &value,
  std::list<Record> &records,
  bool equals) const
{
  auto iter = records.begin();

  while (iter != records.end())
  {
    auto now = iter++;
    const bool excludeNonMatch = (now->value(column) != value) && equals;
    const bool excludeMatch = (now->value(column) == value) && !equals;

    if (excludeNonMatch || excludeMatch)
    {
      records.erase(now);
    }
  }

  return records;
}

std::pair<std::vector<std::string>, std::vector<Datum>>Database::tableTypes(const Table &t)
{
  std::vector<std::string> columns;
  std::vector<Datum> types;
  for (const auto& c : t.columns())
  {
    columns.push_back(c);
    types.push_back(t.columnType(c));
  }
  return { columns, types };
}

bool Database::isValidFilter(const Filters &filters, const std::string &name) const
{
  const Table &t = mTables.at(name);
  for (const auto &filter : filters)
  {
    const auto &column = filter.column();
    const auto &datum = filter.datum();

    if (!t.columns().count(column))
      return false;

    if (t.columnType(column).isInteger() != datum.isInteger())
      return false;
  }
  return true;
}

Table Database::search(const Database::Filters &filters, const std::string &name)
{
  updateFilterUsageCount(filters);

  const Table &tableToFilter = getTable(name);
  auto filterTableTypes = tableTypes(tableToFilter);
  Table res(tableToFilter.keys(), filterTableTypes.first, filterTableTypes.second);

  std::list<Record> records(tableToFilter.records().begin(), tableToFilter.records().end());

  for (const auto& filter : filters)
  {
    filterRecords(filter.column(), filter.datum(), records, filter.equals());
  }

  for (const auto& r : records)
  {
    res.addRecord(r);
  }

  return res;
}
void Database::updateFilterUsageCount(const Database::Filters &filters)
{
  if (mFilterUsageCount.count(filters))
  {
    mFilterUsageCount.at(filters)++;
  }
  else
  {
    mFilterUsageCount.fast_insert({ filters, 1 });
  }
}

linear_set<Database::Filters> Database::mostUsedFilter() const
{
  linear_set<Filters> ret;
  int max = 0;
  for (const auto& crit_count : mFilterUsageCount)
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
    mIndexRefs[table][column][r.value(column)].insert(r);
  }
}

bool Database::hasIndex(const std::string &table, const std::string &column)
{
  return mIndexRefs[table].count(column) >= 1;
}

join_iterator Database::join(const std::string &table1, const std::string &table2, const std::string &columnName)
{
  const bool firstTableHasIndex = hasIndex(table1, columnName);
  const auto &indexedTable = firstTableHasIndex ? table1 : table2;
  const auto &nonIndexedTable = firstTableHasIndex ? table2 : table1;

  return join_helper(indexedTable, nonIndexedTable, columnName, firstTableHasIndex);
}

join_iterator Database::join_helper(const std::string &leftTable, const std::string &rightTable, const std::string &joinValue, const bool &order)
{
  const Table &t2 = getTable(rightTable);
  auto it2 = t2.begin();
  int cant_reg_it2 = t2.size();
  auto clave = (*it2).value(joinValue);

  auto it = mIndexRefs[leftTable].at(joinValue).find(clave)->second.begin();
  auto it_end = mIndexRefs[leftTable].at(joinValue).find(clave)->second.end();

  // Busca primer coincidencia entre las dos tableNames
  while (cant_reg_it2 != 0 && it == it_end)
  {
    clave = (*it2).value(joinValue);
    it = mIndexRefs[leftTable].at(joinValue).find(clave)->second.begin();
    it_end = mIndexRefs[leftTable].at(joinValue).find(clave)->second.end();
    if (it == it_end)
    {
      ++it2;
      cant_reg_it2--;
    }
  }

  if (cant_reg_it2 == 0) return join_iterator();

  auto diccClaves = std::make_shared<std::map<Datum, linear_set<Record>>>(mIndexRefs[leftTable].at(joinValue));
  auto &tab = mIndexRefs[leftTable];
  auto &col = tab.at(joinValue);

  if (col.find(clave) == col.end()) return join_iterator();

  auto &dat = col.at(clave);

  auto it_tabla_con_indice = dat.begin();
  unsigned long cant_reg_por_indice = mIndexRefs[leftTable].at(joinValue).at(clave).size();
  auto it_tabla_sin_indice = t2.begin();

  return join_iterator(it_tabla_con_indice, it_tabla_sin_indice, cant_reg_por_indice, cant_reg_it2, diccClaves, joinValue, order);
}

join_iterator Database::join_end()
{
  return join_iterator();
}
