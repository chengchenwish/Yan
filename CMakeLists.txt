cmake_minimum_required(VERSION 3.5)
project(compiler)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-std=c++17 ")
add_library(compiler "")
target_sources(compiler
    PRIVATE
    parser/lexer.cpp
    parser/parser.cpp
    parser/token.cpp
    parser/error.cpp
    parser/symbol.cpp
    parser/AST.cpp
    parser/gen.cpp
    )
add_executable(YAN main.cpp)
target_link_libraries(YAN compiler)
                      #include_directories(server)
option(YAN_BUILD_TESTS "Build YAN unit test" ON)
if(YAN_BUILD_TESTS)
    enable_testing()
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    set(install_gtest OFF)
    set(install_gmock OFF)
    set(build_gmock ON)
    # This project is tested using GoogleTest.
    add_subdirectory("third_part/googletest")
    set_property(TARGET gtest
                      APPEND PROPERTY COMPILE_OPTIONS -Wno-missing-field-initializers)
    set_property(TARGET gmock
                      APPEND PROPERTY COMPILE_OPTIONS -Wno-missing-field-initializers)
    add_executable(UT UT/test_parser.cpp
                        UT/testAll.cpp)
    add_custom_target(FT ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/test/run_test.py)
    target_link_libraries(UT gtest)
    target_link_libraries(UT compiler)
    add_test(NAME UT COMMAND UT)
    add_test(NAME FT COMMAND  FT)
endif(YAN_BUILD_TESTS)



