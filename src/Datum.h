#pragma once
#include <cstring>
#include <exception>
#include <iostream>
#include <memory>
#include <string>

class Datum
{
public:
  Datum() = delete;

  template<typename T>
  Datum(T t)
  {
    mSelf = std::make_unique<entity_impl<T>>(t);
  }

  Datum(const char* str)
  {
    mSelf = std::make_unique<entity_impl<std::string>>(str);
  }

  Datum(const Datum &datum)
  {
    mSelf = datum.mSelf->copy();
  }

  Datum &operator=(const Datum &rhs)
  {
    mSelf = rhs.mSelf->copy();
    return *this;
  }

  bool isInteger() const
  {
    return dynamic_cast<entity_impl<int> *>(mSelf.get()) != nullptr;
  }

  bool isString() const
  {
    return dynamic_cast<entity_impl<std::string> *>(mSelf.get()) != nullptr;
  }

  template<typename T>
  T value() const
  {
    return (dynamic_cast<entity_impl<T> *>(mSelf.get()))->mData;
  }

  bool operator==(const Datum &rhs) const
  {
    return mSelf->operator==(rhs.mSelf);
  }

  bool operator!=(const Datum &rhs) const
  {
    return !(*this == rhs);
  }

  bool operator<(const Datum &rhs) const
  {
    return mSelf->operator<(rhs.mSelf);
  }

  friend std::ostream &operator<<(std::ostream &os, const Datum &datum)
  {
    if (datum.isInteger())
    {
      os << std::to_string(datum.value<int>());
    }
    else
    {
      os << datum.value<std::string>();
    }
    return os;
  }

private:

  struct entity
  {
    virtual ~entity() = default;
    virtual std::unique_ptr<entity> copy() = 0;
    virtual bool operator<(const std::unique_ptr<entity> &rhs) const = 0;
    virtual bool operator==(const std::unique_ptr<entity> &rhs) const = 0;
  };

  template<typename T>
  struct entity_impl final : entity
  {
    entity_impl(T t) : mData(std::move(t)) {}

    ~entity_impl() = default;

    std::unique_ptr<entity> copy() override
    {
      std::unique_ptr<entity> rt = std::make_unique<entity_impl<T>>(mData);
      return rt;
    }

    bool operator<(const std::unique_ptr<entity> &rhs) const override
    {
      try
      {
        auto rhs_casted = dynamic_cast<entity_impl<T>*>(rhs.get());
        return mData < rhs_casted->mData;
      }
      catch (const std::exception& e)
      {
        throw e.what();
      }
    }

    bool operator==(const std::unique_ptr<entity> &rhs) const override
    {
      auto rhs_casted = dynamic_cast<entity_impl<T>*>(rhs.get());
      if (rhs_casted == nullptr) return false;
      return mData == rhs_casted->mData;
    }

    T mData;
  };

  std::unique_ptr<entity> mSelf;
};
