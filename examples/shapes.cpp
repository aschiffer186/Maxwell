/// \file shapes.cpp
/// \brief Brief example showing how creating derived quantities can lead to
/// enhanced type safety over using the pre-defined ISQ quantities.

#include <Maxwell.hpp>

#include <iostream>

using namespace maxwell;

class circle {
public:
  // Strongly type radius type, not constructible from length quantity
  template <auto U = si::meter_unit>
  using radius_type =
      quantity_value<U, make_derived_quantity_t<"radius", isq::length>{}>;

  explicit circle(radius_type<> r) : radius_{r} {}

  radius_type<> get_radius() const { return radius_; }

private:
  radius_type<> radius_{};
};

class rectangle {
public:
  template <auto U = si::meter_unit>
  using length_type =
      quantity_value<U, make_derived_quantity_t<"length", isq::length>{}>;

  template <auto U = si::meter_unit>
  using width_type =
      quantity_value<U, make_derived_quantity_t<"width", isq::length>{}>;

  explicit rectangle(length_type<> length, width_type<> width)
      : length_(length), width_(width) {}

  length_type<centi_unit<si::meter_unit>> get_length() const { return length_; }

  width_type<milli_unit<si::meter_unit>> get_width() const { return width_; }

private:
  length_type<> length_{};
  width_type<> width_{};
};

int main() {
  circle c(circle::radius_type<us::foot_unit>{1.0});
  // circle c2(us::foot<>{1.0}); Error - foot is not compatible with radius

  std::cout << "Circle radius: " << c.get_radius() << "\n"; // Prints 3.048 m

  rectangle::length_type<> length{10.0};
  rectangle::width_type<> width{20.0};

  rectangle r{length, width};
  //   rectangle r2{width, length}; // Error - won't compile because width and
  //   length
  // are switched

  std::cout << "Rectangle length, width: " << r.get_length() << ", "
            << r.get_width();

  return 0;
}