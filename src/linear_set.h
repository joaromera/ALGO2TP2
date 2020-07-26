#pragma once
#include <utility>
#include <ostream>
#include "linear_map.h"

namespace Db {
namespace Types {

template<typename T>
class linear_set
{
public:
  class iterator;

  class const_iterator
  {
  public:
    using value_type = T;
    using iterator_category = std::forward_iterator_tag;
    using reference = value_type &;
    using pointer = value_type *;
    using difference_type = std::ptrdiff_t;

    const_iterator(const const_iterator &);

    const_iterator(const iterator &);

    const_iterator &operator++();

    const value_type &operator*() const;

    const value_type *operator->() const;

    bool operator==(const const_iterator &other) const;

    bool operator!=(const const_iterator &other) const;

  private:
    friend class linear_set<T>;

    const_iterator(const typename linear_map<T, bool>::const_iterator &);

    typename linear_map<T, bool>::const_iterator it;
  };

  class iterator
  {
  public:
    using value_type = const T;
    using iterator_category = std::forward_iterator_tag;
    using reference = value_type &;
    using pointer = value_type *;
    using difference_type = std::ptrdiff_t;

    iterator(const iterator &);

    iterator &operator++();

    const value_type &operator*() const;

    const value_type *operator->() const;

    bool operator==(const iterator &other) const;

    bool operator!=(const iterator &other) const;

  private:
    friend class linear_set;

    iterator(const typename linear_map<T, bool>::iterator &);

    typename linear_map<T, bool>::iterator it;
  };

  using value_type = T;
  using size_type = size_t;

  linear_set();

  linear_set(std::initializer_list<value_type> init);


  template<class InputIt>
  linear_set(InputIt first, InputIt last);

  linear_set(const linear_set<T> &other);

  std::pair<iterator, bool> insert(const T &x);

  iterator fast_insert(const value_type &v);

  size_type count(const T &x) const;

  bool empty() const;

  size_type size() const;


  size_type erase(const T &x);

  iterator find(const T &x);

  linear_set<T> &operator=(const linear_set<T> &other);

  bool operator==(const linear_set<T> &other) const;

  bool operator!=(const linear_set<T> &other) const;

  iterator begin();

  iterator end();

  const_iterator begin() const;

  const_iterator end() const;

private:
  linear_map<T, bool> _elems;
};

template<class T>
std::ostream &operator<<(std::ostream &, const linear_set<T> &);

} // namespace Types
} // namespace Db

#include "linear_set.hpp"
#include "linear_set_iterators.h"