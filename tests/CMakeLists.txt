find_package(GTest REQUIRED)
include(CTest)

# Dummy file for clang-tidy
add_executable(clang-tidy-check clang-tidy-check.cpp)
target_link_libraries(clang-tidy-check PRIVATE Maxwell)

add_executable(test_template_string test_template_string.cpp)
add_test(NAME TestTemplateString COMMAND test_template_string)
target_link_libraries(test_template_string PRIVATE Maxwell GTest::gtest_main)
gtest_discover_tests(test_template_string)

add_executable(test_compile_time_math test_compile_time_math.cpp)
add_test(NAME TestCompileTimeMath COMMAND test_compile_time_math)
target_link_libraries(test_compile_time_math PRIVATE Maxwell GTest::gtest_main)
gtest_discover_tests(test_compile_time_math)

add_executable(test_dimension test_dimension.cpp)
add_test(NAME TestDimension COMMAND test_dimension)
target_link_libraries(test_dimension PRIVATE Maxwell GTest::gtest_main)
gtest_discover_tests(test_dimension)

add_executable(test_quantity_value test_quantity_value.cpp) 
add_test(NAME TestQuantityValue COMMAND test_quantity_value)
target_link_libraries(test_quantity_value PRIVATE Maxwell GTest::gtest_main)
gtest_discover_tests(test_quantity_value)

add_executable(test_quantity test_quantity.cpp)
add_test(NAME TestQuantity COMMAND test_quantity)
target_link_libraries(test_quantity PRIVATE Maxwell GTest::gtest_main)
gtest_discover_tests(test_quantity)

add_executable(test_units test_units.cpp)
add_test(NAME TestUnits COMMAND test_units)
target_link_libraries(test_units PRIVATE Maxwell GTest::gtest_main)
gtest_discover_tests(test_units)

add_executable(test_isq test_isq.cpp)
add_test(NAME TestISQ COMMAND test_isq)
target_link_libraries(test_isq PRIVATE Maxwell GTest::gtest_main)
gtest_discover_tests(test_isq)