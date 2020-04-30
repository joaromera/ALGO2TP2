#pragma once

#include <cassert>
#include <string>
#include <vector>

template<typename T>
class string_map
{
public:
  typedef std::string key_type;
  typedef T mapped_type;
  typedef std::pair<const key_type, mapped_type> value_type;
  typedef size_t size_type;

private:
  struct Nodo;

public:
  class iterator
  {
  public:
    typedef std::string key_type;
    typedef T mapped_type;
    typedef std::pair<const key_type, mapped_type> value_type;

    iterator()
    {
      nodo = nullptr;
    }

    iterator(const iterator &n)
    {
      nodo = n.nodo;
    }

    iterator &operator=(const iterator &n)
    {
      nodo = n.nodo;
      return *this;
    }

    ~iterator()
    {
      nodo = nullptr;
    }

    bool operator==(const iterator &n) const
    {
      if (nodo == nullptr || n.nodo == nullptr)
      {
        return nodo == n.nodo;
      }
      if (nodo->par == nullptr || n.nodo->par == nullptr)
      {
        return nodo->par == n.nodo->par;
      }
      return *(nodo->par) == *(n.nodo->par);
    }

    bool operator!=(const iterator &n) const
    {
      return !(*this == n);
    }

    iterator &operator++()
    {
      Nodo *nu = proximoAbajo(nodo);
      if (nu != nullptr)
      {
        nodo = nu;
        return *this;
      }
      else
      {
        nu = proximoArriba(nodo);
        nodo = nu;
        return *this;
      }
    }

    iterator operator++(int)
    {
      auto copia = *this;
      ++*this;
      return copia;
    }

    value_type &operator*() const
    {
      return *nodo->par;
    }

    value_type *operator->() const
    {
      return nodo->par;
    }

    friend class string_map;

  private:
    iterator(Nodo *n) : nodo(n){};
    Nodo *nodo;

    typename string_map<T>::Nodo *proximoAbajo(Nodo *n);
    void buscarNodoAbajo(Nodo *&n);
    typename string_map<T>::Nodo *proximoArriba(Nodo *n);
    void buscarNodoArriba(Nodo *&n);
  };

  class const_iterator
  {
  public:
    typedef std::string key_type;
    typedef T mapped_type;
    typedef std::pair<const key_type, mapped_type> value_type;

    const_iterator()
    {
      nodo = nullptr;
    }

    const_iterator(const const_iterator &n)
    {
      nodo = n.nodo;
    }

    ~const_iterator()
    {
      nodo = nullptr;
    }

    const_iterator &operator=(const const_iterator &n)
    {
      nodo = n.nodo;
      return *this;
    }

    bool operator==(const const_iterator &n) const
    {
      if (nodo == nullptr || n.nodo == nullptr)
      {
        return nodo == n.nodo;
      }
      if (nodo->par == nullptr || n.nodo->par == nullptr)
      {
        return nodo->par == n.nodo->par;
      }
      return *(nodo->par) == *(n.nodo->par);
    }

    bool operator!=(const const_iterator &n) const
    {
      return !(*this == n);
    }

    const_iterator &operator++()
    {
      Nodo *nu = proximoAbajo(nodo);
      if (nu != nullptr)
      {
        nodo = nu;
        return *this;
      }
      else
      {
        nu = proximoArriba(nodo);
        nodo = nu;
        return *this;
      }
    }

    const_iterator operator++(int)
    {
      auto copia = *this;
      ++*this;
      return copia;
    }

    const value_type &operator*() const
    {
      return *nodo->par;
    }

    const value_type *operator->() const
    {
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

  string_map();

  ~string_map();

  string_map(const string_map &p);

  string_map &operator=(const string_map &p);

  bool operator==(const string_map &otro) const;

  bool operator!=(const string_map &otro) const;

  size_type count(const key_type &key) const;

  size_t size() const;

  bool empty() const;

  mapped_type &operator[](const key_type &key);

  mapped_type &at(const key_type &key);

  const mapped_type &at(const key_type &key) const;

  void clear();

  iterator begin();

  iterator end();

  /// Versiones const de begin/end
  const_iterator begin() const;
  const_iterator end() const;
  const_iterator cbegin() const;
  const_iterator cend() const;

  iterator find(const key_type &key);

  const_iterator find(const key_type &key) const;

  std::pair<iterator, bool> insert(const value_type &value);

  size_type erase(const key_type &key);

  iterator erase(iterator pos);

private:
  struct Nodo
  {
    std::string clave;
    Nodo *padre;
    std::vector<Nodo *> hijos;
    int cantHijos;
    value_type *par;

    Nodo() : clave(), padre(nullptr), hijos(128, nullptr), cantHijos(), par(nullptr) {}

    Nodo(const Nodo &n) : clave(n.clave), padre(n.padre), hijos(n.hijos), cantHijos(n.cantHijos), par(n.par) {}

    bool operator==(const Nodo &rhs) const
    {
      return clave == rhs.clave && padre == rhs.padre && hijos == rhs.hijos && cantHijos == rhs.cantHijos && par == rhs.par;
    }

    bool operator!=(const Nodo &rhs) const
    {
      return !(rhs == *this);
    }
  };

  Nodo* head = nullptr;
  size_t cantClaves = 0;

  void borrar(Nodo *&n);
  void copiarHijos(Nodo *head, Nodo *other);
};

#include "string_map.hpp"
