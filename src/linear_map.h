#pragma once
#include <list>
#include <iostream>
#include <utility>

template<class K, class S>
class linear_map
{
public:
  using key_type = K;
  using mapped_type = S;
  using value_type = std::pair<const K, S>;
  using size_type = size_t;

  using iterator = typename std::list<std::pair<const K, S>>::iterator;
  using const_iterator = typename std::list<std::pair<const K, S>>::const_iterator;

  linear_map();

  linear_map(const linear_map<K, S> &other);

  std::pair<iterator, bool> insert(const value_type &v);

  iterator fast_insert(const value_type &v);

  size_type size() const;

  S &at(const K &key);

  bool empty() const
  {
    return _elems.empty();
  }

  iterator find(const K &k);

  const_iterator find(const K &k) const;

  iterator begin();

  iterator end();

  const_iterator begin() const;

  const_iterator end() const;

  size_t count(const K &k) const;

  const S &at(const K &key) const;

  size_type erase(const K &key);

  linear_map &operator=(const linear_map &other);

  bool operator==(const linear_map &other) const;

private:
  std::list<std::pair<const K, S>> _elems;
};

template<class K, class S>
std::ostream &operator<<(std::ostream &os, const linear_map<K, S> &m);

#include "linear_map.hpp"