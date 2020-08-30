#pragma once

#include <utility>

namespace Db {
namespace Types {

template<typename T>
string_map<T>::string_map()
{
  mTrieRoot = new Node;
}

template<typename T>
string_map<T>::~string_map()
{
  delete mTrieRoot;
  mTrieRoot = nullptr;
}

template<typename T>
string_map<T>::string_map(const string_map &p)
{
  mTrieRoot = new Node;
  mTrieRoot->mKey = p.mTrieRoot->mKey;
  mKeysCount = p.mKeysCount;
  copyChildren(mTrieRoot, p.mTrieRoot);
}

template<typename T>
void string_map<T>::copyChildren(Node *head, Node *other)
{
  if (other != nullptr)
  {
    for (int i = 0; i < 128; i++)
    {
      if (other->mChildren[i] != nullptr)
      {
        head->mSize++;
        head->mChildren[i] = new Node;
        head->mChildren[i]->mParent = head;
        copyChildren(head->mChildren[i], other->mChildren[i]);
      }
    }
    head->mKey = other->mKey;
    if (other->mValue != nullptr)
    {
      key_type key = other->mValue->first;
      mapped_type value = other->mValue->second;
      head->mValue = new value_type(key, value);
    }
  }
}

template<typename T>
string_map<T> &string_map<T>::operator=(const string_map &p)
{
  clear();

  if (p.mTrieRoot == nullptr)
  {
    mTrieRoot = nullptr;
  }
  else
  {
    if (p.mTrieRoot->mValue != nullptr) { mTrieRoot->mValue = new value_type(p.mTrieRoot->mValue->first, p.mTrieRoot->mValue->second); }
    copyChildren(mTrieRoot, p.mTrieRoot);
  }
  mTrieRoot->mKey = p.mTrieRoot->mKey;
  mKeysCount = p.mKeysCount;

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
  // Busca recursivamente el primer node no nulo y con mValue definido
  // Si no hay ninguno n -> nullptr
  Node *tmp = n;
  buscarNodoAbajo(tmp);
  return tmp;
}

template<typename T>
void string_map<T>::iterator::buscarNodoAbajo(Node *&n)
{
  // Busca recursivamente el primer node no nulo y con mValue definido
  // Si no hay ninguno n -> nullptr
  int i = 0;
  while (i < 128 && n->mChildren[i] == nullptr) i++;
  if (i == 128)
  {
    n = nullptr;
  }
  else if (n->mChildren[i]->mValue != nullptr)
  {
    n = n->mChildren[i];
  }
  else
  {
    n = n->mChildren[i];
    buscarNodoAbajo(n);
  }
}

template<typename T>
typename string_map<T>::Node *string_map<T>::iterator::proximoArriba(Node *n)
{
  // Busca recursivamente el primer node no nulo y con mValue definido
  // Si no hay ninguno n -> nullptr
  Node *tmp = n;
  buscarNodoArriba(tmp);
  return tmp;
}

template<typename T>
void string_map<T>::iterator::buscarNodoArriba(Node *&n)
{
  if (n->mParent == nullptr) n = nullptr;
  if (n != nullptr)
  {
    Node *proximo = n->mParent;
    char clave = n->mKey.back();
    int i = clave + 1;
    while (i < 128 && proximo->mChildren[i] == nullptr) i++;
    if (i != 128)
    {
      n = proximo->mChildren[i];
      if (n->mValue == nullptr)
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
  // Busca recursivamente el primer node no nulo y con mValue definido
  // Si no hay ninguno n -> nullptr
  Node *tmp = n;
  buscarNodoAbajo(tmp);
  return tmp;
}

template<typename T>
void string_map<T>::const_iterator::buscarNodoAbajo(Node *&n)
{
  // Busca recursivamente el primer node no nulo y con mValue definido
  // Si no hay ninguno n -> nullptr
  int i = 0;
  while (i < 128 && n->mChildren[i] == nullptr) i++;
  if (i == 128)
  {
    n = nullptr;
  }
  else if (n->mChildren[i]->mValue != nullptr)
  {
    n = n->mChildren[i];
  }
  else
  {
    n = n->mChildren[i];
    buscarNodoAbajo(n);
  }
}

template<typename T>
typename string_map<T>::Node *string_map<T>::const_iterator::proximoArriba(Node *n)
{
  // Busca recursivamente el primer node no nulo y con mValue definido
  // Si no hay ninguno n -> nullptr
  Node *tmp = n;
  buscarNodoArriba(tmp);
  return tmp;
}

template<typename T>
void string_map<T>::const_iterator::buscarNodoArriba(Node *&n)
{
  // Busca los hermanos mayores lexicograficamente de n
  if (n->mParent == nullptr) n = nullptr;
  if (n != nullptr)
  {
    Node *proximo = n->mParent;
    char clave = n->mKey.back();
    int i = clave + 1;
    while (i < 128 && proximo->mChildren[i] == nullptr) i++;
    if (i != 128)
    {
      n = proximo->mChildren[i];
      if (n->mValue == nullptr)
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
  Node *walk = mTrieRoot;
  for (const char& c : key)
  {
    if (walk->mChildren[int(c)] == nullptr)
    {
      return 0;
    }
    else
    {
      walk = walk->mChildren[int(c)];
    }
  }
  if (walk->mKey == key && walk->mValue != nullptr)
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
  return mKeysCount;
}

template<typename T>
bool string_map<T>::empty() const
{
  return size() == 0;
}

template<typename T>
typename string_map<T>::mapped_type &string_map<T>::operator[](const key_type &key)
{
  Node *temp = mTrieRoot;
  for (auto it = key.begin(); it < key.end(); it++)
  {
    if (temp->mChildren[int(*it)] == nullptr)
    {
      temp->mSize++;
      Node *nuevo = new Node;
      temp->mChildren[int(*it)] = nuevo;
      temp->mChildren[int(*it)]->mParent = temp;
    }
    temp = temp->mChildren[int(*it)];
    temp->mKey = *it;
  }
  temp->mKey = key;
  if (temp->mValue == nullptr)
  {
    temp->mValue = new value_type(key, T());
    mKeysCount++;
  }
  return temp->mValue->second;
}

template<typename T>
typename string_map<T>::mapped_type &string_map<T>::at(const key_type &key)
{
  auto it = find(key);
  return it.node->mValue->second;
}

template<typename T>
const typename string_map<T>::mapped_type &string_map<T>::at(const key_type &key) const
{
  auto it = find(key);
  return it.node->mValue->second;
}

template<typename T>
void string_map<T>::clear()
{
  if (mTrieRoot != nullptr) delete mTrieRoot;
  mTrieRoot = new Node;
  mKeysCount = 0;
}

template<typename T>
typename string_map<T>::iterator string_map<T>::begin()
{
  if (empty()) return end();
  Node *walk = mTrieRoot;
  while (walk->mValue == nullptr)
  {
    int i = 0;
    while (i < 128 && walk->mChildren[i] == nullptr) i++;
    if (walk->mChildren[i] != nullptr) walk = walk->mChildren[i];
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
  Node *walk = mTrieRoot;
  while (walk->mValue == nullptr)
  {
    int i = 0;
    while (i < 128 && walk->mChildren[i] == nullptr) i++;
    if (walk->mChildren[i] != nullptr) walk = walk->mChildren[i];
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
  Node *temp = mTrieRoot;
  for (auto it = key.begin(); it < key.end(); it++)
  {
    if (temp->mChildren[int(*it)] == nullptr)
    {
      return end();
    }
    else
    {
      temp = temp->mChildren[int(*it)];
    }
  }
  return iterator(temp);
}

template<typename T>
typename string_map<T>::const_iterator string_map<T>::find(const key_type &key) const
{
  Node *temp = mTrieRoot;
  for (auto it = key.begin(); it < key.end(); it++)
  {
    if (temp->mChildren[int(*it)] == nullptr)
    {
      return end();
    }
    else
    {
      temp = temp->mChildren[int(*it)];
    }
  }
  return const_iterator(temp);
}

template<typename T>
std::pair<typename string_map<T>::iterator, bool> string_map<T>::insert(const string_map::value_type &value)
{
  std::pair<iterator, bool> finish;
  bool mod = false;
  Node *current = mTrieRoot;
  std::string tmpKey = value.first;

  for (auto it = tmpKey.begin(); it < tmpKey.end() - 1; it++)
  {
    if (current->mChildren[int(*it)] != nullptr)
    {
      current = current->mChildren[int(*it)];
    }
    else
    {
      current->mSize += 1;
      current->mChildren[int(*it)] = new Node;
      current->mChildren[int(*it)]->mParent = current;
      current = current->mChildren[int(*it)];
      current->mKey = *it;
    }
  }

  char c = tmpKey.back();

  if (current->mChildren[int(c)] == nullptr)
  {
    mKeysCount += 1;
    current->mSize += 1;
    current->mChildren[int(c)] = new Node;
  }
  else
  {
    mod = true;
  }

  current->mChildren[int(c)]->mValue = new value_type(tmpKey, value.second);
  current->mChildren[int(c)]->mKey = tmpKey;
  current->mChildren[int(c)]->mParent = current;

  iterator it = iterator(current->mChildren[int(c)]);
  finish.first = it;
  finish.second = mod;
  return finish;
}

template<typename T>
typename string_map<T>::size_type string_map<T>::erase(const string_map<T>::key_type &key)
{
  auto it = find(key);
  Node *walk = it.node;
  int eliminados = 0;

  delete walk->mValue;
  walk->mValue = nullptr;
  walk->mKey = key.back();
  mKeysCount--;
  eliminados++;

  if (walk->mSize > 0) return eliminados;

  Node *actualizar = walk->mParent;
  while (actualizar->mSize == 0 && actualizar->mValue == nullptr && actualizar->mParent != nullptr)
  {
    actualizar->mParent->mChildren[int(actualizar->mKey.back())] = nullptr;
    actualizar->mParent->mSize -= 1;
    actualizar = actualizar->mParent;
  }

  return eliminados;
}

template<typename T>
typename string_map<T>::iterator string_map<T>::erase(iterator pos)
{
  std::string clave = pos.node->mKey;
  if (pos.node->mSize == 0)
  {

    Node *nodoaborrar = pos.node;
    Node *padredelnodo = pos.node->mParent;

    delete nodoaborrar->mValue;
    nodoaborrar->mValue = nullptr;
    nodoaborrar->mKey = clave.back();

    pos++;

    delete nodoaborrar;
    nodoaborrar = nullptr;

    padredelnodo->mChildren[int(clave.back())] = nullptr;
    padredelnodo->mSize -= 1;
    clave = padredelnodo->mKey;
    Node *actualizar = padredelnodo;
    padredelnodo = nullptr;

    while (actualizar->mSize == 0 && actualizar->mValue == nullptr && actualizar->mParent != nullptr)
    {
      actualizar = actualizar->mParent;

      delete actualizar->mChildren[int(clave.back())];
      actualizar->mChildren[int(clave.back())] = nullptr;
      actualizar->mSize -= 1;

      clave = actualizar->mKey;
    }
    actualizar = nullptr;
    mKeysCount -= 1;
    return pos;
  }
  else
  {
    delete pos.node->mValue;
    pos.node->mValue = nullptr;
    pos.node->mKey = clave.back();
    mKeysCount -= 1;
    pos++;
    return pos;
  }
}

} // namespace Types
} // namespace Db
