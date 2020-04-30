#pragma once
#include <vector>
#include <iostream>
#include "Datum.h"
#include "linear_set.h"
#include "linear_map.h"
#include "string_map.h"

class Registro
{
public:
  Registro(const std::vector<std::string> &campos, const std::vector<Datum> &datos);

  const Datum &dato(const std::string &campo) const;
  const linear_set<std::string> &campos() const;
  const string_map<Datum> &camposDatos() const;

private:
  linear_set<std::string> _campos;
  string_map<Datum> _datos;
  friend std::ostream &operator<<(std::ostream &, const Registro &);
};

bool operator==(const Registro &, const Registro &);
std::ostream &operator<<(std::ostream &, const Registro &);
