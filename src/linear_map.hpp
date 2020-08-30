#pragma once

#include "linear_map.h"
#include <map>
#include <utility>

namespace Db {
namespace Types {

template<class K, class S>
linear_map<K, S>::linear_map() : mElems() {}

template<class K, class S>
linear_map<K, S>::linear_map(const linear_map<K, S> &other) : mElems(other.mElems) {}

template<class K, class S>
std::pair<typename linear_map<K, S>::iterator, bool> linear_map<K, S>::insert(const linear_map<K, S>::value_type &v)
{
  for (linear_map<K, S>::iterator it = mElems.begin(); it != mElems.end(); ++it)
  {
    if (it->first == v.first)
    {
      it->second = v.second;
      return make_pair(it, false);
    }
  }
  auto it = mElems.insert(mElems.end(), v);
  return make_pair(it, true);
}

template<class K, class S>
typename linear_map<K, S>::iterator linear_map<K, S>::fast_insert(const linear_map<K, S>::value_type &v)
{
  auto it = mElems.insert(mElems.end(), v);
  return it;
}

template<class K, class S>
linear_map<K, S> &linear_map<K, S>::operator=(const linear_map<K, S> &other)
{
  mElems.clear();
  for (const auto& v : other.mElems)
  {
    mElems.push_back(v);
  }
  return *this;
}

template<class K, class S>
typename linear_map<K, S>::size_type linear_map<K, S>::size() const
{
  return mElems.size();
}

template<class K, class S>
typename linear_map<K, S>::iterator linear_map<K, S>::find(const K &k)
{
  for (auto it = mElems.begin(); it != mElems.end(); ++it)
  {
    if (it->first == k)
    {
      return it;
    }
  }
  return mElems.end();
}

template<class K, class S>
typename linear_map<K, S>::const_iterator linear_map<K, S>::find(const K &k) const
{
  for (auto it = mElems.begin(); it != mElems.end(); it++)
  {
    if (it->first == k)
    {
      return it;
    }
  }
  return mElems.end();
}

template<class K, class S>
typename linear_map<K, S>::size_type linear_map<K, S>::count(const K &k) const
{
  return find(k) != mElems.end();
}

template<class K, class S>
bool linear_map<K, S>::empty() const
{
  return mElems.empty();
}

template<class K, class S>
bool linear_map<K, S>::operator==(const linear_map<K, S> &other) const
{
  for (const auto& v : other)
  {
    if (not count(v.first) or at(v.first) != v.second)
    {
      return false;
    }
  }
  return true;
}

template<class K, class S>
bool linear_map<K, S>::erase(const K &key)
{
  for (auto it = mElems.begin(); it != mElems.end(); it++)
  {
    if (it->first == key)
    {
      mElems.erase(it);
      return true;
    }
  }
  return false;
}

template<class K, class S>
const S &linear_map<K, S>::at(const K &key) const
{
  return find(key)->second;
}

template<class K, class S>
S &linear_map<K, S>::at(const K &key)
{
  return find(key)->second;
}

template<class K, class S>
typename linear_map<K, S>::iterator linear_map<K, S>::begin()
{
  return mElems.begin();
}

template<class K, class S>
typename linear_map<K, S>::iterator linear_map<K, S>::end()
{
  return mElems.end();
}

template<class K, class S>
typename linear_map<K, S>::const_iterator linear_map<K, S>::begin() const
{
  return mElems.cbegin();
}

template<class K, class S>
typename linear_map<K, S>::const_iterator linear_map<K, S>::end() const
{
  return mElems.cend();
}

template<class K, class S>
std::ostream &operator<<(std::ostream &os, const linear_map<K, S> &m)
{
  os << "{ ";
  int count = m.size();
  for (const auto& kv : m)
  {
    count--;
    os << kv.first << ": " << kv.second;
    if (count)
    {
      os << ", ";
    }
  }
  os << " }";
  return os;
}

} // namespace Types
} // namespace Db
