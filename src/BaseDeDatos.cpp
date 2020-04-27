#include "BaseDeDatos.h"
#include <list>
#include <tuple>
#include <algorithm>

BaseDeDatos::BaseDeDatos() {
};

void BaseDeDatos::crearTabla(const string &nombre,
                             const linear_set<string> &claves,
                             const vector<string> &campos,
                             const vector<Datum> &tipos) {
    _nombres_tablas.fast_insert(nombre);
    _tablas.insert(make_pair(nombre, Tabla(claves, campos, tipos)));
}

void BaseDeDatos::agregarRegistro(const Registro &r, const string &nombre) {
    Tabla& t = _tablas.at(nombre);
    t.agregarRegistro(r);
    for (auto c : r.campos()) {
        if (tieneIndice(nombre, c)) {
            if (r.dato(c).esNat()) {
                _indicesNum[nombre][c][r.dato(c).valorNat()].insert(r);
            } else {
                _indices[nombre][c][r.dato(c).valorStr()].insert(r);
            }
        }
    }
}

const linear_set<string> &BaseDeDatos::tablas() const { return _nombres_tablas; }

const Tabla &BaseDeDatos::dameTabla(const string &nombre) const {
    return _tablas.at(nombre);
}

int BaseDeDatos::uso_criterio(const BaseDeDatos::Criterio &criterio) const {
    if (_uso_criterios.count(criterio)) {
        return _uso_criterios.at(criterio);
    } else {
        return 0;
    }
}

bool BaseDeDatos::registroValido(const Registro &r,
                                 const string &nombre) const {
    const Tabla& t = _tablas.at(nombre);
    if (t.campos().size() != r.campos().size()) return false;
    for (auto c : r.camposDatos()) {
        if (t.campos().count(c.first) == 0) return false;
        if (t.tipoCampo(c.first).esNat() != c.second.esNat()) return false;
    }
    for (auto rt : t.registros()) {
        int coincidencias = 0;
        int claves_en_t = t.claves().size();
        for (auto c : t.claves()) {
            if (r.dato(c) == rt.dato(c)) coincidencias++;
        }
        if (claves_en_t - coincidencias == 0) return false;
    }
    return true;
}

list<Registro> &
BaseDeDatos::_filtrar_registros(const string &campo, const Datum &valor,
                                list<Registro> &registros) const {
    return _filtrar_registros(campo, valor, registros, true);
}

list<Registro> &BaseDeDatos::_filtrar_registros(const string &campo,
                                                const Datum &valor,
                                                list<Registro> &registros,
                                                bool igualdad) const {
    auto iter = registros.begin();
    while (iter != registros.end()) {
        auto now = iter;
        iter++;
        if ((not igualdad) xor now->dato(campo) != valor) {
            registros.erase(now);
        }
    }

    return registros;
}

pair<vector<string>, vector<Datum> > BaseDeDatos::_tipos_tabla(const Tabla &t) {
    vector<string> res_campos;
    vector<Datum> res_tipos;
    for (auto c : t.campos()) {
        res_campos.push_back(c);
        res_tipos.push_back(t.tipoCampo(c));
    }
    return make_pair(res_campos, res_tipos);
}

bool BaseDeDatos::criterioValido(const Criterio &c, const string &nombre) const {
    const Tabla& t = _tablas.at(nombre);
    for (auto restriccion : c) {
        if (not t.campos().count(restriccion.campo())) return false;
        if (t.tipoCampo(restriccion.campo()).esNat() != restriccion.dato().esNat()) return false;
    }
    return true;
}

Tabla BaseDeDatos::busqueda(const BaseDeDatos::Criterio &c, const string &nombre) {
    if (_uso_criterios.count(c)) {
        _uso_criterios.at(c)++;
    } else {
        _uso_criterios.fast_insert(make_pair(c, 1));
    }

    const Tabla &ref = dameTabla(nombre);
    auto campos_datos = _tipos_tabla(ref);
    Tabla res(ref.claves(), campos_datos.first, campos_datos.second);
    list<Registro> regs(ref.registros().begin(), ref.registros().end());
    for (auto restriccion : c) {
        _filtrar_registros(restriccion.campo(), restriccion.dato(),
                           regs, restriccion.igual());
    }
    for (auto r : regs) {
        res.agregarRegistro(r);
    }
    return res;
}

linear_set<BaseDeDatos::Criterio> BaseDeDatos::top_criterios() const {
    linear_set<Criterio> ret;
    int max = 0;
    for (auto crit_count : _uso_criterios) {
        if (crit_count.second >= max) {
            if (crit_count.second > max) {
                ret = linear_set<Criterio>();
                max = crit_count.second;
            }
            ret.fast_insert(crit_count.first);
        }
    }
    return ret;
}

