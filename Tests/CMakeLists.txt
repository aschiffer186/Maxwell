enable_testing()
include(CTest)

find_package(GTest REQUIRED)

add_executable(TestQuantity TestQuantity.cpp)
target_link_libraries(TestQuantity PRIVATE GTest::gtest_main Maxwell)
target_compile_options(TestQuantity PRIVATE "-Wall" "-Wextra" "-Wshadow" "-pedantic")
gtest_discover_tests(TestQuantity)

add_executable(TestUnits TestUnits.cpp)
target_link_libraries(TestUnits PRIVATE GTest::gtest_main Maxwell)
target_compile_options(TestUnits PRIVATE "-Wall" "-Wextra" "-Wshadow" "-pedantic")
gtest_discover_tests(TestUnits)