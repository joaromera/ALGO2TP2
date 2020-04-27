#ifndef _BASEDEDATOS_H
#define _BASEDEDATOS_H

#include "Registro.h"
#include "Restriccion.h"
#include "Tabla.h"
#include <utility>
#include <list>
#include <string>
#include "linear_map.h"
#include "linear_set.h"
#include "string_map.h"
#include <map>

using namespace std;
typedef string_map<string_map<string_map<linear_set<Registro>>>> Index;
typedef string_map<string_map<map<int, linear_set<Registro>>>> IndexInt;

/**
 * @brief Una base de datos es un administrador de tablas con funciones de
 * búsqueda.
 *
 * Una base de datos permite administrar tablas identificadas por registro.
 * Permite saber si se puede agegar un registro a una tabla y luego agregarlo.
 * Permite realizar filtros del contenido de tablas mediante criterios de
 * búsqueda. Además mantiene estadísticas del uso de los criterios.
 *
 * **se explica con** TAD BaseDeDatos
 */
class BaseDeDatos {
private:

public:
    /**
     * @brief Iterador de la unión de dos tablas por un campo en común.
     *
     * Se construye mediante el campo en común que tienen ambas tablas -al menos una tiene que tener índice-, al construirse
     * toma como atributos (descriptos más abajo) el iterador a la tabla con índice (it1) y el iterador a la tabla sin
     * indice (it2). El join iterator toma el primer registro de la tabla sin índice cuya clave en el campo por el cual
     * se hace la unión tenga coincidencia en la tabla con índice. De esta manera se avanza it1 hasta que no haya más
     * coincidencias por clave, momento en el cual se avanza it2 y se busca una coincidencia según la nueva clave. El
     * proceso se repite hasta que no hay más coincidencias ni registros (cant_reg_por_clave y cant_reg_en_tabla serán 0 y
     * final será true). Para saber que un join_iterator llegó al final se verifica que final sea true.
     *
     *
     * @param it1 es el iterador al conjunto de registros por clave de la tabla con índice.
     * @param it2 es el iterador de la tabla sin índice.
     * @param cant_reg_en_tabla indica la cantidad restante de registros en la tabla sin indice, se decrementa a medida que
     * avanza el iterador (it2).
     * @param cant_reg_por_clave es la cantidad restante de registros por clave de la tabla con índice. Se decrementa conforme
     * it1 avanza.
     * @param diccClavesStr o diccClavesNat apuntan al índice de la tabla sobre el cual se hace el join, se usa la versión str
     * o nat dependiendo del tipo de datos del campo.
     * @param campo es el campo con el que se hace el join, es común a ambas tablas.
     * @param final indica si se llegó al final del conjunto del join.
     * @param orden determina el orden en el cual se unen los campos de cada tabla al dereferenciar el join_iterator.
     * @param tipo precisa si se trata de un join_iterator hecho sobre un campo de datos Nat (valor 0) o Str (1).
     *
     */
    class join_iterator {

    public:
        /**
        * @brief Constructor por defecto del iterador.
        *
        * \complexity{\O(1)}
        */
        join_iterator() {
            it1 = nullptr;
            it2 = nullptr;
            cant_reg_en_tabla = 0;
            cant_reg_por_clave = 0;
            diccClavesStr = nullptr;
            diccClavesNat = nullptr;
            campo = "";
            final = true;
            orden = true;
            tipo = 0;
        }
        /**
        * @brief Constructor por copia del iterador.
        *
        * \complexity{\O(1)}
        */
        join_iterator(const join_iterator & n) {
            it1 = nullptr;
            it2 = nullptr;
            if (n.final) {
                it1 = nullptr;
                it2 = nullptr;
                cant_reg_en_tabla = 0;
                cant_reg_por_clave = 0;
                diccClavesStr = nullptr;
                diccClavesNat = nullptr;
                campo = "";
                final = true;
                orden = true;
                tipo = 0;
            } else {
                if (it1 != nullptr) {
                    delete it1;
                    it1 = nullptr;
                }
                if (it2 != nullptr) {
                    delete it2;
                    it2 = nullptr;
                }
                it1 = new linear_set<Registro>::const_iterator(*n.it1);
                it2 = new Tabla::const_iterador_registros(*n.it2);
                cant_reg_por_clave = n.cant_reg_por_clave;
                cant_reg_en_tabla = n.cant_reg_en_tabla;
                diccClavesStr = n.diccClavesStr;
                diccClavesNat = n.diccClavesNat;
                campo = n.campo;
                final = n.final;
                orden = n.orden;
                tipo = n.tipo;
            }
        }

