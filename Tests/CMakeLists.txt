enable_testing()

find_package(GTest REQUIRED)

add_executable(TestQuantity TestQuantity.cpp)
target_link_libraries(TestQuantity PRIVATE Maxwell GTest::gtest_main)
gtest_discover_tests(TestQuantity)