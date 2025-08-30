add_executable(compile_check compile_check.cpp)
target_link_libraries(compile_check PRIVATE Maxwell)

find_package(GTest REQUIRED)
include(CTest)

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