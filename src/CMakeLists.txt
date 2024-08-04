# Copyright (c) 2023-present, eaxecx.
# This file is distributed under the MIT license (https://opensource.org/license/mit/)

set(HEADERS
    "${PROJECT_SOURCE_DIR}/include/ncpipes/cli.h"
    "${PROJECT_SOURCE_DIR}/include/ncpipes/config.h"
    "${PROJECT_SOURCE_DIR}/include/ncpipes/direction.h"
    "${PROJECT_SOURCE_DIR}/include/ncpipes/pipe.h"
    "${PROJECT_SOURCE_DIR}/include/ncpipes/point.h"
    "${PROJECT_SOURCE_DIR}/include/ncpipes/util.h"
)

set(SOURCES
    "${PROJECT_SOURCE_DIR}/src/cli.c"
    "${PROJECT_SOURCE_DIR}/src/main.c"
    "${PROJECT_SOURCE_DIR}/src/pipe.c"
    "${PROJECT_SOURCE_DIR}/src/util.c"
)

add_executable(ncpipes ${HEADERS} ${SOURCES})
target_include_directories(ncpipes PRIVATE "${PROJECT_SOURCE_DIR}/include")
target_compile_options(ncpipes PRIVATE -Wall -Wextra -Wconversion -Wsign-conversion -Wshadow)
target_compile_definitions(ncpipes PRIVATE _XOPEN_SOURCE=700)
target_link_libraries(ncpipes PRIVATE notcurses-core argtable3)

install(TARGETS ncpipes DESTINATION bin)
