#include "Restriccion.h"
#include <tuple>

Restriccion::Restriccion(const std::string &campo, const Datum &dato, bool igual)
    : _campo(campo), _dato(dato), _igual(igual){};

const std::string &Restriccion::campo() const { return _campo; }

const Datum &Restriccion::dato() const { return _dato; }

const bool &Restriccion::igual() const { return _igual; }

bool operator==(const Restriccion &r1, const Restriccion &r2) {
  return (r1.campo() == r2.campo() and r1.dato() == r2.dato() and
          r1.igual() == r2.igual());
}

bool operator<(const Restriccion &r1, const Restriccion &r2) {
  return (make_tuple(r1.campo(), r1.dato(), r1.igual()) <
          make_tuple(r2.campo(), r2.dato(), r2.igual()));
}

Restriccion Rig(const std::string &campo, const std::string &valor) {
  return Restriccion(campo, Datum(valor), true);
}
Restriccion Rig(const std::string &campo, const int &valor) {
  return Restriccion(campo, Datum(valor), true);
}
Restriccion Rdif(const std::string &campo, const std::string &valor) {
  return Restriccion(campo, Datum(valor), false);
}
Restriccion Rdif(const std::string &campo, const int &valor) {
  return Restriccion(campo, Datum(valor), false);
}
