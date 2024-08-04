# Copyright (c) 2023-present, eaxecx.
# This file is distributed under the MIT license (https://opensource.org/license/mit/)

cmake_minimum_required(VERSION 3.7.2)

project(ncpipes VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_subdirectory(src)
add_subdirectory(third-party)
