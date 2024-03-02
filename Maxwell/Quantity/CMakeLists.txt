add_library(Maxwell-Quantity INTERFACE 
            QuantityBase.hh
            QuantityConcepts.hh
            QuantityTypes.hh)
target_include_directories(Maxwell-Quantity INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(Maxwell-Quantity INTERFACE cxx_std_20)
target_link_libraries(Maxwell-Quantity INTERFACE Maxwell-Units)