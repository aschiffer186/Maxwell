set(MAXWELL_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(Maxwell INTERFACE 
            ${MAXWELL_INCLUDE}/Quantity.hpp
            ${MAXWELL_INCLUDE}/QuantityCore.hpp
            ${MAXWELL_INCLUDE}/QuantityLiterals.hpp
            ${MAXWELL_INCLUDE}/Unit.hpp 
            ${MAXWELL_INCLUDE}/UnitCore.hpp
            ${MAXWELL_INCLUDE}/UnitTypes.hpp)
            
target_compile_features(Maxwell INTERFACE cxx_std_20)
target_include_directories(Maxwell INTERFACE ${MAXWELL_INCLUDE})