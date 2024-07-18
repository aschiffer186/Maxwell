find_package(GTest REQUIRED)

add_executable(TestQuantity TestQuantity.cpp)
target_link_libraries(TestQuantity PRIVATE Maxwell GTest::gtest_main)
add_test(NAME TestQuantity COMMAND TestQuantity)
gtest_discover_tests(TestQuantity)

add_executable(TestUnitRepo TestUnitRepo.cpp)
target_link_libraries(TestUnitRepo PRIVATE Maxwell GTest::gtest_main)
add_test(NAME TestUnitRepo COMMAND TestUnitRepo)
gtest_discover_tests(TestQuantity TestUnitRepo)