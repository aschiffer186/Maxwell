add_library(Maxwell INTERFACE
    Maxwell.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/detail/Dimension.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/detail/Quantity.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/detail/QuantityMath.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/detail/QuantityRepo.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/detail/UnitRepo.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/detail/Unit.hpp)
target_include_directories(Maxwell INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})