        /**
        * @brief Operador de asignacion.
        *
        * \complexity{\O(1)}
        */
        join_iterator& operator =(const join_iterator & n) {
            if (n.final == true) {
                it1 = nullptr;
                it2 = nullptr;
                diccClavesNat = nullptr;
                diccClavesStr = nullptr;
                final = true;
            } else {
                if (it1 != nullptr) {
                    delete it1;
                    it1 = nullptr;
                }
                if (it2 != nullptr) {
                    delete it2;
                    it2 = nullptr;
                }
                it1 = new linear_set<Registro>::const_iterator(*n.it1);
                it2 = new Tabla::const_iterador_registros(*n.it2);;
                diccClavesStr = n.diccClavesStr;
                diccClavesNat = n.diccClavesNat;
                campo = n.campo;
                final = n.final;
                cant_reg_en_tabla = n.cant_reg_en_tabla;
                cant_reg_por_clave = n.cant_reg_por_clave;
                orden = n.orden;
                tipo = n.tipo;
            }
            return *this;
        }

        /**
        * @brief Destructor del join_iterator.
        *
        * \complexity{\O(1)}
        */
        ~join_iterator() {
            if (it1 != nullptr) delete it1;
            if (it2 != nullptr) delete it2;
            it1 = nullptr;
            it2 = nullptr;
            diccClavesStr = nullptr;
            diccClavesNat = nullptr;
        }

        /** @brief Operador de comparacion
         *  @param \P{j}
         *
         * \pre true
         * \post res == it1 = \P{j}.it1 \LAND it2 = \P{j}.it2
         *  \LAND diccClavesStr = \P{j}.diccClavesStr \LAND diccClavesNat = \P{j}.diccClavesNat
         *  \LAND cant_reg_en_tabla = \P{j}.cant_reg_en_tabla \LAND cant_reg_por_clave = \P{j}.cant_reg_por_clave
         *  \LAND campo = \P{j}.campo \LAND orden = \P{j}.orden \LAND tipo = \P{j}.tipo
         *
         * \complexity{\O(1)}
         */
        bool operator==(const join_iterator &j) const {
            if (final == j.final) {
                return true;
            } else {
                return it1 == j.it1 &&
                       it2 == j.it2 &&
                       diccClavesStr == j.diccClavesStr &&
                       diccClavesNat == j.diccClavesNat &&
                       cant_reg_en_tabla == j.cant_reg_en_tabla &&
                       cant_reg_por_clave == j.cant_reg_por_clave &&
                       campo == j.campo &&
                       orden == j.orden &&
                       tipo == j.tipo;
            }
        }

        /** @brief Operador de comparacion
         *  @param \P{j}
         *
         * \pre true
         * \post res != it1 = \P{j}.it1 \LAND it2 = \P{j}.it2
         *  \LAND diccClavesStr = \P{j}.diccClavesStr \LAND diccClavesNat = \P{j}.diccClavesNat
         *  \LAND cant_reg_en_tabla = \P{j}.cant_reg_en_tabla \LAND cant_reg_por_clave = \P{j}.cant_reg_por_clave
         *  \LAND campo = \P{j}.campo \LAND orden = \P{j}.orden \LAND tipo = \P{j}.tipo
         *
         * \complexity{\O(1)}
         */
        bool operator!=(const join_iterator &j) const {
            return !(*this==j);
        }

