find_package(GTest REQUIRED)
include(CTest)

add_executable(test_utilities test_utilities.cpp)
target_link_libraries(test_utilities PRIVATE Maxwell GTest::gtest_main)
gtest_discover_tests(test_utilities)
add_test(NAME TestUtilities COMMAND test_utilities)