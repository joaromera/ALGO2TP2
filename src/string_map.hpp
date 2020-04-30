#pragma once

#include <utility>

template<typename T>
string_map<T>::string_map()
{
  head = new Nodo;
}

template<typename T>
string_map<T>::~string_map()
{
  borrar(head);
  head = nullptr;
}

template<typename T>
void string_map<T>::borrar(Nodo *&n)
{
  // Recursivamente recorre todos los hijos del nodo, liberando memoria y seteando a nullptr
  Nodo *node = n;
  for (int i = 0; i < 128; i++)
  {
    if (node->hijos[i] != nullptr)
    {
      borrar(node->hijos[i]);
      node->hijos[i] = nullptr;
      node->cantHijos--;
    }
  }
  if (node->par != nullptr)
  {
    delete node->par;
    node->par = nullptr;
    cantClaves--;
  }
  if (node->padre != nullptr)
  {
    node->padre = nullptr;
  }
  delete node;
  node = nullptr;
}

template<typename T>
string_map<T>::string_map(const string_map &p)
{
  head = new Nodo;
  head->clave = p.head->clave;
  cantClaves = p.cantClaves;
  copiarHijos(head, p.head);
}

template<typename T>
void string_map<T>::copiarHijos(Nodo *head, Nodo *other)
{
  if (other != nullptr)
  {
    for (int i = 0; i < 128; i++)
    {
      if (other->hijos[i] != nullptr)
      {
        head->cantHijos++;
        head->hijos[i] = new Nodo;
        head->hijos[i]->padre = head;
        copiarHijos(head->hijos[i], other->hijos[i]);
      }
    }
    head->clave = other->clave;
    if (other->par != nullptr)
    {
      key_type key = other->par->first;
      mapped_type value = other->par->second;
      head->par = new value_type(key, value);
    }
  }
}

template<typename T>
string_map<T> &string_map<T>::operator=(const string_map &p)
{
  this->clear();

  if (p.head == nullptr)
  {
    head = nullptr;
  }
  else
  {
    if (p.head->par != nullptr) { head->par = new value_type(p.head->par->first, p.head->par->second); }
    copiarHijos(head, p.head);
  }
  head->clave = p.head->clave;
  cantClaves = p.cantClaves;

  return *this;
}

template<typename T>
bool string_map<T>::operator==(const string_map &otro) const
{
  auto it1 = begin();
  auto it2 = otro.begin();
  auto itend1 = end();
  auto itend2 = otro.end();
  while (it1 != itend1 && it2 != itend2)
  {
    if (it1 != it2) return false;
    it1++;
    it2++;
  }
  if (it1 != itend1 || it2 != itend2) return false;

  return true;
}

template<typename T>
bool string_map<T>::operator!=(const string_map &otro) const
{
  return !(*this == otro);
}

template<typename T>
typename string_map<T>::Nodo *string_map<T>::iterator::proximoAbajo(Nodo *n)
{
  // Busca recursivamente el primer nodo no nulo y con par definido
  // Si no hay ninguno n -> nullptr
  Nodo *tmp = n;
  buscarNodoAbajo(tmp);
  return tmp;
}

template<typename T>
void string_map<T>::iterator::buscarNodoAbajo(Nodo *&n)
{
  // Busca recursivamente el primer nodo no nulo y con par definido
  // Si no hay ninguno n -> nullptr
  int i = 0;
  while (i < 128 && n->hijos[i] == nullptr) i++;
  if (i == 128)
  {
    n = nullptr;
  }
  else if (n->hijos[i]->par != nullptr)
  {
    n = n->hijos[i];
  }
  else
  {
    n = n->hijos[i];
    buscarNodoAbajo(n);
  }
}

template<typename T>
typename string_map<T>::Nodo *string_map<T>::iterator::proximoArriba(Nodo *n)
{
  // Busca recursivamente el primer nodo no nulo y con par definido
  // Si no hay ninguno n -> nullptr
  Nodo *tmp = n;
  buscarNodoArriba(tmp);
  return tmp;
}

template<typename T>
void string_map<T>::iterator::buscarNodoArriba(Nodo *&n)
{
  if (n->padre == nullptr) n = nullptr;
  if (n != nullptr)
  {
    Nodo *proximo = n->padre;
    char clave = n->clave.back();
    int i = clave + 1;
    while (i < 128 && proximo->hijos[i] == nullptr) i++;
    if (i != 128)
    {
      n = proximo->hijos[i];
      if (n->par == nullptr)
      {
        buscarNodoAbajo(n);
      }
    }
    else
    {
      n = proximo;
      buscarNodoArriba(n);
    }
  }
}

