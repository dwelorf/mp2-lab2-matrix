set(target "gtest")

add_library(${target} STATIC gtest-all.cc)

add_library(gtest_main STATIC gtest-all.cc)
target_compile_definitions(gtest_main PRIVATE GTEST_CREATE_SHARED_LIBRARY=1)

if((${CMAKE_CXX_COMPILER_ID} MATCHES "GNU" OR
    ${CMAKE_CXX_COMPILER_ID} MATCHES "Clang") AND
    (${CMAKE_SYSTEM_NAME} MATCHES "Linux"))
    set(pthread "-pthread")
endif()

target_link_libraries(${target} ${pthread})
target_link_libraries(gtest_main ${pthread})