        /**
         * @brief Avanza it1 una posición, si la cantidad de registros por clave es igual a 0
         * avanza it2 una posicion tambien y renueva it1.
         *
         * \pre cant_reg_por_clave \TEXTGREATER 0
         * \post \P{res} es una referencia a \P{this}. \P{this} apunta a la posición
         * siguiente.
         *
         * \complexity{\O(n * [L + log(m)])}
         */
        join_iterator& operator++() {
            avanzarItConIndice();
            if (cant_reg_por_clave == 0) {
                avanzarItSinIndice();
                if (cant_reg_en_tabla == 0) {
                    ponerEnFin();
                    return *this;
                } else {
                    if (tipo == 0) {
                        string clave = (*it2)->dato(campo).valorStr();
                        buscarProxCoincidenciaStr(clave);
                        return *this;
                    } else {
                        int clave = (*it2)->dato(campo).valorNat();
                        buscarProxCoincidenciaNat(clave);
                        return *this;
                    }
                }
            } else {
                return *this;
            }
        }

        /**
         * @brief Avanza el it1 n posiciones, si la cantidad de registros por clave es igual a 0
         * avanza it2 una posicion tambien.
         *
         * \pre cant_reg_por_clave \TEXTGREATER n - 1.
         * \post \P{res} es una referencia a \P{this}. \P{this} apunta a la posición
         * siguiente n veces.
         *
         * \complexity{\O(n * [L + log(m)])}
         */
        join_iterator operator++(int) {
            auto copia = *this;
            ++*this;
            return copia;
        }

        /**
         * @brief Desreferencia el puntero
         *
         * El valor devuelto tiene aliasing dentro de la colección.
         *
         * \pre cant_reg_por_clave \TEXTGREATER 0.
         * \post El valor resultado es una referencia constante al valor apuntado.
         *
         * \complexity{\O(copy(Registro))}
         */
        const Registro operator*() {
            if (orden) {
                Registro nuevo = combinarRegistro(**it1, **it2);
                return nuevo;
            } else {
                Registro nuevo = combinarRegistro(**it2, **it1);
                return nuevo;
            }
        }

        friend class BaseDeDatos;
        friend class Tabla;

    private:
        join_iterator(linear_set<Registro>::const_iterator a,
                      int ind,
                      Tabla::const_iterador_registros c,
                      int sin,
                      const string_map<linear_set<Registro>>* e,
                      const map<int, linear_set<Registro>>* g,
                      const string &f,
                      const bool &o,
                      int t) : cant_reg_por_clave(ind),
                               cant_reg_en_tabla(sin),
                               diccClavesStr(e), diccClavesNat(g),
                               campo(f), final(false), orden(o), tipo(t)
        {
            if (t == 0 ) { // es string
                it1 = new linear_set<Registro>::const_iterator(a);
                it2 = new Tabla::const_iterador_registros(c);
            } else { // es nat
                it1 = new linear_set<Registro>::const_iterator(a);
                it2 = new Tabla::const_iterador_registros(c);
            }
        };

        join_iterator(const bool &) : it1(nullptr), cant_reg_por_clave(0),
                                       it2(nullptr), cant_reg_en_tabla(0),
                                       diccClavesStr(nullptr), diccClavesNat(nullptr),
                                       campo(""), final(true), orden(true), tipo(0) {};

        linear_set<Registro>::const_iterator* it1;
        int cant_reg_por_clave;
        Tabla::const_iterador_registros* it2;
        int cant_reg_en_tabla;
        const string_map<linear_set<Registro>>* diccClavesStr;
        const map<int, linear_set<Registro>>* diccClavesNat;
        string campo;
        bool final;
        bool orden;
        int tipo;

        /**
        * @brief Crea el registro que es la unión del registro r1 con el r2 según el campo elegido
        * Primero crea vectores con los campos y datos de cada registro (no duplicando el campo indexado)
        * luego crea el registro con estos vectores y lo devuelve
        *
        * \complexity{\O(C)}
        *
        */
        Registro combinarRegistro(Registro r1, Registro r2) {
            vector<string> combCampos;
            vector<Datum> combDatos;

            for (auto c : r1.campos()) {
                combCampos.push_back(c);
                combDatos.push_back(r1.dato(c));
            }

            for (auto c : r2.campos()) {
                if (r1.campos().count(c) == 0 && c != campo) {
                    combCampos.push_back(c);
                    combDatos.push_back(r2.dato(c));
                }
            }

            Registro res = Registro(combCampos, combDatos);
            return res;
        }

