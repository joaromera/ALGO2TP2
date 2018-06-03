#include "Restriccion.h"
#include <tuple>

Restriccion::Restriccion(const string &campo, const Dato &dato, bool igual)
    : _campo(campo), _dato(dato), _igual(igual){};

const string &Restriccion::campo() const { return _campo; }

const Dato &Restriccion::dato() const { return _dato; }

const bool &Restriccion::igual() const { return _igual; }

bool operator==(const Restriccion &r1, const Restriccion &r2) {
  return (r1.campo() == r2.campo() and r1.dato() == r2.dato() and
          r1.igual() == r2.igual());
}

bool operator<(const Restriccion &r1, const Restriccion &r2) {
  return (make_tuple(r1.campo(), r1.dato(), r1.igual()) <
          make_tuple(r2.campo(), r2.dato(), r2.igual()));
}

Restriccion Rig(const string &campo, const string &valor) {
  return Restriccion(campo, datoStr(valor), true);
}
Restriccion Rig(const string &campo, const int &valor) {
  return Restriccion(campo, datoNat(valor), true);
}
Restriccion Rdif(const string &campo, const string &valor) {
  return Restriccion(campo, datoStr(valor), false);
}
Restriccion Rdif(const string &campo, const int &valor) {
  return Restriccion(campo, datoNat(valor), false);
}
