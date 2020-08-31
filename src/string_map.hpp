#pragma once

#include <utility>

namespace Db {
namespace Types {

template<typename T>
string_map<T>::string_map() : mTrieRoot(new Node)
{
}

template<typename T>
string_map<T>::~string_map()
{
  delete mTrieRoot;
  mTrieRoot = nullptr;
}

template<typename T>
string_map<T>::string_map(const string_map &p) : mTrieRoot(new Node), mKeysCount(p.mKeysCount)
{
  mTrieRoot->mKey = p.mTrieRoot->mKey;
  copyChildren(mTrieRoot, p.mTrieRoot);
}

template<typename T>
void string_map<T>::copyChildren(Node *head, Node *other)
{
  if (!other) return;

  for (int i = 0; i < 128; i++)
  {
    if (other->mChildren[i])
    {
      head->mSize++;
      head->mChildren[i] = new Node;
      head->mChildren[i]->mParent = head;
      copyChildren(head->mChildren[i], other->mChildren[i]);
    }
  }

  head->mKey = other->mKey;

  if (other->mValue)
  {
    key_type key = other->mValue->first;
    mapped_type value = other->mValue->second;
    head->mValue = new value_type(key, value);
  }
}

template<typename T>
string_map<T> &string_map<T>::operator=(const string_map &p)
{
  clear();

  if (!p.mTrieRoot)
  {
    mTrieRoot = nullptr;
  }
  else
  {
    if (p.mTrieRoot->mValue)
    {
      mTrieRoot->mValue = new value_type(p.mTrieRoot->mValue->first, p.mTrieRoot->mValue->second);
    }
    copyChildren(mTrieRoot, p.mTrieRoot);
  }

  mTrieRoot->mKey = p.mTrieRoot->mKey;
  mKeysCount = p.mKeysCount;
  return *this;
}

template<typename T>
bool string_map<T>::operator==(const string_map &other) const
{
  auto it1 = begin();
  auto it2 = other.begin();
  auto itend1 = end();
  auto itend2 = other.end();
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
typename string_map<T>::Node *string_map<T>::iterator::nextDescendant(Node *n)
{
  Node *tmp = n;
  findNextDescendant(tmp);
  return tmp;
}

template<typename T>
void string_map<T>::iterator::findNextDescendant(Node *&n)
{
  int i = 0;
  while (i < 128 && !n->mChildren[i]) i++;

  if (i == 128)
  {
    n = nullptr;
  }
  else if (n->mChildren[i]->mValue)
  {
    n = n->mChildren[i];
  }
  else
  {
    n = n->mChildren[i];
    findNextDescendant(n);
  }
}

template<typename T>
typename string_map<T>::Node *string_map<T>::iterator::nextAncestor(Node *n)
{
  Node *tmp = n;
  findNextAncestor(tmp);
  return tmp;
}

template<typename T>
void string_map<T>::iterator::findNextAncestor(Node *&n)
{
  if (!n->mParent)
  {
    n = nullptr;
    return;
  }

  Node *next = n->mParent;
  char key = n->mKey.back();
  int i = key + 1;
  while (i < 128 && !next->mChildren[i]) i++;

  if (i != 128)
  {
    n = next->mChildren[i];
    if (!n->mValue)
    {
      findNextDescendant(n);
    }
  }
  else
  {
    n = next;
    findNextAncestor(n);
  }
}

template<typename T>
size_t string_map<T>::count(const key_type &key) const
{
  Node *walk = mTrieRoot;

  for (const char& c : key)
  {
    if (!walk->mChildren[int(c)]) return 0;
    walk = walk->mChildren[int(c)];
  }

  if (walk->mKey == key && walk->mValue) return 1;
  return 0;
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
    if (!temp->mChildren[int(*it)])
    {
      temp->mSize++;
      Node *newNode = new Node;
      temp->mChildren[int(*it)] = newNode;
      temp->mChildren[int(*it)]->mParent = temp;
    }
    temp = temp->mChildren[int(*it)];
    temp->mKey = *it;
  }
  temp->mKey = key;
  if (!temp->mValue)
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
  return it.mNode->mValue->second;
}

template<typename T>
const typename string_map<T>::mapped_type &string_map<T>::at(const key_type &key) const
{
  auto it = find(key);
  return it->second;
}

template<typename T>
void string_map<T>::clear()
{
  if (mTrieRoot) delete mTrieRoot;
  mTrieRoot = new Node;
  mKeysCount = 0;
}

template<typename T>
typename string_map<T>::iterator string_map<T>::begin()
{
  if (empty()) return end();
  Node *walk = mTrieRoot;
  while (!walk->mValue)
  {
    int i = 0;
    while (i < 128 && !walk->mChildren[i]) i++;
    if (walk->mChildren[i]) walk = walk->mChildren[i];
  }
  auto it = iterator(walk);
  return it;
}

template<typename T>
typename string_map<T>::iterator string_map<T>::end()
{
  return iterator();
}

template<typename T>
typename string_map<T>::const_iterator string_map<T>::begin() const
{
  if (empty()) return end();
  Node *walk = mTrieRoot;
  while (!walk->mValue)
  {
    int i = 0;
    while (i < 128 && !walk->mChildren[i]) i++;
    if (walk->mChildren[i]) walk = walk->mChildren[i];
  }
  auto it = const_iterator(walk);
  return it;
}

template<typename T>
typename string_map<T>::const_iterator string_map<T>::end() const
{
  return const_iterator();
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
    if (!temp->mChildren[int(*it)]) return end();
    temp = temp->mChildren[int(*it)];
  }
  return iterator(temp);
}

template<typename T>
typename string_map<T>::const_iterator string_map<T>::find(const key_type &key) const
{
  Node *temp = mTrieRoot;
  for (auto it = key.begin(); it < key.end(); it++)
  {
    if (!temp->mChildren[int(*it)]) return end();
    temp = temp->mChildren[int(*it)];
  }
  return const_iterator(temp);
}

template<typename T>
std::pair<typename string_map<T>::iterator, bool> string_map<T>::insert(const string_map::value_type &value)
{
  Node *current = mTrieRoot;
  const std::string tmpKey = value.first;

  for (auto it = tmpKey.begin(); it < tmpKey.end() - 1; ++it)
  {
    if (current->mChildren[int(*it)])
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

  const char c = tmpKey.back();
  bool modified = false;
  if (!current->mChildren[int(c)])
  {
    mKeysCount += 1;
    current->mSize += 1;
    current->mChildren[int(c)] = new Node;
  }
  else
  {
    modified = true;
  }

  current->mChildren[int(c)]->mValue = new value_type(tmpKey, value.second);
  current->mChildren[int(c)]->mKey = tmpKey;
  current->mChildren[int(c)]->mParent = current;

  auto it = iterator(current->mChildren[int(c)]);
  return { it, modified };
}

template<typename T>
typename string_map<T>::size_type string_map<T>::erase(const string_map<T>::key_type &key)
{
  auto it = find(key);
  Node *walk = it.mNode;
  size_type erasedCount = 0;

  delete walk->mValue;
  walk->mValue = nullptr;
  walk->mKey = key.back();
  mKeysCount--;
  erasedCount++;

  if (walk->mSize > 0) return erasedCount;

  Node *update = walk->mParent;
  while (update->mSize == 0 && !update->mValue && update->mParent)
  {
    update->mParent->mChildren[int(update->mKey.back())] = nullptr;
    update->mParent->mSize -= 1;
    update = update->mParent;
  }

  return erasedCount;
}

template<typename T>
typename string_map<T>::iterator string_map<T>::erase(iterator pos)
{
  std::string key = pos.mNode->mKey;
  if (pos.mNode->mSize == 0)
  {
    Node *nodeToErase = pos.mNode;
    Node *nodeParent = pos.mNode->mParent;

    delete nodeToErase->mValue;
    nodeToErase->mValue = nullptr;
    nodeToErase->mKey = key.back();

    pos++;

    delete nodeToErase;
    nodeToErase = nullptr;

    nodeParent->mChildren[int(key.back())] = nullptr;
    nodeParent->mSize -= 1;
    key = nodeParent->mKey;
    Node *nodeToUpdate = nodeParent;
    nodeParent = nullptr;

    while (nodeToUpdate->mSize == 0 && !nodeToUpdate->mValue && nodeToUpdate->mParent)
    {
      nodeToUpdate = nodeToUpdate->mParent;

      delete nodeToUpdate->mChildren[int(key.back())];
      nodeToUpdate->mChildren[int(key.back())] = nullptr;
      nodeToUpdate->mSize -= 1;

      key = nodeToUpdate->mKey;
    }
    nodeToUpdate = nullptr;
    mKeysCount -= 1;
    return pos;
  }
  else
  {
    delete pos.mNode->mValue;
    pos.mNode->mValue = nullptr;
    pos.mNode->mKey = key.back();
    mKeysCount -= 1;
    pos++;
    return pos;
  }
}

} // namespace Types
} // namespace Db
