cmake_minimum_required(VERSION 4.0)
project(bibtool_wrapper C)

set(CMAKE_C_STANDARD 23)

add_library(bibtool_wrapper STATIC
        library.c
        apa.c
        apa.h
        records.c
        records.h
        types.h
        types.c
        bibtool_guard.h
)

add_executable(bibtool_wrapper_test
        test.c
)

target_include_directories(bibtool_wrapper PRIVATE
        libs/bibtool/include
)

target_link_libraries(bibtool_wrapper PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/libs/bibtool/libbibtool.a
)

target_include_directories(bibtool_wrapper_test PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(bibtool_wrapper_test PRIVATE
        bibtool_wrapper
)
