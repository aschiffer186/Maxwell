add_library(${PROJECT_NAME} INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/core/dimension.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/core/quantity_holder.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/core/quantity_value.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core/quantity.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core/quantity_system.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core/scale.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core/unit.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/formatting/formatting.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/math/quantity_limits.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/quantity_systems/isq.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/quantity_systems/si.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/quantity_systems/si_constants.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/utility/compile_time_math.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/utility/config.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/utility/template_string.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/utility/type_traits.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Maxwell.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Maxwell_Core.hpp
    )
target_include_directories(Maxwell
INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_DIR}>
)
target_compile_features(Maxwell 
  INTERFACE cxx_std_20
)

if (${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.28" AND BUILD_MODULES)
add_library(Maxwell_Modules)
target_sources(Maxwell_Modules 
  PUBLIC FILE_SET CXX_MODULES FILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/Maxwell.cppm
 )
target_compile_features(Maxwell_Modules PUBLIC cxx_std_20)
target_compile_definitions(Maxwell_Modules PRIVATE MAXWELL_MODULES)
target_include_directories(Maxwell_Modules PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
endif()