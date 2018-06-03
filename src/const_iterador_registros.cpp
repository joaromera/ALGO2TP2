#include "const_iterador_registros.h"

Tabla::const_iterador_registros::const_iterador_registros(const const_iterador_registros& o_it) :
  it_registro(o_it.it_registro) {}

const Registro& Tabla::const_iterador_registros::operator*() const {
  return *it_registro;
}

const Registro *Tabla::const_iterador_registros::operator->() const {
  return &(*it_registro);
}

Tabla::const_iterador_registros& Tabla::const_iterador_registros::operator++() {
  ++it_registro;
  return *this;
}

bool Tabla::const_iterador_registros::operator==(const Tabla::const_iterador_registros& o_it) const {
  return it_registro == o_it.it_registro;
}

bool Tabla::const_iterador_registros::operator!=(const Tabla::const_iterador_registros& o_it) const {
  return not (it_registro == o_it.it_registro);
}

Tabla::const_iterador_registros::const_iterador_registros(const linear_set<Registro>::const_iterator _it_registro) :
    it_registro(_it_registro) {}
