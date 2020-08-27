#pragma once
#include <iostream>

namespace Db {
namespace Types {

// const_iterator methods

template<typename T>
linear_set<T>::const_iterator::const_iterator(
  const typename linear_set<T>::const_iterator &other) : it(other.it) {}

template<typename T>
linear_set<T>::const_iterator::const_iterator(
  const typename linear_set<T>::iterator &other) : it(other.it) {}

template<typename T>
typename linear_set<T>::const_iterator &linear_set<T>::const_iterator::operator++()
{
  it++;
  return *this;
}

template<typename T>
const typename linear_set<T>::const_iterator::value_type &linear_set<T>::const_iterator::operator*() const
{
  return it->first;
}

template<typename T>
const typename linear_set<T>::const_iterator::value_type *linear_set<T>::const_iterator::operator->() const
{
  return &it->first;
}

template<typename T>
bool linear_set<T>::const_iterator::operator==(const linear_set<T>::const_iterator &other) const
{
  return it == other.it;
}

template<typename T>
bool linear_set<T>::const_iterator::operator!=(const linear_set<T>::const_iterator &other) const
{
  return !(*this == other);
}

template<typename T>
linear_set<T>::const_iterator::const_iterator(
  const typename linear_map<T, bool>::const_iterator &_it)
  : it(_it) {}


// iterator methods

template<typename T>
linear_set<T>::iterator::iterator(const typename linear_set<T>::iterator &other)
  : it(other.it) {}

template<typename T>
linear_set<T>::iterator::iterator(const typename linear_map<T, bool>::iterator &_it)
  : it(_it) {}

template<typename T>
typename linear_set<T>::iterator &linear_set<T>::iterator::operator++()
{
  it++;
  return *this;
}

template<typename T>
const typename linear_set<T>::iterator::value_type &linear_set<T>::iterator::operator*() const
{
  return it->first;
}

template<typename T>
const typename linear_set<T>::iterator::value_type *linear_set<T>::iterator::operator->() const
{
  return &it->first;
}

template<typename T>
bool linear_set<T>::iterator::operator==(const linear_set<T>::iterator &other) const
{
  return it == other.it;
}

template<typename T>
bool linear_set<T>::iterator::operator!=(const linear_set<T>::iterator &other) const
{
  return !(*this == other);
}

} // namespace Types
} // namespace Db
