#pragma once

#include <string>

class Datum
{
public:
  Datum(int valorNat);
  Datum(const std::string &valorStr);

  bool esNat() const;
  bool esString() const;
  std::string valorStr() const;
  int valorNat() const;

private:
  int _valorNat;
  std::string _valorStr;
  bool _esNat;

  friend bool operator<(const Datum &, const Datum &);
};

bool operator==(const Datum &, const Datum &);

bool operator!=(const Datum &, const Datum &);

std::ostream &operator<<(std::ostream &, const Datum &);
