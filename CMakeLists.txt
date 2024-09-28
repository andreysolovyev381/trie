cmake_minimum_required(VERSION 3.19)
set (PROJECT_NAME "Trie implementation")
project(${PROJECT_NAME}
        VERSION 0.1
        DESCRIPTION "Basic trie implementation using C++20"
        LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(./include)
add_subdirectory(./tests)
