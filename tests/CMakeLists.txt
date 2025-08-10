add_executable(compile_check compile_check.cpp)
target_link_libraries(compile_check Maxwell-Utility Maxwell-Systems Maxwell-Quantity)

find_package(GTest REQUIRED)
include(CTest)

add_executable(test_template_string test_template_string.cpp)
add_test(NAME TestTemplateString COMMAND test_template_string)
target_link_libraries(test_template_string PRIVATE Maxwell-Utility GTest::gtest_main)
gtest_discover_tests(test_template_string)