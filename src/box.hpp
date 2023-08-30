#pragma once

#include <memory>

template <typename T> class Box
{
  public:
    Box(T &&value) : contents(new T(std::move(value))) {}
    Box(const T &value) : contents(new T(value)) {}

    Box(const Box &other) : Box(*other.contents) {}
    auto operator=(const Box &other) -> Box &
    {
        *contents = *other.contents;
        return *this;
    }

    ~Box() = default;

    auto operator*() -> T & { return *contents; }
    auto operator*() const -> const T & { return *contents; }

    auto operator->() -> T * { return contents.get(); }
    auto operator->() const -> const T * { return contents.get(); }

  private:
    std::unique_ptr<T> contents;
};
