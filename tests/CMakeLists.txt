add_executable(foo foo.cpp)
target_link_libraries(foo PRIVATE maxwell)

add_executable(test_utilities test_utilities.cpp)
target_link_libraries(test_utilities PRIVATE maxwell GTest::gtest_main)
add_test(NAME test_utilities COMMAND test_utilities)
gtest_discover_tests(test_utilities)

add_executable(test_dimensional_product test_dimensional_product.cpp)
target_link_libraries(test_dimensional_product PRIVATE maxwell GTest::gtest_main)
add_test(NAME test_dimensional_product COMMAND test_dimensional_product)
gtest_discover_tests(test_dimensional_product)

add_executable(test_unit test_unit.cpp)
target_link_libraries(test_unit PRIVATE maxwell GTest::gtest_main)
add_test(NAME test_unit COMMAND test_unit)
gtest_discover_tests(test_unit)