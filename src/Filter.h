#pragma once
#include <string>
#include "Datum.h"

class Filter
{
public:
  Filter(const std::string &column, const Datum &datum, bool equals);

  const std::string &column() const;
  const Datum &datum() const;
  const bool &equals() const;

private:
  std::string _column;
  Datum _datum;
  bool _equals;
};

bool operator==(const Filter &r1, const Filter &r2);
bool operator<(const Filter &r1, const Filter &r2);

Filter Rig(const std::string &column, const std::string &value);
Filter Rig(const std::string &column, const int &value);
Filter Rdif(const std::string &column, const std::string &value);
Filter Rdif(const std::string &column, const int &value);