        /**
        * @brief Setea punteros a null y final a true para indicar que llegó al final de la union de tablas
        *
        * \complexity{\O(1)}
        */
        void ponerEnFin() {
            if (it1 != nullptr) delete it1;
            if (it2 != nullptr) delete it2;
            it1 = nullptr;
            it2 = nullptr;
            campo = "";
            final = true;
            diccClavesNat = nullptr;
            diccClavesStr = nullptr;
        }

        /**
        * @brief Incrementa el iterador del string_map de pares (claves, registros)
        *
        * \complexity{\O(S)}
        */
        void avanzarItConIndice() {
            ++(*it1);
            cant_reg_por_clave--;        
        }

        /**
        * @brief Incrementa el iterador de la tabla que no está siendo recorrida por índice
        *
        * \complexity{\O(1)}
        *
        */
        void avanzarItSinIndice() {
            ++(*it2);
            cant_reg_en_tabla--;
        }

        /**
        * @brief Busca nueva clave en el campo por el cual se hace el join, luego, busca coincidencias en la otra tabla.
        * Si no encuentra ninguna, incrementa la tabla que no está siendo recorrida por índice y busca la nueva clave del
        * próximo registro hasta llegar al final o a alguna coincidencia.
        *
        * \complexity{\O(S)}
        *
        */
        void buscarProxCoincidenciaStr(string &clave) {
            while (cant_reg_en_tabla != 0 && diccClavesStr->count(clave) == 0) {
                clave = (*it2)->dato(campo).valorStr();
                if (diccClavesStr->count(clave) == 0) {
                    avanzarItSinIndice();
                }
            }
            if (cant_reg_en_tabla == 0) {
                ponerEnFin();
            } else if (diccClavesStr->count(clave) != 0) {
                setItEnNuevaClaveStr(clave);
            } else {
                ponerEnFin();
            }
        }

        /**
        * @brief Busca nueva clave en el campo por el cual se hace el join, luego, busca coincidencias en la otra tabla.
        * Si no encuentra ninguna, incrementa la tabla que no está siendo recorrida por índice y busca la nueva clave del
        * próximo registro hasta llegar al final o a alguna coincidencia.
        *
        * \complexity{\O(log(m))}
        *
        */
        void buscarProxCoincidenciaNat(int &clave) {
            while (cant_reg_en_tabla != 0 && diccClavesNat->count(clave) == 0) {
                clave = (*it2)->dato(campo).valorNat();
                if (diccClavesNat->count(clave) == 0) {
                    avanzarItSinIndice();
                }
            }
            if (cant_reg_en_tabla == 0) {
                ponerEnFin();
            } else if (diccClavesNat->count(clave) != 0) {
                setItEnNuevaClaveNat(clave);
            } else {
                ponerEnFin();
            }
        }

        /**
        * @brief De tener coincidencia por clave, setea el iterador del diccClavesStr al nuevo conjunto de registros.
        * Actualiza la cantidad de registros.
        *
        * \complexity{\O(1)}
        *
        */
        void setItEnNuevaClaveStr(const string &clave) {
            delete it1;
            it1 = new linear_set<Registro>::const_iterator(diccClavesStr->at(clave).begin());
            cant_reg_por_clave = diccClavesStr->at(clave).size();
        }

        /**
        * @brief De tener coincidencia por clave, setea el iterador del diccClavesNat al nuevo conjunto de registros.
        * Actualiza la cantidad de registros.
        *
        * \complexity{\O(1)}
        *
        */
        void setItEnNuevaClaveNat(const int &clave) {
            delete it1;
            it1 = new linear_set<Registro>::const_iterator(diccClavesNat->at(clave).begin());
            cant_reg_por_clave = diccClavesNat->at(clave).size();
        }
    };

    /** @brief Criterio de búsqueda para una base de datos */
    typedef linear_set<Restriccion> Criterio;

    /**
     * @brief Inicializa una base de datos sin tablas.
     *
     * \pre true
     * \post \P{this} = nuevaDB
     *
     * \complexity{\O(1)}
     */
    BaseDeDatos();

