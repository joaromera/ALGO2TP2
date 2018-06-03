#ifndef _REGISTRO_H
#define _REGISTRO_H

#include <vector>
#include <iostream>
#include "Dato.h"
#include "linear_set.h"
#include "linear_map.h"
#include "string_map.h"

using namespace std;

/**
 * @brief Representa un registro de una tabla.
 *
 * Un registro asocia campos identificados con un string con valores
 * específicos.
 *
 * **se explica con** TAD Diccionario(string, Dato)
 */
class Registro {

public:

    /**
     * @brief Genera un nuevo registro con los campos y valores designados.
     *
     * \pre long(campos) = long(datos)
     * \post \P{res} = nuevoRegistro(campos, datos)
     *
     * \complexity{\O(long(campos) * (copy(campo) + copy(dato)))}
     */
    Registro(const vector<string>& campos, const vector<Dato>& datos);

    /**
     * @brief Devuelve el dato asociado a un campo.
     *
     * Devuelve el dato por referencia no modificable.
     *
     * \pre campo \in campos(\P{this})
     * \post \P{res} = valor(campo, \P{this})
     * 
     * \complexity{\O(S)}
     */
    const Dato& dato(const string& campo) const;

    /**
     * @brief Devuelve los campos definidos en un registro
     *
     * El conjunto de campos se devuelve por referencia
     *
     * \pre true
     * \post \P{res} = campos(\P{this})
     *
     * \complexity{\O(1)}
     */
    const linear_set<string>& campos() const;

    /**
     * @brief Devuelve diccionario (campos, datos)
     *
     * El diccionario se devuelve por referencia
     *
     * \pre true
     * \post \P{res} = (\P{this})
     *
     * \complexity{\O(1)}
     */
    const string_map<Dato>& camposDatos() const;

private:
	  ///////////////////////////////////////////////////////////////////////////////////////////////////
    /** \name Representación
     * rep: registro \TO bool\n
     * rep(d) \EQUIV 
     *  * _campos = claves(_datos)
     *
     * abs: registro \TO Registro\n
     * abs(r) \EQUIV r' \|
     *  * campos(r') = _campos \LAND
     *  * \FORALL (c : string) c \in _campos \IMPLIES valor(c, r') = valor(c,
     *    _datos) 
     */
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    /** @{ */
    linear_set<string> _campos;
    string_map<Dato> _datos;
    /** @} */

    friend ostream &operator<<(ostream &, const Registro &);
};


bool operator==(const Registro&, const Registro&);
ostream &operator<<(ostream &, const Registro &);


#endif //_REGISTRO_H
