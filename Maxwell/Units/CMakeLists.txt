add_library(Maxwell-Units INTERFACE UnitBase.hh UnitTypes.hh)
target_include_directories(Maxwell-Units INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(Maxwell-Units INTERFACE cxx_std_20)