    /**
     * @brief Crea una nueva tabla en la base de datos.
     *
     * @param nombre Nombre identificador de la tabla
     * @param claves Claves de la tabla a crear
     * @param campos Campos de la tabla a crear
     * @param tipos  Tipos para los campos de la tabla a crear
     *
     * \pre db = \P{this} \LAND
     *      \LNOT (nombre \IN tablas(\P{this})) \LAND
     *      \LAND \LNOT \EMPTYSET?(claves) \LAND
     *      \FORALL (c: campo) c \IN claves \IMPLICA c \IN campos \LAND
     *      long(campos) = long(tipos) \LAND sinRepetidos(campos)
     * \post \P{this} = agregarTabla(nuevaTabla(claves, nuevoRegistro(campos, tipos)), db)
     *
     * \complexity{\O(C + S)}
     */
    void crearTabla(const string &nombre, const linear_set<string> &claves,
                    const vector<string> &campos, const vector<Datum> &tipos);

    /**
     * @brief Agrega un registro a la tabla parámetro
     *
     * @param r Registro a agregar
     * @param nombre Nombre de la tabla donde se agrega el registro
     *
     * \pre db = \P{this} \LAND nombre \IN tablas(\P{this}) \LAND
     *      puedoInsertar?(r, dameTabla(\P{this}))
     * \post \P{this} = insertarEntrada(r, nombre, db)
     *
     * \complexity{\O(copy(reg)) o \O((L+log(m))*C+copy(reg))}
     */
    void agregarRegistro(const Registro &r, const string &nombre);

    /**
     * @brief Devuelve el conjunto de tablas existentes en la base.
     *
     * El conjunto de nombres se devuelve por referencia no-modificable.
     *
     * \pre true
     * \post \P{res} = tablas(\P{this})
     *
     * \complexity{\O(1)}
     */
    const linear_set<string> &tablas() const;

    /**
     * @brief Devuelve la tabla asociada al nombre.
     *
     * La tabla se devuelve por referencia no modificable.
     *
     * @param nombre Nombre de la tabla buscada.
     *
     * \pre nombre \IN tablas(\P{this})
     * \post \P{res} = dameTabla(nombre, \P{this})
     *
     * \complexity{O(L)}
     */
    const Tabla &dameTabla(const string &nombre) const;

    /**
     * @brief Devuelve la cantidad de usos que tiene un criterio
     *
     * @param criterio Criterio por el cual se consulta.
     *
     * \pre nombre \IN tablas(\P{this})
     * \post \P{res} = usoCriterio(criterio, \P{this})
     *
     * \complexity{\O(#cs * cmp(Criterio))}
     */
    int uso_criterio(const Criterio &criterio) const;

    /**
     * @brief Evalúa si un registro puede ingresarse en la tabla parámetro.
     *
     * @param r Registro a ingresar en la tabla.
     * @param nombre Nombre de la tabla.
     *
     * \pre nombre \IN tablas(\P{this})
     * \post \P{res} = puedoInsertar?(r, dameTabla(nombre, \P{this}))
     *
     * \complexity{\O(C+(c*n*L))}
     */
    bool registroValido(const Registro &r, const string &nombre) const;

    /**
     * @brief Evalúa si un criterio puede aplicarse en la tabla parámetro.
     *
     * @param c Criterio a utilizar.
     * @param nombre Nombre de la tabla.
     *
     * \pre tabla \IN tablas(\P{this})
     * \post \P{res} = criterioValido(c, nombre, \P{this})
     *
     * \complexity{\O(cr)}
     */
    bool criterioValido(const Criterio &c, const string &nombre) const;

    /**
     * @brief Devuelve el resultado de buscar en una tabla con un criterio.
     *
     * @param c Criterio de búsqueda utilizado.
     * @param nombre Nombre de la tabla.
     *
     * \pre nombre \IN tablas(\P{this}) \LAND criterioValido(c, nombre, \P{this})
     * \post \P{res} = buscar(c, nombre, \P{this})
     *
     * \complexity{\O(T + cs * cmp(Criterio) + cr * n * (C + L + copy(reg)))}
     */
    Tabla busqueda(const Criterio &c, const string &nombre);

