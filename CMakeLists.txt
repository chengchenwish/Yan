cmake_minimum_required(VERSION 3.5)
project(compiler)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-std=c++17 ")
add_library(compiler "")
target_sources(compiler
    PRIVATE
    parser/interp.cpp
    parser/parser.cpp
    parser/AST.cpp
    )
add_executable(YAN main)
target_link_libraries(YAN compiler)
                      #include_directories(server)

                      ##target_link_libraries(httpServer ${CMAKE_THREAD_LIBS_INIT})
                      set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
                      set(install_gtest OFF)
                      set(install_gmock OFF)
                      set(build_gmock ON)

                      # This project is tested using GoogleTest.
                      add_subdirectory("third_party/googletest")
                      set_property(TARGET gtest
                                  APPEND PROPERTY COMPILE_OPTIONS -Wno-missing-field-initializers)
                      set_property(TARGET gmock
                                              APPEND PROPERTY COMPILE_OPTIONS -Wno-missing-field-initializers)
                      add_executable(Test test/test_parser.cpp)
                      target_link_libraries(Test gtest)

