#pragma once
#include <string>
#include "Datum.h"

class Restriccion
{
public:
  Restriccion(const std::string &campo, const Datum &dato, bool igual);

  const std::string &campo() const;
  const Datum &dato() const;
  const bool &igual() const;

private:
  std::string _campo;
  Datum _dato;
  bool _igual;
};

bool operator==(const Restriccion &r1, const Restriccion &r2);
bool operator<(const Restriccion &r1, const Restriccion &r2);

Restriccion Rig(const std::string &campo, const std::string &valor);
Restriccion Rig(const std::string &campo, const int &valor);
Restriccion Rdif(const std::string &campo, const std::string &valor);
Restriccion Rdif(const std::string &campo, const int &valor);