    /**
     * @brief Devuelve los criterios de máximo uso.
     *
     * \pre true
     * \post \FORALL (c : Criterio) [c \IN \P{res} \IFF
     *       \FORALL (c' : Criterio) usoCriterio(c, db) >= usoCriterio(c', db)]
     *
     * \complexity{\O(cs * copy(Criterio))}
     */
    linear_set<Criterio> top_criterios() const;

    /**
     * @brief Crea indice en una tabla según uno de sus campos.
     *
     * @param tabla Nombre de la tabla.
     * @param campo Nombre del campo de la tabla.
     *
     * \pre db = \P{this} \LAND tabla \IN tablas(\P{this}) \LAND
     *      campo \IN campos(tabla)
     * \post \P{this} = crearIndice(tabla, campo, db)
     *
     *
     * \complexity{\O(m * (L + log(m)))}
     */
    void crearIndice(const string &tabla, const string &campo);

    /**
     * @brief Evalúa si una tabla tiene índice en el campo.
     *
     * @param tabla Nombre de la tabla.
     * @param campo Nombre del campo de la tabla.
     *
     * \pre db = \P{this} \LAND tabla \IN tablas(\P{this}) \LAND
     *      campo \IN campos(tabla)
     * \post \P{res} = tieneIndice?(tabla, campo, db)
     *
     * \complexity{\O(L)}
     */
    bool tieneIndice(const string &tabla, const string &campo);

    /**
     * @brief Join de dos tablas designando un campo en común.
     *
     * @param tabla1 Nombre de la tabla 1.
     * @param tabla2 Nombre de la tabla 2.
     * @param campo Nombre del campo de las tablas para hacer el join.
     *
     * \pre db = \P{this} \LAND tabla1 \IN tablas(\P{this}) \LAND tabla2 \IN tablas(\P{this})
     *      \LAND campo \IN campos(tabla1) \LAND campo \IN campos(tabla2)
     *      \LAND tieneIndice?(tabla1, campo, db) \LOR tieneIndice?(tabla2, campo, db)
     * \post \P{res} = Join(tabla1, tabla2, campo, db)
     *
     * \complexity{\O(m * (L+log(m))}
     */
    join_iterator join(const string &tabla1, const string &tabla2, const string &campo);

    /**
     * @brief Devuelve un join_iterator que indica el final de un join.
     *
     * \complexity{\O(1)}
     */
    join_iterator join_end();

private:
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    /** \name Representación
     * rep: basededatos \TO bool\n
     * rep(bd) \EQUIV 
     *  * _nombres_tablas = claves(_tablas) \LAND
     *  * \FORALL (c : Criterio) c \IN claves(_uso_criterios) \IMPLIES 
     *     * (
     *       * \EXISTS (n : string) n \IN _nombres_tablas 
     *       * \LAND criterioValido(c, n, db)
     *     * ) \LAND
     *     * obtener(c, _uso_criterios) > 0 \LAND
     *  * \FORALL (t : tabla) t \IN claves(_indices) \IMPLIES t \IN claves(_tablas) \LAND
     *  * * \FORALL (c : campo) c \IN campos(t) \IMPLIES c \IN claves(obtener(t, _indices)) \LAND
     *  * * \FORALL (d : dato) d \IN obtener(c, registros(t)) \IMPLIES d \IN claves(obtener(c, obtener(t, _indices))) \LAND
     *  * * \FORALL (r : registro) r \IN registros(t) /IMPLIES r \IN obtener(d, obtener(c, obtener(t, _indices))))
     *  * \LAND
     *  * \FORALL (t : tabla) t \IN claves(_indicesNum) \IMPLIES t \IN claves(_tablas) \LAND
     *  * * \FORALL (c : campo) c \IN campos(t) \IMPLIES c \IN claves(obtener(t, _indicesNum)) \LAND
     *  * * \FORALL (d : dato) d \IN obtener(c, registros(t)) \IMPLIES d \IN claves(obtener(c, obtener(t, _indicesNum))) \LAND
     *  * * \FORALL (r : registro) r \IN registros(t) /IMPLIES r \IN obtener(d, obtener(c, obtener(t, _indicesNum))))
     *
     *
     * abs: basededatos \TO BaseDeDatos\n
     * abs(bd) \EQUIV bd' \|
     *  * _nombres_tablas = tablas(bd') \LAND
     *  * (\FORALL nt : string) nt \IN _nombres_tablas \IMPLIES
     *    * obtener(nt, _tablas) = dameTabla(nt, bd') \LAND
     *  * (\FORALL c : criterio) 
     *    * (usoCriterio(c, bd') == 0 \LAND \LNOT def?(c, _uso_criterios)) \LOR
     *    * (usoCriterio(c, db') == obtener(c, _uso_criterios)) \LAND
     *  * (\FORALL c : campos),(t : tablas) \IMPLIES tieneIndice?(c,t,bd) =
     *    * def?(t, _indicesNum) \LAND def?(c, obtener(t,_indicesNum)) \LAND (\FORALL cl: claves) cl \IN c
     *    * \IMPLIES def?(cl, obtener(c, obtener(t,_indicesNum)) \LOR
     *    * def?(t, _indices) \LAND def?(c, obtener(t,_indices)) \LAND (\FORALL cl: claves) cl \IN c
     *    * \IMPLIES def?(cl, obtener(c, obtener(t,_indices))
     */
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    /** @{ */
    linear_set<string> _nombres_tablas;
    string_map<Tabla> _tablas;
    linear_map<Criterio, int> _uso_criterios;
    Index _indices;
    IndexInt _indicesNum;
    /** @} */

