#pragma once

#include <cassert>
#include <string>
#include <vector>

namespace Db {
namespace Types {

template<typename T>
class string_map
{
public:
  typedef std::string key_type;
  typedef T mapped_type;
  typedef std::pair<const key_type, mapped_type> value_type;
  typedef size_t size_type;

private:
  struct Node;

public:
  class iterator
  {
  public:
    typedef std::string key_type;
    typedef T mapped_type;
    typedef std::pair<const key_type, mapped_type> value_type;

    iterator() = default;

    iterator(const iterator &n)
    {
      node = n.node;
    }

    iterator &operator=(const iterator &n)
    {
      node = n.node;
      return *this;
    }

    ~iterator()
    {
      node = nullptr;
    }

    bool operator==(const iterator &n) const
    {
      if (node == nullptr || n.node == nullptr)
      {
        return node == n.node;
      }
      if (node->mValue == nullptr || n.node->mValue == nullptr)
      {
        return node->mValue == n.node->mValue;
      }
      return *(node->mValue) == *(n.node->mValue);
    }

    bool operator!=(const iterator &n) const
    {
      return !(*this == n);
    }

    iterator &operator++()
    {
      Node *nu = proximoAbajo(node);
      if (nu != nullptr)
      {
        node = nu;
        return *this;
      }
      else
      {
        nu = proximoArriba(node);
        node = nu;
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
      return *node->mValue;
    }

    value_type *operator->() const
    {
      return node->mValue;
    }

    friend class string_map;

  private:
    iterator(Node *n) : node(n){};
    Node* node = nullptr;

    typename string_map<T>::Node *proximoAbajo(Node *n);
    void buscarNodoAbajo(Node *&n);
    typename string_map<T>::Node *proximoArriba(Node *n);
    void buscarNodoArriba(Node *&n);
  };

  class const_iterator
  {
  public:
    typedef std::string key_type;
    typedef T mapped_type;
    typedef std::pair<const key_type, mapped_type> value_type;

    const_iterator() = default;

    const_iterator(const const_iterator &n)
    {
      node = n.node;
    }

    ~const_iterator()
    {
      node = nullptr;
    }

    const_iterator &operator=(const const_iterator &n)
    {
      node = n.node;
      return *this;
    }

    bool operator==(const const_iterator &n) const
    {
      if (node == nullptr || n.node == nullptr)
      {
        return node == n.node;
      }
      if (node->mValue == nullptr || n.node->mValue == nullptr)
      {
        return node->mValue == n.node->mValue;
      }
      return *(node->mValue) == *(n.node->mValue);
    }

    bool operator!=(const const_iterator &n) const
    {
      return !(*this == n);
    }

    const_iterator &operator++()
    {
      Node *nu = proximoAbajo(node);
      if (nu != nullptr)
      {
        node = nu;
        return *this;
      }
      else
      {
        nu = proximoArriba(node);
        node = nu;
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
      return *node->mValue;
    }

    const value_type *operator->() const
    {
      return node->mValue;
    }

    friend class string_map;

  private:
    const_iterator(Node *n) : node(n) {}
    Node *node = nullptr;

    typename string_map<T>::Node *proximoAbajo(Node *n);
    void buscarNodoAbajo(Node *&n);
    typename string_map<T>::Node *proximoArriba(Node *n);
    void buscarNodoArriba(Node *&n);
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
  struct Node
  {
    std::string mKey;
    Node *mParent;
    std::vector<Node *> mChildren;
    int mSize;
    value_type *mValue;

    Node() : mKey(), mParent(nullptr), mChildren(128, nullptr), mSize(), mValue(nullptr) {}

    Node(const Node &n) : mKey(n.mKey), mParent(n.mParent), mChildren(n.mChildren), mSize(n.mSize), mValue(n.mValue) {}

    bool operator==(const Node &rhs) const
    {
      return mKey == rhs.mKey && mParent == rhs.mParent && mChildren == rhs.mChildren && mSize == rhs.mSize && mValue == rhs.mValue;
    }

    bool operator!=(const Node &rhs) const
    {
      return !(rhs == *this);
    }
  };

  Node *mTrieRoot = nullptr;
  size_t mKeysCount = 0;

  void eraseTrie(Node *&n);
  void copyChildren(Node *head, Node *other);
};

} // namespace Types
} // namespace Db


#include "string_map.hpp"
