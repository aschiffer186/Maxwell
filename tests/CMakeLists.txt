find_package(GTest REQUIRED)
include(CTest)

add_executable(test_utilities test_utilities.cpp)
target_link_libraries(test_utilities PRIVATE Maxwell GTest::gtest_main)
gtest_discover_tests(test_utilities)
add_test(NAME TestUtilities COMMAND test_utilities)

add_executable(test_dimension test_dimension.cpp)
target_link_libraries(test_dimension PRIVATE Maxwell GTest::gtest_main)
gtest_discover_tests(test_dimension)
add_test(NAME TestDimension COMMAND test_dimension)

add_executable(test_quantity test_quantity.cpp)
target_link_libraries(test_quantity PRIVATE Maxwell GTest::gtest_main) 
gtest_discover_tests(test_quantity)
add_test(NAME TestQuantity COMMAND test_quantity)

add_executable(test_quantity_math test_quantity_math.cpp)
target_link_libraries(test_quantity_math PRIVATE Maxwell GTest::gtest_main) 
gtest_discover_tests(test_quantity_math)
add_test(NAME TestQuantityMath COMMAND test_quantity_math)