template<typename T>
typename string_map<T>::Nodo *string_map<T>::const_iterator::proximoAbajo(Nodo *n)
{
  // Busca recursivamente el primer nodo no nulo y con par definido
  // Si no hay ninguno n -> nullptr
  Nodo *tmp = n;
  buscarNodoAbajo(tmp);
  return tmp;
}

template<typename T>
void string_map<T>::const_iterator::buscarNodoAbajo(Nodo *&n)
{
  // Busca recursivamente el primer nodo no nulo y con par definido
  // Si no hay ninguno n -> nullptr
  int i = 0;
  while (i < 128 && n->hijos[i] == nullptr) i++;
  if (i == 128)
  {
    n = nullptr;
  }
  else if (n->hijos[i]->par != nullptr)
  {
    n = n->hijos[i];
  }
  else
  {
    n = n->hijos[i];
    buscarNodoAbajo(n);
  }
}

template<typename T>
typename string_map<T>::Nodo *string_map<T>::const_iterator::proximoArriba(Nodo *n)
{
  // Busca recursivamente el primer nodo no nulo y con par definido
  // Si no hay ninguno n -> nullptr
  Nodo *tmp = n;
  buscarNodoArriba(tmp);
  return tmp;
}

template<typename T>
void string_map<T>::const_iterator::buscarNodoArriba(Nodo *&n)
{
  // Busca los hermanos mayores lexicograficamente de n
  if (n->padre == nullptr) n = nullptr;
  if (n != nullptr)
  {
    Nodo *proximo = n->padre;
    char clave = n->clave.back();
    int i = clave + 1;
    while (i < 128 && proximo->hijos[i] == nullptr) i++;
    if (i != 128)
    {
      n = proximo->hijos[i];
      if (n->par == nullptr)
      {
        buscarNodoAbajo(n);
      }
    }
    else
    {
      n = proximo;
      buscarNodoArriba(n);
    }
  }
}

