# Copyright (c) 2023-present, eaxecx.
# This file is distributed under the MIT license (https://opensource.org/license/mit/)

add_library(argtable3 STATIC argtable3/argtable3.h argtable3/argtable3.c)
target_include_directories(argtable3 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(argtable3 PUBLIC m)
