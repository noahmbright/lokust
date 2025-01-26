cmake_minimum_required(VERSION 3.20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(lokust)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(LLVM REQUIRED CONFIG)
set(LLVM_ENABLE_WARNINGS OFF)

message(STATUS "found llvm ${LLVM_PACKAGE_VERSION}")
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
llvm_map_components_to_libnames(llvm_libs support core irreader)

file(GLOB_RECURSE SOURCE_FILES 
	${CMAKE_SOURCE_DIR}/src/lexer.cpp
	${CMAKE_SOURCE_DIR}/src/parser.cpp
)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)

add_compile_options(-Wall -Wextra -pedantic -Werror -Fsanitize=address)

add_library(lokust_lib ${SOURCE_FILES})
target_link_libraries(lokust_lib ${llvm_libs})
link_libraries(lokust_lib)

add_executable(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/src/main.cpp)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()
link_libraries(GTest::gtest_main)

add_executable(
    lexer_test
    ${CMAKE_SOURCE_DIR}/tests/lexer_test.cpp
)

add_executable(
    parser_test
    ${CMAKE_SOURCE_DIR}/tests/parser_test.cpp
)


include(GoogleTest)
gtest_discover_tests(lexer_test)
gtest_discover_tests(parser_test)
