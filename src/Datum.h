#pragma once
#include <cstring>
#include <exception>
#include <iostream>
#include <memory>
#include <string>

class Datum
{
public:
  Datum() {}

  template<typename T>
  Datum(T t)
  {
    self_ = std::make_unique<entity_impl<T>>(t);
  }

  Datum(const char* str)
  {
    self_ = std::make_unique<entity_impl<std::string>>(str);
  }

  Datum(const Datum &datum)
  {
    self_ = datum.self_->copy();
  }

  Datum &operator=(const Datum &rhs)
  {
    self_ = rhs.self_->copy();
    return *this;
  }

  bool isInteger() const
  {
    return dynamic_cast<entity_impl<int> *>(self_.get()) != nullptr;
  }

  bool isString() const
  {
    return dynamic_cast<entity_impl<std::string> *>(self_.get()) != nullptr;
  }

  std::string stringValue() const
  {
    std::string value;

    try
    {
      if (isString())
        value = (dynamic_cast<entity_impl<std::string> *>(self_.get()))->data_;

    }
    catch (std::exception &e)
    {
      throw e;
    }

    return value;
  }

  int integerValue() const
  {
    int value = 0;

    try
    {
      if (isInteger())
        value = (dynamic_cast<entity_impl<int> *>(self_.get()))->data_;

    }
    catch (std::exception &e)
    {
      throw e;
    }

    return value;
  }

private:

  struct entity
  {
    virtual ~entity() = default;
    virtual std::unique_ptr<entity> copy() = 0;
    virtual bool operator<(const std::unique_ptr<entity> &rhs) const = 0;
  };

  template<typename T>
  struct entity_impl final : entity
  {
    typedef T value_type;

    entity_impl(T t) : data_(std::move(t)) {}

    ~entity_impl() = default;

    std::unique_ptr<entity> copy() override
    {
      std::unique_ptr<entity> rt = std::make_unique<entity_impl<T>>(data_);
      return rt;
    }

    bool operator<(const std::unique_ptr<entity> &rhs) const override
    {
      return data_ < dynamic_cast<entity_impl<value_type>*>(rhs.get())->data_;
    }

    T data_;
  };

  std::unique_ptr<entity> self_;

public:

  bool operator==(const Datum &rhs) const
  {
    if (isInteger() == rhs.isInteger())
    {
      if (isInteger())
      {
        return integerValue() == rhs.integerValue();
      }
      else
      {
        return stringValue() == rhs.stringValue();
      }
    }
    return false;
  }

  bool operator!=(const Datum &rhs) const
  {
    return !(*this == rhs);
  }

  bool operator<(const Datum &rhs) const
  {
    return self_.get() < rhs.self_.get();
  }

  friend std::ostream &operator<<(std::ostream &os, const Datum &datum)
  {
    if (datum.isInteger())
    {
      os << std::to_string(datum.integerValue());
    }
    else
    {
      os << datum.stringValue();
    }
    return os;
  }
};
