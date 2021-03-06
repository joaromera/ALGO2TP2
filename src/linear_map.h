#pragma once
#include <list>
#include <iostream>
#include <utility>

namespace Db {
namespace Types {

template<class K, class S>
class linear_map
{
public:
  using key_type = K;
  using mapped_type = S;
  using value_type = std::pair<const K, S>;
  using size_type = typename std::list<std::pair<const K, S>>::size_type;
  using iterator = typename std::list<std::pair<const K, S>>::iterator;
  using const_iterator = typename std::list<std::pair<const K, S>>::const_iterator;

  linear_map();
  linear_map(const linear_map<K, S> &other);
  linear_map &operator=(const linear_map &other);

  std::pair<iterator, bool> insert(const value_type &v);
  iterator fast_insert(const value_type &v);
  size_type size() const;
  S &at(const K &key);
  bool empty() const;
  iterator find(const K &k);
  const_iterator find(const K &k) const;
  iterator begin();
  iterator end();
  const_iterator begin() const;
  const_iterator end() const;
  size_type count(const K &k) const;
  const S &at(const K &key) const;
  bool erase(const K &key);
  bool operator==(const linear_map &other) const;

private:
  std::list<std::pair<const K, S>> mElems;
};

template<class K, class S>
std::ostream &operator<<(std::ostream &os, const linear_map<K, S> &m);

} // namespace Types
} // namespace Db

#include "linear_map.hpp"
