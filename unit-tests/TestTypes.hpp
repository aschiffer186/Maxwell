#ifndef TEST_TYPES_HPP
#define TEST_TYPES_HPP

#include <initializer_list>
#include <iostream>
#include <numeric>

template <bool Noexcept>
struct Noisy
{
    static int numDefaultCtorCalls;
    static int numCopyCtorCalls;
    static int numMoveCtorCalls;
    static int numCopyAssignCalls;
    static int numMoveAssignCalls;

    Noisy() noexcept(Noexcept);
    Noisy(const Noisy&) noexcept(Noexcept);
    Noisy(Noisy&&) noexcept(Noexcept);
    Noisy& operator=(const Noisy&) noexcept(Noexcept);
    Noisy& operator=(Noisy&&) noexcept(Noexcept);
};

template <bool Noexcept>
int Noisy<Noexcept>::numDefaultCtorCalls = 0;
template <bool Noexcept>
int Noisy<Noexcept>::numCopyCtorCalls = 0;
template <bool Noexcept>
int Noisy<Noexcept>::numMoveCtorCalls = 0;
template <bool Noexcept>
int Noisy<Noexcept>::numCopyAssignCalls = 0;
template <bool Noexcept>
int Noisy<Noexcept>::numMoveAssignCalls = 0;

template <bool Noexcept>
Noisy<Noexcept>::Noisy() noexcept(Noexcept)
{
    ++numDefaultCtorCalls;
}

template <bool Noexcept>
Noisy<Noexcept>::Noisy(const Noisy&) noexcept(Noexcept)
{
    ++numCopyCtorCalls;
}

template <bool Noexcept>
Noisy<Noexcept>::Noisy(Noisy&&) noexcept(Noexcept)
{
    ++numMoveCtorCalls;
}

template <bool Noexcept>
Noisy<Noexcept>& Noisy<Noexcept>::operator=(const Noisy&) noexcept(Noexcept)
{
    ++numCopyAssignCalls;
}

template <bool Noexcept>
Noisy<Noexcept>& Noisy<Noexcept>::operator=(Noisy&&) noexcept(Noexcept)
{
    ++numMoveAssignCalls;
}

struct in_place
{
    double value;

    in_place(double d1, double d2) : value(d1 + d2)
    {
    }

    in_place(std::initializer_list<double> il, double d) : value(std::accumulate(il.begin(), il.end(), 0.0) + d)
    {
    }
};
#endif