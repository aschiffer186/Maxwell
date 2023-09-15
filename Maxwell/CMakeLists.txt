set(MAXWELL_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(Maxwell INTERFACE 
            ${MAXWELL_INCLUDE}/Quantity.hpp
            ${MAXWELL_INCLUDE}/QuantityCore.hpp
            ${MAXWELL_INCLUDE}/Unit.hpp 
            ${MAXWELL_INCLUDE}/UnitCore.hpp
            )