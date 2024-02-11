cmake_minimum_required(VERSION 3.22)

project(gfpp
    VERSION 0.1
    LANGUAGES C CXX
    DESCRIPTION "Implementation for calculations on Galois Fields in C++20")

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_FLAGS "-g -O3")
set(CMAKE_CXX_FLAGS "-g -O3")

# Find Boost Test package
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

add_library(gfpp-lib INTERFACE)
target_sources(gfpp-lib INTERFACE src/gf++.hpp)
target_include_directories(gfpp-lib INTERFACE ${CMAKE_SOURCE_DIR}/src)

add_executable(gf++)
target_sources(gf++ PRIVATE tools/gftool.cpp)
target_include_directories(gf++ PRIVATE ${CMAKE_SOURCE_DIR}/tools)
target_link_libraries(gf++ PRIVATE gfpp-lib)

enable_testing()

function(create_test name sources)
    add_executable(${name} ${sources})
    target_include_directories(${name} PRIVATE ${CMAKE_SOURCE_DIR}/test)
    target_link_libraries(${name} PRIVATE gfpp-lib Boost::unit_test_framework)
    target_compile_definitions(${name} PRIVATE -DBOOST_TEST_DYN_LINK -DBOOST_TEST_MODULE="GF++" -DBOOST_TEST_MAIN)
    add_test(NAME ${name} COMMAND ${name})
endfunction()

create_test(test_calc test/test_calc.cpp)
create_test(test_dump test/test_dump.cpp)
