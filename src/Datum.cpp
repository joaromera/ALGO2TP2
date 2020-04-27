#include "Datum.h"
#include <iostream>
#include <string>
#include <tuple>

Datum::Datum(int valorNat) : _valorNat(valorNat), _valorStr(""), _esNat(true) {}

Datum::Datum(const std::string &valorStr) : _valorNat(0), _valorStr(valorStr), _esNat(false) {}

bool Datum::esNat() const
{
  return _esNat;
}

bool Datum::esString() const
{
  return !esNat();
}

std::string Datum::valorStr() const
{
  return _valorStr;
}

int Datum::valorNat() const
{
  return _valorNat;
}

bool operator==(const Datum &d1, const Datum &d2)
{
  if (d1.esNat() == d2.esNat())
  {
    if (d1.esNat())
    {
      return d1.valorNat() == d2.valorNat();
    }
    else
    {
      return d1.valorStr() == d2.valorStr();
    }
  }
  return false;
}

bool operator!=(const Datum &d1, const Datum &d2)
{
  return !(d1 == d2);
}

bool operator<(const Datum &d1, const Datum &d2)
{
  return (std::make_tuple(d1.esNat(), d1._valorNat, d1._valorStr) < std::make_tuple(d2.esNat(), d2._valorNat, d2._valorStr));
}

std::ostream &operator<<(std::ostream &os, const Datum &d)
{
  if (d.esNat())
  {
    os << std::to_string(d.valorNat());
  }
  else
  {
    os << d.valorStr();
  }
  return os;
}
