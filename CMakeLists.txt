cmake_minimum_required (VERSION 3.22)

project (one_in_taiga VERSION 1.0)

add_subdirectory (src)

file (COPY assets DESTINATION ${CMAKE_BINARY_DIR})
