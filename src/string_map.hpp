#pragma once

#include <utility>

namespace Db {
namespace Types {

template<typename T>
string_map<T>::string_map()
{
  head = new Node;
}

template<typename T>
string_map<T>::~string_map()
{
  borrar(head);
  head = nullptr;
}

template<typename T>
void string_map<T>::borrar(Node *&n)
{
  // Recursivamente recorre todos los children del node, liberando memoria y seteando a nullptr
  Node *node = n;
  for (int i = 0; i < 128; i++)
  {
    if (node->children[i] != nullptr)
    {
      borrar(node->children[i]);
      node->children[i] = nullptr;
      node->cantHijos--;
    }
  }
  if (node->par != nullptr)
  {
    delete node->par;
    node->par = nullptr;
    cantClaves--;
  }
  if (node->parent != nullptr)
  {
    node->parent = nullptr;
  }
  delete node;
  node = nullptr;
}

template<typename T>
string_map<T>::string_map(const string_map &p)
{
  head = new Node;
  head->key = p.head->key;
  cantClaves = p.cantClaves;
  copiarHijos(head, p.head);
}

template<typename T>
void string_map<T>::copiarHijos(Node *head, Node *other)
{
  if (other != nullptr)
  {
    for (int i = 0; i < 128; i++)
    {
      if (other->children[i] != nullptr)
      {
        head->cantHijos++;
        head->children[i] = new Node;
        head->children[i]->parent = head;
        copiarHijos(head->children[i], other->children[i]);
      }
    }
    head->key = other->key;
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
  head->key = p.head->key;
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
typename string_map<T>::Node *string_map<T>::iterator::proximoAbajo(Node *n)
{
  // Busca recursivamente el primer node no nulo y con par definido
  // Si no hay ninguno n -> nullptr
  Node *tmp = n;
  buscarNodoAbajo(tmp);
  return tmp;
}

template<typename T>
void string_map<T>::iterator::buscarNodoAbajo(Node *&n)
{
  // Busca recursivamente el primer node no nulo y con par definido
  // Si no hay ninguno n -> nullptr
  int i = 0;
  while (i < 128 && n->children[i] == nullptr) i++;
  if (i == 128)
  {
    n = nullptr;
  }
  else if (n->children[i]->par != nullptr)
  {
    n = n->children[i];
  }
  else
  {
    n = n->children[i];
    buscarNodoAbajo(n);
  }
}

template<typename T>
typename string_map<T>::Node *string_map<T>::iterator::proximoArriba(Node *n)
{
  // Busca recursivamente el primer node no nulo y con par definido
  // Si no hay ninguno n -> nullptr
  Node *tmp = n;
  buscarNodoArriba(tmp);
  return tmp;
}

template<typename T>
void string_map<T>::iterator::buscarNodoArriba(Node *&n)
{
  if (n->parent == nullptr) n = nullptr;
  if (n != nullptr)
  {
    Node *proximo = n->parent;
    char clave = n->key.back();
    int i = clave + 1;
    while (i < 128 && proximo->children[i] == nullptr) i++;
    if (i != 128)
    {
      n = proximo->children[i];
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
typename string_map<T>::Node *string_map<T>::const_iterator::proximoAbajo(Node *n)
{
  // Busca recursivamente el primer node no nulo y con par definido
  // Si no hay ninguno n -> nullptr
  Node *tmp = n;
  buscarNodoAbajo(tmp);
  return tmp;
}

template<typename T>
void string_map<T>::const_iterator::buscarNodoAbajo(Node *&n)
{
  // Busca recursivamente el primer node no nulo y con par definido
  // Si no hay ninguno n -> nullptr
  int i = 0;
  while (i < 128 && n->children[i] == nullptr) i++;
  if (i == 128)
  {
    n = nullptr;
  }
  else if (n->children[i]->par != nullptr)
  {
    n = n->children[i];
  }
  else
  {
    n = n->children[i];
    buscarNodoAbajo(n);
  }
}

template<typename T>
typename string_map<T>::Node *string_map<T>::const_iterator::proximoArriba(Node *n)
{
  // Busca recursivamente el primer node no nulo y con par definido
  // Si no hay ninguno n -> nullptr
  Node *tmp = n;
  buscarNodoArriba(tmp);
  return tmp;
}

template<typename T>
void string_map<T>::const_iterator::buscarNodoArriba(Node *&n)
{
  // Busca los hermanos mayores lexicograficamente de n
  if (n->parent == nullptr) n = nullptr;
  if (n != nullptr)
  {
    Node *proximo = n->parent;
    char clave = n->key.back();
    int i = clave + 1;
    while (i < 128 && proximo->children[i] == nullptr) i++;
    if (i != 128)
    {
      n = proximo->children[i];
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
  Node *walk = head;
  for (const char& c : key)
  {
    if (walk->children[int(c)] == nullptr)
    {
      return 0;
    }
    else
    {
      walk = walk->children[int(c)];
    }
  }
  if (walk->key == key && walk->par != nullptr)
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
  Node *temp = head;
  for (auto it = key.begin(); it < key.end(); it++)
  {
    if (temp->children[int(*it)] == nullptr)
    {
      temp->cantHijos++;
      Node *nuevo = new Node;
      temp->children[int(*it)] = nuevo;
      temp->children[int(*it)]->parent = temp;
    }
    temp = temp->children[int(*it)];
    temp->key = *it;
  }
  temp->key = key;
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
  return it.node->par->second;
}

template<typename T>
const typename string_map<T>::mapped_type &string_map<T>::at(const key_type &key) const
{
  auto it = find(key);
  return it.node->par->second;
}

template<typename T>
void string_map<T>::clear()
{
  if (head != nullptr)
  {
    borrar(head);
  }
  head = new Node;
  cantClaves = 0;
}

template<typename T>
typename string_map<T>::iterator string_map<T>::begin()
{
  if (empty()) return end();
  Node *walk = head;
  while (walk->par == nullptr)
  {
    int i = 0;
    while (i < 128 && walk->children[i] == nullptr) i++;
    if (walk->children[i] != nullptr) walk = walk->children[i];
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
  Node *walk = head;
  while (walk->par == nullptr)
  {
    int i = 0;
    while (i < 128 && walk->children[i] == nullptr) i++;
    if (walk->children[i] != nullptr) walk = walk->children[i];
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
  Node *temp = head;
  for (auto it = key.begin(); it < key.end(); it++)
  {
    if (temp->children[int(*it)] == nullptr)
    {
      return end();
    }
    else
    {
      temp = temp->children[int(*it)];
    }
  }
  return iterator(temp);
}

template<typename T>
typename string_map<T>::const_iterator string_map<T>::find(const key_type &key) const
{
  Node *temp = head;
  for (auto it = key.begin(); it < key.end(); it++)
  {
    if (temp->children[int(*it)] == nullptr)
    {
      return this->end();
    }
    else
    {
      temp = temp->children[int(*it)];
    }
  }
  return const_iterator(temp);
}

template<typename T>
std::pair<typename string_map<T>::iterator, bool> string_map<T>::insert(const string_map::value_type &value)
{
  std::pair<iterator, bool> finish;
  bool mod = false;
  Node *current = head;
  std::string tmpKey = value.first;

  for (auto it = tmpKey.begin(); it < tmpKey.end() - 1; it++)
  {
    if (current->children[int(*it)] != nullptr)
    {
      current = current->children[int(*it)];
    }
    else
    {
      current->cantHijos += 1;
      current->children[int(*it)] = new Node;
      current->children[int(*it)]->parent = current;
      current = current->children[int(*it)];
      current->key = *it;
    }
  }

  char c = tmpKey.back();

  if (current->children[int(c)] == nullptr)
  {
    cantClaves += 1;
    current->cantHijos += 1;
    current->children[int(c)] = new Node;
  }
  else
  {
    mod = true;
  }

  current->children[int(c)]->par = new value_type(tmpKey, value.second);
  current->children[int(c)]->key = tmpKey;
  current->children[int(c)]->parent = current;

  iterator it = iterator(current->children[int(c)]);
  finish.first = it;
  finish.second = mod;
  return finish;
}

template<typename T>
typename string_map<T>::size_type string_map<T>::erase(const string_map<T>::key_type &key)
{
  auto it = this->find(key);
  Node *walk = it.node;
  int eliminados = 0;

  delete walk->par;
  walk->par = nullptr;
  walk->key = key.back();
  cantClaves--;
  eliminados++;

  if (walk->cantHijos > 0) return eliminados;

  Node *actualizar = walk->parent;
  while (actualizar->cantHijos == 0 && actualizar->par == nullptr && actualizar->parent != nullptr)
  {
    actualizar->parent->children[int(actualizar->key.back())] = nullptr;
    actualizar->parent->cantHijos -= 1;
    actualizar = actualizar->parent;
  }

  return eliminados;
}

template<typename T>
typename string_map<T>::iterator string_map<T>::erase(iterator pos)
{
  std::string clave = pos.node->key;
  if (pos.node->cantHijos == 0)
  {

    Node *nodoaborrar = pos.node;
    Node *padredelnodo = pos.node->parent;

    delete nodoaborrar->par;
    nodoaborrar->par = nullptr;
    nodoaborrar->key = clave.back();

    pos++;

    delete nodoaborrar;
    nodoaborrar = nullptr;

    padredelnodo->children[int(clave.back())] = nullptr;
    padredelnodo->cantHijos -= 1;
    clave = padredelnodo->key;
    Node *actualizar = padredelnodo;
    padredelnodo = nullptr;

    while (actualizar->cantHijos == 0 && actualizar->par == nullptr && actualizar->parent != nullptr)
    {
      actualizar = actualizar->parent;

      delete actualizar->children[int(clave.back())];
      actualizar->children[int(clave.back())] = nullptr;
      actualizar->cantHijos -= 1;

      clave = actualizar->key;
    }
    actualizar = nullptr;
    cantClaves -= 1;
    return pos;
  }
  else
  {
    delete pos.node->par;
    pos.node->par = nullptr;
    pos.node->key = clave.back();
    cantClaves -= 1;
    pos++;
    return pos;
  }
}

} // namespace Types
} // namespace Db
