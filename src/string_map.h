#ifndef STRING_MAP_STRING_MAP_H
#define STRING_MAP_STRING_MAP_H

#include <cassert>
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::pair;

/**
 * @brief Implementacion de map<string,T> sobre Trie
 *
 * Asume de T:
 * - tiene constructor por copia
 * - tiene operador ==
 * - solo permite utilizar el operator[] si T tiene constructor por defecto
 *
 * **se explica con** TAD Diccionario(string, T)
 */
template<typename T>
class string_map {
public:
    typedef string key_type;
    typedef T mapped_type;
    typedef std::pair<const key_type, mapped_type> value_type;
    typedef size_t size_type;

private:
    struct Nodo;

public:
    class iterator {
    public:
        typedef string key_type;
        typedef T mapped_type;
        typedef std::pair<const key_type, mapped_type> value_type;

        /**
        * @brief Constructor por defecto del iterador.
        *
        * \complexity{\O(1)}
        */
        iterator() {
            nodo = nullptr;
        }

        /**
        * @brief Constructor por copia del iterador.
        *
        * \complexity{\O(1)}
        */
        iterator(const iterator &n) {
            nodo = n.nodo;
        }

        /**
        * @brief Operador de asignacion.
        *
        * \complexity{\O(1)}
        */
        iterator &operator=(const iterator &n) {
            nodo = n.nodo;
            return *this;
        }

        ~iterator() {
            nodo = nullptr;
        }

        /** @brief Operador de comparacion
         *  @param \P{n}
         *
         * \pre true
         * \post res == (first(par(nodo)) = first(par(\P{n}.nodo)) \LAND second(par(nodo)) = second(par(\P{n}.nodo)))
         *
         * \complexity{\O(1)}
         */
        bool operator==(const iterator &n) const {
            if (nodo == nullptr || n.nodo == nullptr) {
                return nodo == n.nodo;
            }
            if (nodo->par == nullptr || n.nodo->par == nullptr) {
                return nodo->par == n.nodo->par;
            }
            return *(nodo->par) == *(n.nodo->par);
        }

        /** @brief Operador de comparacion
         *  @param \P{n}
         *
         * \pre true
         * \post res != (first(par(nodo)) = first(par(\P{n}.nodo)) \LAND second(par(nodo)) = second(par(\P{n}.nodo)))
         *
         * \complexity{\O(1)}
         */
        bool operator!=(const iterator &n) const {
            return !(*this == n);
        }

        /**
         * @brief Avanza el iterador una posición.
         *
         * \pre El iterador no debe estar en la posición pasando-el-último.
         * \post \P{res} es una referencia a \P{this}. \P{this} apunta a la posición
         * siguiente.
         *
         * \complexity{\O(1)}
         */
        iterator &operator++() {
            Nodo *nu = proximoAbajo(nodo);
            if (nu != nullptr) {
                nodo = nu;
                return *this;
            } else {
                nu = proximoArriba(nodo);
                nodo = nu;
                return *this;
            }
        }

        /**
         * @brief Avanza el iterador n posiciones.
         *
         * \pre El iterador no debe estar en la posición pasando-el-ante-n-último.
         * \post \P{res} es una referencia a \P{this}. \P{this} apunta a la posición
         * siguienten veces.
         *
         * \complexity{\O(1)}
         */
        iterator operator++(int) {
            auto copia = *this;
            ++*this;
            return copia;
        }

        /**
         * @brief Desreferencia el puntero
         *
         * El valor devuelto tiene aliasing dentro de la colección.
         *
         * \pre El iterador no debe estar en la posición pasando-el-último.
         * \post El valor resultado es una referencia constante al valor apuntado.
         *
         * \complexity{\O(1)}
         */
        value_type &operator*() const {
            return *nodo->par;
        }

        /**
         * @brief Operador flechita
         *
         * El valor devuelvo tiene aliasing dentro de la colección.
         *
         * \pre El iterador no debe estar en la posición pasando-el-último.
         * \post El valor resultado es un puntero al valor apuntado.
         *
         * \complexity{\O(1)}
         */
        value_type *operator->() const {
            return nodo->par;
        }

        friend class string_map;

    private:
        iterator(Nodo *n) : nodo(n) {};
        Nodo *nodo;

        typename string_map<T>::Nodo *proximoAbajo(Nodo *n);
        void buscarNodoAbajo(Nodo *&n);
        typename string_map<T>::Nodo *proximoArriba(Nodo *n);
        void buscarNodoArriba(Nodo *&n);
    };

    class const_iterator {
    public:
        typedef string key_type;
        typedef T mapped_type;
        typedef std::pair<const key_type, mapped_type> value_type;

        /**
        * @brief Constructor por defecto del iterador.
        *
        * \complexity{\O(1)}
        */
        const_iterator() {
            nodo = nullptr;
        }

        /**
        * @brief Constructor por copia del iterador.
        *
        * \complexity{\O(1)}
        */
        const_iterator(const const_iterator &n) {
            nodo = n.nodo;
        }

