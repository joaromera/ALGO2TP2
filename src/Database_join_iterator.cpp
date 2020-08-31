#include "Database_join_iterator.h"

join_iterator::join_iterator(const join_iterator &n)
{
  if (!n.mIsFinal)
  {
    mIterLeft = std::make_shared<linear_set<Record>::const_iterator>(*n.mIterLeft);
    mIterRight = std::make_shared<Table::const_iterator>(*n.mIterRight);
    mTableRecordCountByKey = n.mTableRecordCountByKey;
    mTableRecordCount = n.mTableRecordCount;
    mDatumKeys = n.mDatumKeys;
    mValue = n.mValue;
    mIsFinal = n.mIsFinal;
    mOrder = n.mOrder;
  }
}

join_iterator &join_iterator::operator=(const join_iterator &n)
{
  if (!n.mIsFinal)
  {
    mIterLeft = std::make_shared<linear_set<Record>::const_iterator>(*n.mIterLeft);
    mIterRight = std::make_shared<Table::const_iterator>(*n.mIterRight);
    mDatumKeys = n.mDatumKeys;
    mValue = n.mValue;
    mIsFinal = n.mIsFinal;
    mTableRecordCount = n.mTableRecordCount;
    mTableRecordCountByKey = n.mTableRecordCountByKey;
    mOrder = n.mOrder;
  }
  return *this;
}

join_iterator::join_iterator(
  const linear_set<Record> &a,
  const Table &c,
  int ind,
  int sin,
  std::shared_ptr<std::map<Datum, linear_set<Record>>> e,
  const std::string &f,
  const bool &o)
  : mTableRecordCountByKey(ind)
  , mTableRecordCount(sin)
  , mDatumKeys(e)
  , mValue(f)
  , mIsFinal(false)
  , mOrder(o)
{
  mIterLeft = std::make_shared<linear_set<Record>::const_iterator>(a.begin());
  mIterRight = std::make_shared<Table::const_iterator>(c.begin());
}

bool join_iterator::operator==(const join_iterator &j) const
{
  if (mIsFinal == j.mIsFinal) return true;

  return mIterLeft == j.mIterLeft &&
         mIterRight == j.mIterRight &&
         mDatumKeys == j.mDatumKeys &&
         mTableRecordCount == j.mTableRecordCount &&
         mTableRecordCountByKey == j.mTableRecordCountByKey &&
         mValue == j.mValue &&
         mOrder == j.mOrder;
}

bool join_iterator::operator!=(const join_iterator &j) const
{
  return !(*this == j);
}

join_iterator &join_iterator::operator++()
{
  incrementIteratorWithIndex();
  if (mTableRecordCountByKey == 0)
  {
    incrementIteratorWithoutIndex();
    if (mTableRecordCount == 0)
    {
      advanceToEnd();
    }
    else
    {
      auto key = (*mIterRight)->value(mValue);
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
  if (mOrder) return mergeRecords(**mIterLeft, **mIterRight);
  return mergeRecords(**mIterRight, **mIterLeft);
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
    if (r1.columns().count(c) == 0 && c != mValue)
    {
      mergedColumns.emplace_back(c);
      mergedValues.emplace_back(r2.value(c));
    }
  }

  return { mergedColumns, mergedValues };
}

void join_iterator::advanceToEnd()
{
  mIterLeft.reset();
  mIterRight.reset();
  mDatumKeys.reset();
  mIsFinal = true;
  mValue = "";
}

void join_iterator::incrementIteratorWithIndex()
{
  ++(*mIterLeft);
  mTableRecordCountByKey--;
}

void join_iterator::incrementIteratorWithoutIndex()
{
  ++(*mIterRight);
  mTableRecordCount--;
}

void join_iterator::findNextMatchByDatum(Datum datum)
{
  while (mTableRecordCount != 0 && mDatumKeys->count(datum) == 0)
  {
    datum = (*mIterRight)->value(mValue);
    if (mDatumKeys->count(datum) == 0) incrementIteratorWithoutIndex();
  }

  if (mTableRecordCount == 0)
  {
    advanceToEnd();
  }
  else if (mDatumKeys->count(datum) != 0)
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
  const auto &iter = mDatumKeys->at(datum).begin();
  mIterLeft = std::make_shared<linear_set<Record>::const_iterator>(iter);
  mTableRecordCountByKey = mDatumKeys->at(datum).size();
}
