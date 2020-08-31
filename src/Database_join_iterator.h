#pragma once
#include "Record.h"
#include "Table.h"

class join_iterator
{
public:
  join_iterator(const join_iterator &n);
  join_iterator &operator=(const join_iterator &n);
  ~join_iterator() = default;

  join_iterator &operator++();
  join_iterator operator++(int);
  const Record operator*();

  bool operator==(const join_iterator &j) const;
  bool operator!=(const join_iterator &j) const;

private:
  join_iterator() = default;
  join_iterator(const linear_set<Record> &a,
    const Table &c,
    int tableRecordCountByKey,
    int tableRecordCount,
    std::shared_ptr<std::map<Datum, linear_set<Record>>> e,
    const std::string &joinColumn,
    const bool &o);

  Record mergeRecords(const Record & r1, const Record & r2);
  void advanceToEnd();
  void incrementIteratorWithIndex();
  void incrementIteratorWithoutIndex();
  void findNextMatchByDatum(Datum datum);
  void setIteratorToNewDatumKey(const Datum &datum);

  std::shared_ptr<linear_set<Record>::const_iterator> mIterLeft{nullptr};
  std::shared_ptr<Table::const_iterator> mIterRight{nullptr};
  int mTableRecordCountByKey{0};
  int mTableRecordCount{0};
  std::shared_ptr<std::map<Datum, linear_set<Record>>> mDatumKeys{nullptr};
  std::string mValue{""};
  bool mIsFinal{true};
  bool mOrder{true};

  friend class Database;
};