        ~const_iterator() {
            nodo = nullptr;
        }

        /**
        * @brief Operador de asignacion.
        *
        * \complexity{\O(sn)}
        */
        const_iterator &operator=(const const_iterator &n) {
            nodo = n.nodo;
            return *this;
        }

        /** @brief Operador de comparacion
         *  @param \P{n}
         *
         * \pre true
         * \post res == (first(par(nodo)) = first(par(\P{n}.nodo)) \LAND first(par(nodo)) = first(par(\P{n}.nodo)))
         *
         * \complexity{\O(1)}
         */
        bool operator==(const const_iterator &n) const {
            if (nodo == nullptr || n.nodo == nullptr) {
                return nodo == n.nodo;
            }
            if (nodo->par == nullptr || n.nodo->par == nullptr) {
                return nodo->par == n.nodo->par;
            }
            return *(nodo->par) == *(n.nodo->par);
        }

        /** @brief Operador de comparacion
         *  @param \P{n}
         *
         * \pre true
         * \post res != (first(par(nodo)) = first(par(\P{n}.nodo)) \LAND first(par(nodo)) = first(par(\P{n}.nodo)))
         *
         * \complexity{\O(1)}
         */
        bool operator!=(const const_iterator &n) const {
            return !(*this == n);
        }

        /**
         * @brief Avanza el iterador una posición.
         *
         * \pre El iterador no debe estar en la posición pasando-el-último.
         * \post \P{res} es una referencia a \P{this}. \P{this} apunta a la posición
         * siguiente.
         *
         * \complexity{\O(S)}
         */
        const_iterator &operator++() {
            Nodo *nu = proximoAbajo(nodo);
            if (nu != nullptr) {
                nodo = nu;
                return *this;
            } else {
                nu = proximoArriba(nodo);
                nodo = nu;
                return *this;
            }
        }

        /**
         * @brief Avanza el iterador n posiciones.
         *
         * \pre El iterador no debe estar en la posición pasando-el-ante-n-último.
         * \post \P{res} es una referencia a \P{this}. \P{this} apunta a la posición
         * siguienten veces.
         *
         * \complexity{\O(S)}
         */
        const_iterator operator++(int) {
            auto copia = *this;
            ++*this;
            return copia;
        }

        /**
         * @brief Desreferencia el puntero
         *
         * El valor devuelto tiene aliasing dentro de la colección.
         *
         * \pre El iterador no debe estar en la posición pasando-el-último.
         * \post El valor resultado es una referencia constante al valor apuntado.
         *
         * \complexity{\O(1)}
         */
        const value_type &operator*() const {
            return *nodo->par;
        }

        /**
         * @brief Operador flechita
         *
         * El valor devuelvo tiene aliasing dentro de la colección.
         *
         * \pre El iterador no debe estar en la posición pasando-el-último.
         * \post El valor resultado es un puntero al valor apuntado.
         *
         * \complexity{\O(1)}
         */
        const value_type *operator->() const {
            return nodo->par;
        }

        friend class string_map;

    private:
        const_iterator(Nodo *n) : nodo(n) {}

        Nodo *nodo;

        typename string_map<T>::Nodo *proximoAbajo(Nodo *n);
        void buscarNodoAbajo(Nodo *&n);
        typename string_map<T>::Nodo *proximoArriba(Nodo *n);
        void buscarNodoArriba(Nodo *&n);
    };

    /** @brief Construye mapa vacio
     *
     * \pre true
     * \post \P{this} = nuevoSM
     *
     * \complexity{\O(1)}
     */
    string_map();

    /** @brief Destruye mapa
     *
     * \complexity{\O(sn * S)}
     */
    ~string_map();

    /** @brief Constructor por copia
     *
     * \pre true
     * \post \P{this} = nuevoSM \LAND \P{this} = p
     * \complexity{\O(sn * S)}
     */
    string_map(const string_map &p);

    /** @brief Operador de asignacion
     *  \pre true
     *  \post \P{this} = p
     *
     * \complexity{\O(sn * S)}
     */
    string_map &operator=(const string_map &p);

    /** @brief Operadores de comparacion
     *
     * \pre true
     * \post \P{this} == p
     * \complexity{\O(sn * S)}
     */
    bool operator==(const string_map &otro) const;

    /** @brief Operadores de comparacion
     *
     * \pre true
     * \post \P{this} != p
     * \complexity{\O(sn * S)}
     */
    bool operator!=(const string_map &otro) const;

    /** @brief Cantidad de apariciones de la clave (0 o 1)
     *  @param key clave a buscar
     *
     *  \pre true
     *  \post if key \IN claves(\P{this}) then
     *              \P{res} = 1
     *          else
     *              \P{res} = 0
     *       fi
     *
     *  \complexity{\O(S)}
     */
    size_type count(const key_type &key) const;

    /** @brief Devuelve cantidad de claves definidas
     *
     *  \pre true
     *  \post \P{res} = #claves(\P{this})
     *
     */
    size_t size() const;

