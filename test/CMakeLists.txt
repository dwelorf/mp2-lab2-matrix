set(TEST_SOURCES
    test_main.cpp
    test_TDynamicStack.cpp
    test_TArithmeticExpression.cpp
)

add_executable(${MP2_TESTS} ${TEST_SOURCES})
target_link_libraries(${MP2_TESTS} ${MP2_LIBRARY} gtest gtest_main)