#ifndef RESTRICCION_H
#define RESTRICCION_H

#include <string>
#include "Datum.h"

/**
 * @brief Representa una restricción de la base de datos.
 *
 * **se explica con** TAD Restriccion
 */
class Restriccion {

public:

    /**
     * @brief Constructor de Restricción
     *
     * \pre true
     * \post \P{this} == nueva(campo, dato, igual)
     *
     * \complexity{\O(L)}
     */
    Restriccion(const std::string& campo, const Datum& dato, bool igual);

    /**
     * @brief Observador campo
     *
     * El valor resultado se devuelve por referencia
     *
     * \pre true
     * \post \P{res} == campo(\P{this}}
     *
     * \complexity{\O(1)}
     */
    const std::string& campo() const;

    /**
     * @brief Observador dato
     *
     * El valor resultado se devuelve por referencia
     *
     * \pre true
     * \post \P{res} == dato(\P{this}}
     *
     * \complexity{\O(1)}
     */
    const Datum& dato() const;

    /**
     * @brief Observador porIgual
     *
     * El valor resultado se devuelve por referencia
     *
     * \pre true
     * \post \P{res} == porIgual(\P{this}}
     *
     * \complexity{\O(1)}
     */
    const bool& igual() const;

private:
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    /** \name Representación
     * rep: restricción \TO bool\n
     * rep(r) \EQUIV true
     *
     * abs: restricción \TO Restricción \n
     * abs(r) \EQUIV r' \|
     *  * campo(r') = _campo \LAND
     *  * dato(r') = _dato \LAND
     *  * porIgual(r') = _igual
     */
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    /** @{ */
    std::string _campo;
    Datum _dato;
    bool _igual;
    /** @} */
   
};

bool operator==(const Restriccion& r1, const Restriccion& r2);
bool operator<(const Restriccion& r1, const Restriccion& r2);

Restriccion Rig(const std::string& campo, const std::string& valor);
Restriccion Rig(const std::string& campo, const int& valor);
Restriccion Rdif(const std::string& campo, const std::string& valor);
Restriccion Rdif(const std::string& campo, const int& valor);

#endif 