    /** @brief Permite saber si es vacio (no tiene ninguna clave definida)
     *
     *  \pre true
     *  \post  if #claves(\P{this}) = 0 then
     *              \P{res} = true
     *         else
     *              \P{res} = false
     *       fi
     *
     */
    bool empty() const;

    /** @brief Acceso / definición de pares clave/valor
     *  @param key clave a acceder, si no existe, se crea
     *  @returns una referencia a la definicion.
     *
     *  \complexity{\O(S)}
     */
    mapped_type &operator[](const key_type &key);

    /** @brief Acceso a una clave sin modificar mapa
     *  @param key clave a acceder que debe existir previamente
     *  @returns una referencia a la definicion.
     *
     *  \complexity{\O(S)}
     */
    mapped_type &at(const key_type &key);

    /** @brief Acceso a una clave sin modificar mapa
     *  @param key clave a acceder que debe existir previamente
     *  @returns una referencia const a la definicion.
     *
     *  \complexity{\O(S)}
     */
    const mapped_type &at(const key_type &key) const;

    /** @brief Vacia el mapa */
    void clear();

    // Accesos con iteradores

    /** @brief iterador al primer par <clave,significado> en orden lexicografico
     *  @returns iterador al elemento o end() si el mapa era vacio
     *
     *  \complexity{\O(S)}
     */
    iterator begin();

    /**  @brief iterador al fin de la coleccion
     *
     *  \complexity{\O(S)}
     */
    iterator end();

    /// Versiones const de begin/end
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;

    /** @brief busca una clave
     *  @param key clave a buscar
     *  @returns un iterador al par <clave, significado>, si la clave no existe devuelve el end
     *
     *  \complexity{\O(S)}
     */
    iterator find(const key_type &key);

    /** @brief busca una clave
     *  @param key clave a buscar
     *  @returns un iterador const al par <clave, significado>, si la clave no existe devuelve el end
     *
     *  \complexity{\O(S)}
     */
    const_iterator find(const key_type &key) const;

    /** @brief insercion
     *
     * @param value par <clave,significado> a insertar
     * @returns un par con un iterador al par clave-significado agregado o
     * modificado y un bool que indica si la clave se insertó como una clave
     * nueva.
     *
     * \complexity{\O(S + copy(T))}
     */
    pair<iterator, bool> insert(const value_type &value);

    /** @brief eliminar una clave
     *  @param key clave a eliminar
     *  @returns cantidad de elementos eliminados
     *
     *  \complexity{\O(S)}
     */
    size_type erase(const key_type &key);

    /** @brief eliminar una clave mediante iterador
     *  @param pos iterador apuntando a clave a eliminar
     *  @returns iterador apuntando el proximo de la clave eliminada (o end() si era la ultima)
     *
     *  \complexity{\O(S)}
     */
    iterator erase(iterator pos);

private:
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    /** \name Representación
     *
     * String Map se representa con tupla<head: puntero(Nodo), cantClaves: nat>
     *
     * donde Nodo es tupla <clave: string, padre: puntero(Nodo), hijos: arreglo(puntero(Nodo)), cantHijos: nat, par:
     * tupla(string, T)>>
     *
     * rep: string_map sm \TO bool\n
     * rep(sm) \EQUIV
     * * sm.cantClaves == #clavesSM(sm.head) \LAND
     * * cantHijosOk(sm.head) \LAND
     * * consistenciaPadreHijo(sm.head, sm.head.hijos) \LAND
     * * caminoValidoClaveDePar(sm.head, sm.head, sm.head.hijos)
     *
     * abs: string_map sm \TO diccionario(string, T)\n
     * abs(sm) \EQUIV d : diccionario \|
     * * claves(d) == clavesSM(sm.head) \LAND
     * * (\FORALL cl \IN claves(d)) (definido(cl, sm) \LAND obtener(cl, d) == significado(cl, sm))
     *
     */
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    struct Nodo {
        string clave;
        Nodo *padre;
        vector<Nodo *> hijos;
        int cantHijos;
        value_type *par;

        Nodo() : clave(), padre(nullptr), hijos(128, nullptr), cantHijos(), par(nullptr) {}

        Nodo(const Nodo &n) : clave(n.clave), padre(n.padre), hijos(n.hijos), cantHijos(n.cantHijos), par(n.par) {}

        bool operator==(const Nodo &rhs) const {
            return clave == rhs.clave &&
                   padre == rhs.padre &&
                   hijos == rhs.hijos &&
                   cantHijos == rhs.cantHijos &&
                   par == rhs.par;
        }

        bool operator!=(const Nodo &rhs) const {
            return !(rhs == *this);
        }

    };

    Nodo *head;
    size_t cantClaves = 0;

    void borrar(Nodo *&n);
    void copiarHijos(Nodo *head, Nodo *other);
};

#include "string_map.hpp"

#endif //STRING_MAP_STRING_MAP_H
