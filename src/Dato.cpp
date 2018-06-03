#include "Dato.h"
#include <iostream>
#include <tuple>

using namespace std;

Dato::Dato(int valorNat) :
        _valorNat(valorNat), _valorStr(""), _esNat(true) {};

Dato::Dato(const string & valorStr) :
        _valorNat(0), _valorStr(valorStr), _esNat(false) {};

bool Dato::esNat() const {
    return _esNat;
};

bool Dato::esString() const {
    return not esNat();
}

string Dato::valorStr() const {
    return _valorStr;
};

int Dato::valorNat() const {
    return _valorNat;
};

Dato datoNat(int valorNat) {
    return Dato(valorNat);
}

Dato datoStr(string valorStr) {
    return Dato(valorStr);
}

Dato tipoNat = datoNat(0);
Dato tipoStr = datoStr("");

bool operator==(const Dato& d1, const Dato& d2) {
    if (d1.esNat() == d2.esNat()) {
        if (d1.esNat()) {
            return d1.valorNat() == d2.valorNat();
        } else {
            return d1.valorStr() == d2.valorStr();
        }
    }
    return false;
}

bool operator!=(const Dato& d1, const Dato& d2) {
    return !(d1 == d2);
}

bool operator<(const Dato& d1, const Dato& d2) {
    return (make_tuple(d1.esNat(), d1._valorNat, d1._valorStr) <
            make_tuple(d2.esNat(), d2._valorNat, d2._valorStr));
}

ostream & operator<<(ostream &os, const Dato& d) {
  if (d.esNat()) {
    os << to_string(d.valorNat());
  } else {
    os << d.valorStr();
  }
  return os;
}