void BaseDeDatos::crearIndice(const string &tabla, const string &campo) {
    linear_set<Registro> reg = dameTabla(tabla).registros();
    for (auto r : reg) {
        if (r.dato(campo).esNat()) {
            _indicesNum[tabla][campo][r.dato(campo).valorNat()].insert(r);
        } else {
            _indices[tabla][campo][r.dato(campo).valorStr()].insert(r);
        }
    }
}

bool BaseDeDatos::tieneIndice(const string &tabla, const string &campo) {
    return _indices[tabla].count(campo) >= 1 || _indicesNum[tabla].count(campo) >= 1;
}

BaseDeDatos::join_iterator BaseDeDatos::join(const string &tabla1, const string &tabla2, const string &campo) {
    bool mismoOrden = true;
    bool primeraConIndice = tieneIndice(tabla1, campo);
    if (!primeraConIndice) {
        mismoOrden = false;
        if (dameTabla(tabla1).tipoCampo(campo).esNat()) {
            return join_helper_int(tabla2, tabla1, campo, mismoOrden);
        } else {
            return join_helper_str(tabla2, tabla1, campo, mismoOrden);
        }

    }
    if (dameTabla(tabla1).tipoCampo(campo).esNat()) {
        return join_helper_int(tabla1, tabla2, campo, mismoOrden);
    } else {
        return join_helper_str(tabla1, tabla2, campo, mismoOrden);
    }
}

BaseDeDatos::join_iterator BaseDeDatos::join_helper_str(const string &tabla1, const string &tabla2, const string &campo, const bool &orden){
    const Tabla& t1 = dameTabla(tabla1);
    const Tabla& t2 = dameTabla(tabla2);
    int tipo = 0;
    auto it2 = t2.registros_begin();
    int cant_reg_it2 = t2.cant_registros();
    string clave = (*it2).dato(campo).valorStr();

    auto it = _indices[tabla1].at(campo).find(clave);
    auto it_end = _indices[tabla1].at(campo).end();

    // Busca primer coincidencia entre las dos tablas
    while (cant_reg_it2 != 0 && it == it_end) {
        clave = (*it2).dato(campo).valorStr();
        it = _indices[tabla1].at(campo).find(clave);
        it_end = _indices[tabla1].at(campo).end();
        if (it == it_end) {
            ++it2;
            cant_reg_it2--;
        }
    }

    if (cant_reg_it2 == 0) return join_iterator(true);

    auto diccClaves = &_indices[tabla1].at(campo);
    auto it_tabla_con_indice = _indices[tabla1].at(campo).at(clave).begin();
    unsigned long cant_reg_por_indice = _indices[tabla1].at(campo).at(clave).size();
    auto it_tabla_sin_indice = t2.registros_begin();

    return join_iterator(it_tabla_con_indice, cant_reg_por_indice,
                         it_tabla_sin_indice, cant_reg_it2,
                         diccClaves, nullptr, campo, orden, tipo);
}

BaseDeDatos::join_iterator BaseDeDatos::join_helper_int(const string &tabla1, const string &tabla2, const string &campo, const bool &orden){
    const Tabla& t1 = dameTabla(tabla1);
    const Tabla& t2 = dameTabla(tabla2);
    int tipo = 1;
    auto it2 = t2.registros_begin();
    int cant_reg_it2 = t2.cant_registros();
    int clave = (*it2).dato(campo).valorNat();

    auto it = _indicesNum[tabla1].at(campo).find(clave)->second.begin();
    auto it_end = _indicesNum[tabla1].at(campo).find(clave)->second.end();

    // Busca primer coincidencia entre las dos tablas
    while (cant_reg_it2 != 0 && it == it_end) {
        clave = (*it2).dato(campo).valorNat();
        it = _indicesNum[tabla1].at(campo).find(clave)->second.begin();
        it_end = _indicesNum[tabla1].at(campo).find(clave)->second.end();
        if (it == it_end) {
            ++it2;
            cant_reg_it2--;
        }
    }

    if (cant_reg_it2 == 0) return join_iterator(true);

    auto diccClaves = &_indicesNum[tabla1].at(campo);
    auto it_tabla_con_indice = _indicesNum[tabla1].at(campo).at(clave).begin();
    unsigned long cant_reg_por_indice = _indicesNum[tabla1].at(campo).at(clave).size();
    auto it_tabla_sin_indice = t2.registros_begin();

    return join_iterator(it_tabla_con_indice, cant_reg_por_indice,
                         it_tabla_sin_indice, cant_reg_it2,
                         nullptr, diccClaves, campo, orden, tipo);
}

BaseDeDatos::join_iterator BaseDeDatos::join_end() {
    return join_iterator(true);
}