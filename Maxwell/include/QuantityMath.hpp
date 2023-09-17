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
    struct numeric_limits<Maxwell::Quantity<Tp, Unit>> : numeric_limits<Tp>
    {

    };
}

#define MAXWELL_TRIG_FUNCTION(func)\
    template<Arithmetic Tp>\
    double func(Quantity<Tp, Radian> x) noexcept\
    {\
        return std::func(x.value());\
    }\
    template<Arithmetic Tp>\
    double func(Quantity<Tp, Degree> x) noexcept\
    {\
        return std::func(x.value()*Deg_To_Rad);\
    }

#define MAXWELL_RECIP_TRIG_FUNCTION(func, inv_func)\
    template<Arithmetic Tp>\
    double func(Quantity<Tp, Radian> x) noexcept\
    {\
        return 1.0/std::inv_func(x.value());\
    }\
    template<Arithmetic Tp>\
    double func(Quantity<Tp, Degree> x) noexcept\
    {\
        return 1.0/std::inv_func(x.value()*Deg_To_Rad);\
    }

#define MAXWELL_INVERSE_TRIG_FUNCTION(func)\
    inline Quantity<double, Radian> func(double x) noexcept\
    {\
        return Quantity<double, Radian>{std::func(x)};\
    }\
    inline Quantity<double, Degree> func##d(double x) noexcept\
    {\
        return Quantity<double, Degree>{std::func(x)*Rad_To_Deg};\
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
    
    inline Quantity<double, Radian> atan2(double x, double y) noexcept
    {
        return Quantity<double, Radian>{std::atan2(x, y)};
    }
    
    inline Quantity<double, Degree> atan2d(double x, double y) noexcept
    {
        return Quantity<double, Degree>{std::atan2(x, y)*Rad_To_Deg};
    }
}

#endif