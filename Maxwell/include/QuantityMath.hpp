#ifndef QUANTITY_MATH_HPP
#define QUANTITY_MATH_HPP

#include <cmath>
#include <limits>
#include <numbers>

#include "QuantityCore.hpp"
#include "UnitCore.hpp"
#include "UnitTypes.hpp"

namespace std 
{
    template<Maxwell::Arithmetic Tp, Maxwell::UnitLike Unit>
    struct numeric_limits<Maxwell::Basic_Quantity<Tp, Unit>> : numeric_limits<Tp>
    {

    };
}

#define MAXWELL_TRIG_FUNCTION(func)\
    template<Arithmetic Tp>\
    double func(Basic_Quantity<Tp, RadianUnit> x) noexcept\
    {\
        return std::func(x.value());\
    }\
    template<Arithmetic Tp>\
    double func(Basic_Quantity<Tp, DegreeUnit> x) noexcept\
    {\
        return std::func(x.value()*Deg_To_Rad);\
    }

#define MAXWELL_RECIP_TRIG_FUNCTION(func, inv_func)\
    template<Arithmetic Tp>\
    double func(Basic_Quantity<Tp, RadianUnit> x) noexcept\
    {\
        return 1.0/std::inv_func(x.value());\
    }\
    template<Arithmetic Tp>\
    double func(Basic_Quantity<Tp, DegreeUnit> x) noexcept\
    {\
        return 1.0/std::inv_func(x.value()*Deg_To_Rad);\
    }

#define MAXWELL_INVERSE_TRIG_FUNCTION(func)\
    inline Basic_Quantity<double, RadianUnit> func(double x) noexcept\
    {\
        return Basic_Quantity<double, RadianUnit>{std::func(x)};\
    }\
    inline Basic_Quantity<double, DegreeUnit> func##d(double x) noexcept\
    {\
        return Basic_Quantity<double, DegreeUnit>{std::func(x)*Rad_To_Deg};\
    }

namespace Maxwell 
{
    inline constexpr double Deg_To_Rad = std::numbers::pi/180.0;
    inline constexpr double Rad_To_Deg = 180.0/std::numbers::pi;

    MAXWELL_TRIG_FUNCTION(sin)
    MAXWELL_TRIG_FUNCTION(cos)
    MAXWELL_TRIG_FUNCTION(tan)
    MAXWELL_RECIP_TRIG_FUNCTION(sec, cos)
    MAXWELL_RECIP_TRIG_FUNCTION(csc, sin)
    MAXWELL_RECIP_TRIG_FUNCTION(cot, tan)
    MAXWELL_INVERSE_TRIG_FUNCTION(asin)
    MAXWELL_INVERSE_TRIG_FUNCTION(acos)
    MAXWELL_INVERSE_TRIG_FUNCTION(atan)
    
    inline Basic_Quantity<double, RadianUnit> atan2(double x, double y) noexcept
    {
        return Basic_Quantity<double, RadianUnit>{std::atan2(x, y)};
    }
    
    inline Basic_Quantity<double, DegreeUnit> atan2d(double x, double y) noexcept
    {
        return Basic_Quantity<double, DegreeUnit>{std::atan2(x, y)*Rad_To_Deg};
    }

    template<typename Rep, typename Unit, int Exp>
    constexpr auto pow(Basic_Quantity<Rep, Unit> base) noexcept
    {
        if constexpr (Exp < 0)
        {
            return 1/pow<-Exp>(base);
        }
        else if constexpr(Exp == 0)
        {
            return Basic_Quantity<Rep, Unit>(1);
        }
        else if constexpr (Exp == 2)
        {
            return pow<Exp/2>(base*base);
        }
        else 
        {
            constexpr int N = (Exp - 1)/2;
            return base * pow<N>(base*base);
        }
    }
}

#endif