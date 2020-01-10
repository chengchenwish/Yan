cmake_minimum_required(VERSION 3.5)
project(compiler)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-std=c++17 ")
add_executable(compiler   parser/parser.cpp
                          parser/main.cpp
                          )
                      #include_directories(server)
                      #target_link_libraries(httpServer ${CMAKE_THREAD_LIBS_INIT})
