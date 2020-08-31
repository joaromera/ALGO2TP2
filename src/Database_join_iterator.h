#pragma once
#include "Database_defines.h"
#include "linear_set.h"
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
  join_iterator(const RecordSet &a,
    const Table &c,
    int tableRecordCountByKey,
    int tableRecordCount,
    std::shared_ptr<DatumToRecordsMap> e,
    const std::string &joinColumn,
    const bool &o);

  Record mergeRecords(const Record & r1, const Record & r2);
  void advanceToEnd();
  void incrementIteratorWithIndex();
  void incrementIteratorWithoutIndex();
  void findNextMatchByDatum(Datum datum);
  void setIteratorToNewDatumKey(const Datum &datum);

  std::shared_ptr<RecordSet::const_iterator> mIterLeft{nullptr};
  std::shared_ptr<Table::const_iterator> mIterRight{nullptr};
  int mTableRecordCountByKey{0};
  int mTableRecordCount{0};
  std::shared_ptr<DatumToRecordsMap> mDatumKeys{nullptr};
  std::string mValue{""};
  bool mIsFinal{true};
  bool mOrder{true};

  friend class Database;
};
