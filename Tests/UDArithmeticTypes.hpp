#ifndef UD_ARITHMETIC_TYPES
#define UD_ARITHMETIC_TYPES

#include <algorithm>

class Type1
{
  public:
    Type1() noexcept = default;

    Type1(double d) noexcept
        : d_{d}
    {
    }

    Type1(const Type1& d) noexcept
        : d_{d.d_}
    {
        ++copyCtorCalls;
    }

    Type1(Type1&& d) noexcept
        : d_{d.d_}
    {
        ++moveCtorCalls;
    }

    Type1& operator=(const Type1& d) noexcept
    {
        ++copyAssignCalls;
        d_ = d.d_;
        return *this;
    }

    Type1& operator=(Type1&& d) noexcept
    {
        ++moveAssignCalls;
        d_ = d.d_;
        return *this;
    }

    auto val() const noexcept -> double { return d_; }

    auto operator+=(Type1 t) noexcept -> Type1&
    {
        d_ += t.d_;
        return *this;
    }

    auto operator-=(Type1 t) noexcept -> Type1&
    {
        d_ -= t.d_;
        return *this;
    }

    auto operator*=(Type1 t) noexcept -> Type1&
    {
        d_ *= t.d_;
        return *this;
    }

    auto operator/=(Type1 t) noexcept -> Type1&
    {
        d_ /= t.d_;
        return *this;
    }

    auto swap(Type1& t) noexcept -> void
    {
        using std::swap;
        swap(t.d_, d_);
    }

    friend auto operator<=>(Type1, Type1) noexcept = default;

    static int copyCtorCalls;
    static int moveCtorCalls;
    static int copyAssignCalls;
    static int moveAssignCalls;

  private:
    double d_{};
};

inline int Type1::copyCtorCalls{0};
inline int Type1::moveCtorCalls{0};
inline int Type1::copyAssignCalls{0};
inline int Type1::moveAssignCalls{0};

inline auto operator+(Type1 t1, Type1 t2) noexcept -> Type1 { return t1 += t2; }
inline auto operator-(Type1 t1, Type1 t2) noexcept -> Type1 { return t1 -= t2; }
inline auto operator*(Type1 t1, Type1 t2) noexcept -> Type1 { return t1 *= t2; }
inline auto operator/(Type1 t1, Type1 t2) noexcept -> Type1 { return t1 /= t2; }
inline auto operator*(double d, Type1 t) noexcept -> Type1
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

    constexpr auto val() const -> double { return d_; }

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