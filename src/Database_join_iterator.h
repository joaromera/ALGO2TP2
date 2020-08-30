#pragma once
#include "Record.h"
#include "Table.h"

class join_iterator
{
public:
  join_iterator(const join_iterator &n);
  join_iterator &operator=(const join_iterator &n);
  ~join_iterator() = default;

  bool operator==(const join_iterator &j) const;
  bool operator!=(const join_iterator &j) const;

  join_iterator &operator++();
  join_iterator operator++(int);
  const Record operator*();

private:
  join_iterator() = default;
  join_iterator(linear_set<Record>::const_iterator a,
    Table::const_iterator c,
    int ind,
    int sin,
    std::shared_ptr<std::map<Datum,
    linear_set<Record>>> e,
    const std::string &f,
    const bool &o);

  Record mergeRecords(const Record & r1, const Record & r2);
  void advanceToEnd();
  void incrementIteratorWithIndex();
  void incrementIteratorWithoutIndex();
  void findNextMatchByDatum(Datum datum);
  void setIteratorToNewDatumKey(const Datum &datum);

  std::shared_ptr<linear_set<Record>::const_iterator> it1 {nullptr};
  std::shared_ptr<Table::const_iterator> it2 {nullptr};
  int tableRecordCountByKey{0};
  int tableRecordCount{0};
  std::shared_ptr<std::map<Datum, linear_set<Record>>> datumKeys {nullptr};
  std::string campo {""};
  bool isFinal {true};
  bool orden {true};

  friend class Database;
  friend class Table;
};
