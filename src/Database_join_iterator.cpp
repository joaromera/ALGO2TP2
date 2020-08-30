#include "Database_join_iterator.h"

join_iterator::join_iterator(const join_iterator &n)
{
  if (!n.isFinal)
  {
    it1 = std::make_shared<linear_set<Record>::const_iterator>(*n.it1);
    it2 = std::make_shared<Table::const_iterator>(*n.it2);
    tableRecordCountByKey = n.tableRecordCountByKey;
    tableRecordCount = n.tableRecordCount;
    datumKeys = n.datumKeys;
    campo = n.campo;
    isFinal = n.isFinal;
    orden = n.orden;
  }
}

join_iterator &join_iterator::operator=(const join_iterator &n)
{
  if (!n.isFinal)
  {
    it1 = std::make_shared<linear_set<Record>::const_iterator>(*n.it1);
    it2 = std::make_shared<Table::const_iterator>(*n.it2);
    datumKeys = n.datumKeys;
    campo = n.campo;
    isFinal = n.isFinal;
    tableRecordCount = n.tableRecordCount;
    tableRecordCountByKey = n.tableRecordCountByKey;
    orden = n.orden;
  }
  return *this;
}

join_iterator::join_iterator(linear_set<Record>::const_iterator a,
                             Table::const_iterator c,
                             int ind,
                             int sin,
                             std::shared_ptr<std::map<Datum,
                               linear_set<Record>>> e,
                             const std::string &f,
                             const bool &o
)
  : tableRecordCountByKey(ind)
  , tableRecordCount(sin)
  , datumKeys(e)
  , campo(f)
  , isFinal(false)
  , orden(o)
{
  it1 = std::make_shared<linear_set<Record>::const_iterator>(a);
  it2 = std::make_shared<Table::const_iterator>(c);
}

bool join_iterator::operator==(const join_iterator &j) const
{
  if (isFinal == j.isFinal) return true;

  return it1 == j.it1 &&
         it2 == j.it2 &&
         datumKeys == j.datumKeys &&
         tableRecordCount == j.tableRecordCount &&
         tableRecordCountByKey == j.tableRecordCountByKey &&
         campo == j.campo &&
         orden == j.orden;
}

bool join_iterator::operator!=(const join_iterator &j) const
{
  return !(*this == j);
}

join_iterator &join_iterator::operator++()
{
  incrementIteratorWithIndex();
  if (tableRecordCountByKey == 0)
  {
    incrementIteratorWithoutIndex();
    if (tableRecordCount == 0)
    {
      advanceToEnd();
    }
    else
    {
      auto key = (*it2)->value(campo);
      findNextMatchByDatum(key);
    }
  }
  return *this;
}

join_iterator join_iterator::operator++(int)
{
  auto tmp = *this;
  ++*this;
  return tmp;
}

const Record join_iterator::operator*()
{
  if (orden) return mergeRecords(**it1, **it2);
  return mergeRecords(**it2, **it1);
}

Record join_iterator::mergeRecords(const Record & r1, const Record & r2)
{
  std::vector<std::string> mergedColumns;
  std::vector<Datum> mergedValues;

  for (const auto& c : r1.columns())
  {
    mergedColumns.emplace_back(c);
    mergedValues.emplace_back(r1.value(c));
  }

  for (const auto& c : r2.columns())
  {
    if (r1.columns().count(c) == 0 && c != campo)
    {
      mergedColumns.emplace_back(c);
      mergedValues.emplace_back(r2.value(c));
    }
  }

  return { mergedColumns, mergedValues };
}

void join_iterator::advanceToEnd()
{
  it1.reset();
  it2.reset();
  datumKeys.reset();
  isFinal = true;
  campo = "";
}

void join_iterator::incrementIteratorWithIndex()
{
  ++(*it1);
  tableRecordCountByKey--;
}

void join_iterator::incrementIteratorWithoutIndex()
{
  ++(*it2);
  tableRecordCount--;
}

void join_iterator::findNextMatchByDatum(Datum datum)
{
  while (tableRecordCount != 0 && datumKeys->count(datum) == 0)
  {
    datum = (*it2)->value(campo);
    if (datumKeys->count(datum) == 0) incrementIteratorWithoutIndex();
  }

  if (tableRecordCount == 0)
  {
    advanceToEnd();
  }
  else if (datumKeys->count(datum) != 0)
  {
    setIteratorToNewDatumKey(datum);
  }
  else
  {
    advanceToEnd();
  }
}

void join_iterator::setIteratorToNewDatumKey(const Datum &datum)
{
  it1 = std::make_shared<linear_set<Record>::const_iterator>(datumKeys->at(datum).begin());
  tableRecordCountByKey = datumKeys->at(datum).size();
}
