include(CTest)
find_package(GTest REQUIRED)

add_executable(TestMeasure TestMeasure.cpp)
target_link_libraries(TestMeasure PRIVATE Maxwell GTest::gtest_main)
add_test(NAME TestMeasure COMMAND TestMeasure)
gtest_discover_tests(TestMeasure)

add_executable(TestUnit TestUnit.cpp)
target_link_libraries(TestUnit PRIVATE Maxwell GTest::gtest_main)
add_test(NAME TestUnit COMMAND TestUnit)
gtest_discover_tests(TestUnit)

add_executable(TestQuantity TestQuantity.cpp)
target_link_libraries(TestQuantity PRIVATE Maxwell GTest::gtest_main)
add_test(NAME TestQuantity COMMAND TestQuantity)
gtest_discover_tests(TestQuantity)