template<typename T>
size_t string_map<T>::count(const key_type &key) const
{
  Nodo *walk = head;
  for (const char& c : key)
  {
    if (walk->hijos[int(c)] == nullptr)
    {
      return 0;
    }
    else
    {
      walk = walk->hijos[int(c)];
    }
  }
  if (walk->clave == key && walk->par != nullptr)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

template<typename T>
size_t string_map<T>::size() const
{
  return cantClaves;
}

template<typename T>
bool string_map<T>::empty() const
{
  return size() == 0;
}

template<typename T>
typename string_map<T>::mapped_type &string_map<T>::operator[](const key_type &key)
{
  Nodo *temp = head;
  for (auto it = key.begin(); it < key.end(); it++)
  {
    if (temp->hijos[int(*it)] == nullptr)
    {
      temp->cantHijos++;
      Nodo *nuevo = new Nodo;
      temp->hijos[int(*it)] = nuevo;
      temp->hijos[int(*it)]->padre = temp;
    }
    temp = temp->hijos[int(*it)];
    temp->clave = *it;
  }
  temp->clave = key;
  if (temp->par == nullptr)
  {
    temp->par = new value_type(key, T());
    cantClaves++;
  }
  return temp->par->second;
}

template<typename T>
typename string_map<T>::mapped_type &string_map<T>::at(const key_type &key)
{
  auto it = find(key);
  return it.nodo->par->second;
}

template<typename T>
const typename string_map<T>::mapped_type &string_map<T>::at(const key_type &key) const
{
  auto it = find(key);
  return it.nodo->par->second;
}

template<typename T>
void string_map<T>::clear()
{
  if (head != nullptr)
  {
    borrar(head);
  }
  head = new Nodo;
  cantClaves = 0;
}

template<typename T>
typename string_map<T>::iterator string_map<T>::begin()
{
  if (empty()) return end();
  Nodo *walk = head;
  while (walk->par == nullptr)
  {
    int i = 0;
    while (i < 128 && walk->hijos[i] == nullptr) i++;
    if (walk->hijos[i] != nullptr) walk = walk->hijos[i];
  }
  auto it = iterator(walk);
  return it;
}

template<typename T>
typename string_map<T>::iterator string_map<T>::end()
{
  return nullptr;
}

template<typename T>
typename string_map<T>::const_iterator string_map<T>::begin() const
{
  if (empty()) return end();
  Nodo *walk = head;
  while (walk->par == nullptr)
  {
    int i = 0;
    while (i < 128 && walk->hijos[i] == nullptr) i++;
    if (walk->hijos[i] != nullptr) walk = walk->hijos[i];
  }
  auto it = const_iterator(walk);
  return it;
}

template<typename T>
typename string_map<T>::const_iterator string_map<T>::end() const
{
  return nullptr;
}

template<typename T>
typename string_map<T>::const_iterator string_map<T>::cbegin() const
{
  return const_iterator(begin());
}

template<typename T>
typename string_map<T>::const_iterator string_map<T>::cend() const
{
  return nullptr;
}

template<typename T>
typename string_map<T>::iterator string_map<T>::find(const key_type &key)
{
  Nodo *temp = head;
  for (auto it = key.begin(); it < key.end(); it++)
  {
    if (temp->hijos[int(*it)] == nullptr)
    {
      return end();
    }
    else
    {
      temp = temp->hijos[int(*it)];
    }
  }
  return iterator(temp);
}

template<typename T>
typename string_map<T>::const_iterator string_map<T>::find(const key_type &key) const
{
  Nodo *temp = head;
  for (auto it = key.begin(); it < key.end(); it++)
  {
    if (temp->hijos[int(*it)] == nullptr)
    {
      return this->end();
    }
    else
    {
      temp = temp->hijos[int(*it)];
    }
  }
  return const_iterator(temp);
}

template<typename T>
std::pair<typename string_map<T>::iterator, bool> string_map<T>::insert(const string_map::value_type &value)
{
  std::pair<iterator, bool> finish;
  bool mod = false;
  Nodo *current = head;
  std::string clave = value.first;

  for (auto it = clave.begin(); it < clave.end() - 1; it++)
  {
    if (current->hijos[int(*it)] != nullptr)
    {
      current = current->hijos[int(*it)];
    }
    else
    {
      current->cantHijos += 1;
      current->hijos[int(*it)] = new Nodo;
      current->hijos[int(*it)]->padre = current;
      current = current->hijos[int(*it)];
      current->clave = *it;
    }
  }

  char c = clave.back();

  if (current->hijos[int(c)] == nullptr)
  {
    cantClaves += 1;
    current->cantHijos += 1;
    current->hijos[int(c)] = new Nodo;
  }
  else
  {
    mod = true;
  }

  current->hijos[int(c)]->par = new value_type(clave, value.second);
  current->hijos[int(c)]->clave = clave;
  current->hijos[int(c)]->padre = current;

  iterator it = iterator(current->hijos[int(c)]);
  finish.first = it;
  finish.second = mod;
  return finish;
}

template<typename T>
typename string_map<T>::size_type string_map<T>::erase(const string_map<T>::key_type &key)
{
  auto it = this->find(key);
  Nodo *walk = it.nodo;
  int eliminados = 0;

  delete walk->par;
  walk->par = nullptr;
  walk->clave = key.back();
  cantClaves--;
  eliminados++;

  if (walk->cantHijos > 0) return eliminados;

  Nodo *actualizar = walk->padre;
  while (actualizar->cantHijos == 0 && actualizar->par == nullptr && actualizar->padre != nullptr)
  {
    actualizar->padre->hijos[int(actualizar->clave.back())] = nullptr;
    actualizar->padre->cantHijos -= 1;
    actualizar = actualizar->padre;
  }

  return eliminados;
}

template<typename T>
typename string_map<T>::iterator string_map<T>::erase(iterator pos)
{
  std::string clave = pos.nodo->clave;
  if (pos.nodo->cantHijos == 0)
  {

    Nodo *nodoaborrar = pos.nodo;
    Nodo *padredelnodo = pos.nodo->padre;

    delete nodoaborrar->par;
    nodoaborrar->par = nullptr;
    nodoaborrar->clave = clave.back();

    pos++;

    delete nodoaborrar;
    nodoaborrar = nullptr;

    padredelnodo->hijos[int(clave.back())] = nullptr;
    padredelnodo->cantHijos -= 1;
    clave = padredelnodo->clave;
    Nodo *actualizar = padredelnodo;
    padredelnodo = nullptr;

    while (actualizar->cantHijos == 0 && actualizar->par == nullptr && actualizar->padre != nullptr)
    {
      actualizar = actualizar->padre;

      delete actualizar->hijos[int(clave.back())];
      actualizar->hijos[int(clave.back())] = nullptr;
      actualizar->cantHijos -= 1;

      clave = actualizar->clave;
    }
    actualizar = nullptr;
    cantClaves -= 1;
    return pos;
  }
  else
  {
    delete pos.nodo->par;
    pos.nodo->par = nullptr;
    pos.nodo->clave = clave.back();
    cantClaves -= 1;
    pos++;
    return pos;
  }
}