    /** @{ */
    /**
     * @brief Filtra la lista de registros parametro según el criterio.
     *
     * El resultado tiene aliasing con el parámetro registros.
     *
     * \pre \FORALL (r : Registro) r \IN registros \IMPLIES campo \IN
     *      campos(r) \LAND tipo?(valor(campo, r)) = tipo?(valor)
     * \post \P{res} = filtrarRegistrosSegunRestriccion(
     *       nueva(campo, valor, igualdad), registros)
     */
    list<Registro> &_filtrar_registros(const string &campo, const Datum &valor,
                                       list<Registro> &registros,
                                       bool igualdad) const;

    /**
     * @brief Filtra la lista de registros parametro según el criterio.
     *
     * El resultado tiene aliasing con el parámetro registros.
     *
     * \pre \FORALL (r : Registro) r \IN registros \IMPLIES campo \IN
     *      campos(r) \LAND tipo?(valor(campo, r)) = tipo?(valor)
     * \post \P{res} = filtrarRegistrosSegunRestriccion(
     *       nueva(campo, valor, true), registros)
     */
    list<Registro> &_filtrar_registros(const string &campo, const Datum &valor,
                                       list<Registro> &registros) const;

    /**
     * @brief Obtiene los campos y tipos de una tabla.
     *
     * \pre true
     * \post (\FORALL (c : Campo) está?(c, \P1(\P{res})) \IFF c \IN campos(t))
     *       \LAND #(campos(t)) = long(\P1(\P{res}))
     *       \LAND \FORALL (i : Nat) 0 \LEQ i < #(campos(t)) \IMPLIES
     *       tipo?(\P2(\P{res})[i]) = tipoCampo(\P1(\P{res})[i], t)
     */
    pair<vector<string>, vector<Datum> > _tipos_tabla(const Tabla &t);
    /** @} */

    /**
     * @brief Join de dos tablas designando un campo en común.
     *
     * Tabla 1 tiene índice.
     *
     * @param tabla1 Nombre de la tabla 1.
     * @param tabla2 Nombre de la tabla 2.
     * @param campo Nombre del campo de las tablas para hacer el join.
     * @param orden indica el orden de las tablas, si la primera tiene indice o no.
     *
     * \pre db = \P{this} \LAND tabla1 \IN tablas(\P{this}) \LAND tabla2 \IN tablas(\P{this})
     *      \LAND campo \IN campos(tabla1) \LAND campo \IN campos(tabla2)
     *      \LAND tieneIndice?(tabla1, campo, db)
     * \post \P{res} = Join(tabla1, tabla2, campo, db)
     *
     * \complexity{\O(m * (L+log(m))}
     */
    join_iterator join_helper_str(const string &tabla1, const string &tabla2, const string &campo, const bool &orden);

    join_iterator join_helper_int(const string &tabla1, const string &tabla2, const string &campo, const bool &orden);
};

#endif
