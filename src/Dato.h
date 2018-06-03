#ifndef DATO_H
#define DATO_H

#include <string>

using namespace std;

/**
 *  @brief Representa un Dato de una Base de Datos.
 *
 *  **se explica con** TAD Dato
 */
class Dato {

public:

    /**
     * @brief Constructor de dato con valor nat
     *
     * @param valorNat valor natural del dato
     *
     * \pre true
     * \post \P{res} \IGOBS datoNat(valorNat)
     * \complexity{\O(1)}
     */
    Dato(int valorNat);

    /**
     * @brief Constructor de dato con valor string
     *
     * @param valorStr valor string del dato
     *
     * \pre true
     * \post \P{res} \IGOBS datoStr(valorStr)
     * \complexity{\O(1)}
     */
    Dato(const string& valorStr);

    /**
     * @brief True si el dato es de tipo nat
     *
     * \pre true
     * \post \P{res} \IGOBS Nat?(\P{this})
     * \complexity{\O(1)}
     */
    bool esNat() const;

    /**
     * @brief True si el dato es de tipo string
     *
     * \pre true
     * \post \P{res} \IGOBS String?(\P{this})
     * \complexity{\O(1)}
     */
    bool esString() const;

    /**
     * @brief El valor del dato 
     *
     * Devuelve el valor por copia.
     *
     * \pre String?(\P{this})
     * \post \P{res} \IGOBS valorStr(\P{this})
     * \complexity{\O(1)}
     */
    string valorStr() const;

    /**
     * @brief El valor del dato 
     *
     * Devuelve el valor por copia.
     *
     * \pre Nat?(\P{this})
     * \post \P{res} \IGOBS valorNat(\P{this})
     * \complexity{\O(1)}
     */
    int valorNat() const;

private:
	  ///////////////////////////////////////////////////////////////////////////////////////////////////
    /** \name Representación
     * rep: dato \TO bool\n
     * rep(d) \EQUIV 
     *  * _esNat \IMPLIES _valorStr = "" \LOR \LNOT _esNat \IMPLIES _valorNat = 0
     *
     * abs: dato \TO Dato\n
     * abs(d) \EQUIV d' \|
     *  * Nat?(d') = d._esNat \LAND 
     *  * Nat?(d') \IMPLIES valorNat(d) = d._valorNat \LAND
     *  * \NEG Nat?(d') \IMPLIES valorStr(d) = d._valorStr 
     */
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    /** @{ */
    /** @brief Valor cuando el dato es nat. */
    int _valorNat;
    
    /** @brief Valor cuando el dato es nat. */
    string _valorStr;
    
    /** @brief Define si el dato es nat. */
    bool _esNat;
    /** @} */
    
    /** @brief Comparador de datos */
    friend bool operator<(const Dato&, const Dato&);
};

Dato datoNat(int valorNat);

Dato datoStr(string valorStr);

extern Dato tipoNat;

extern Dato tipoStr;

bool operator==(const Dato&, const Dato&);

bool operator!=(const Dato&, const Dato&);

ostream &operator<<(ostream &, const Dato&);


#endif // DATO_H
