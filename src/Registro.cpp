#include <iostream>
#include "Registro.h"
#include "utils.h"

using namespace std;

Registro::Registro(const vector<string>& campos, const vector<Dato>& datos) :
        _campos(campos.begin(), campos.end()) {
    for (int i = 0; i < campos.size(); i++) {
        _datos.insert(make_pair(campos[i], datos[i]));
    }
};

const Dato& Registro::dato(const string& campo) const {
    return _datos.at(campo);
}

const linear_set<string>& Registro::campos() const {
    return _campos;
}

const string_map<Dato>& Registro::camposDatos() const {
    return _datos;
}

bool operator==(const Registro& r1, const Registro& r2) {
    string_map<Dato> reg1 = r1.camposDatos();
    string_map<Dato> reg2 = r2.camposDatos();
    return reg1 == reg2;
}

ostream &operator<<(ostream &os, const Registro &r) {
    os << "{ ";
    int count = r.campos().size();
    for (auto v : r.campos()) {
        os << r._datos.at(v);
        count--;
        if (count) {
            os << ", ";
        }
    }
    os << " }";
    return os;
}

