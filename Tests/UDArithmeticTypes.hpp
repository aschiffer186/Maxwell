#ifndef UD_ARITHMETIC_TYPES
#define UD_ARITHMETIC_TYPES

#include <algorithm>

class Type1
{
  public:
    constexpr Type1() noexcept = default;

    constexpr Type1(double d) noexcept
        : d_{d}
    {
    }

    constexpr auto val() noexcept -> double { return d_; }

    constexpr auto operator+=(Type1 t) noexcept -> Type1&
    {
        d_ += t.d_;
        return *this;
    }

    constexpr auto operator-=(Type1 t) noexcept -> Type1&
    {
        d_ -= t.d_;
        return *this;
    }

    constexpr auto operator*=(Type1 t) noexcept -> Type1&
    {
        d_ *= t.d_;
        return *this;
    }

    constexpr auto operator/=(Type1 t) noexcept -> Type1&
    {
        d_ /= t.d_;
        return *this;
    }

    constexpr auto swap(Type1& t) noexcept -> void
    {
        using std::swap;
        swap(t.d_, d_);
    }

    friend constexpr auto operator<=>(Type1, Type1) noexcept = default;

  private:
    double d_{};
};

constexpr auto operator+(Type1 t1, Type1 t2) noexcept -> Type1
{
    return t1 += t2;
}
constexpr auto operator-(Type1 t1, Type1 t2) noexcept -> Type1
{
    return t1 -= t2;
}
constexpr auto operator*(Type1 t1, Type1 t2) noexcept -> Type1
{
    return t1 *= t2;
}
constexpr auto operator/(Type1 t1, Type1 t2) noexcept -> Type1
{
    return t1 /= t2;
}
constexpr auto operator*(double d, Type1 t) noexcept -> Type1
{
    return Type1{d * t.val()};
}

class Type2
{
  public:
    constexpr Type2() noexcept(false) = default;

    constexpr Type2(double d)
        : d_{d}
    {
    }

    constexpr auto val() -> double { return d_; }

    constexpr auto operator+=(Type2 t) -> Type2&
    {
        d_ += t.d_;
        return *this;
    }

    constexpr auto operator-=(Type2 t) -> Type2&
    {
        d_ -= t.d_;
        return *this;
    }

    constexpr auto operator*=(Type2 t) -> Type2&
    {
        d_ *= t.d_;
        return *this;
    }

    constexpr auto operator/=(Type2 t) -> Type2&
    {
        d_ /= t.d_;
        return *this;
    }

    constexpr auto swap(Type2& t) noexcept(false) -> void
    {
        using std::swap;
        swap(t.d_, d_);
    }

    friend constexpr auto operator<=>(Type2, Type2) = default;

  private:
    double d_{};
};

constexpr auto operator+(Type2 t1, Type2 t2) noexcept(false) -> Type2
{
    return t1 += t2;
}
constexpr auto operator-(Type2 t1, Type2 t2) noexcept(false) -> Type2
{
    return t1 -= t2;
}
constexpr auto operator*(Type2 t1, Type2 t2) noexcept(false) -> Type2
{
    return t1 *= t2;
}
constexpr auto operator/(Type2 t1, Type2 t2) noexcept(false) -> Type2
{
    return t1 /= t2;
}
constexpr auto operator*(double d, Type2 t) noexcept(false) -> Type2
{
    return Type2{d * t.val()};
}

#endif