cmake_minimum_required(VERSION 3.22.1)

project(
    taffy_cpp_lua

    LANGUAGES C
)

#[[
    TROUBLESHOOTING:

    ----------------------------------------------------------------------------

    NOTE: we use addition define 'LUA_C89_NUMBERS', specially to fix the next errors (happens for lua5.3):

    /usr/include/lua5.3/luaconf.h:579:2: error: #error "Compiler does not support 'long long'. Use option '-DLUA_32BITS'   or '-DLUA_C89_NUMBERS' (see file 'luaconf.h' for details)"
      579 | #error "Compiler does not support 'long long'. Use option '-DLUA_32BITS' \
          |  ^~~~~
    /usr/include/lua5.3/lua.h:93:9: error: unknown type name ‘LUA_INTEGER’
       93 | typedef LUA_INTEGER lua_Integer;
          |         ^~~~~~~~~~~
    /usr/include/lua5.3/lua.h:96:22: error: expected ‘=’, ‘,’, ‘;’, ‘asm’ or ‘__attribute__’ before ‘lua_Unsigned’
       96 | typedef LUA_UNSIGNED lua_Unsigned;
          |                      ^~~~~~~~~~~~

    ----------------------------------------------------------------------------

    Instead of linking with 'taffy_cpp_c', we use explicit it's targets names
    (not libraries names) - to ensure, that 'taffy_cpp_c' (added via add_directory())
    is actually build before linkage. Without it, 'taffy_cpp_c' not build.

    target_link_libraries(${PROJECT_NAME}_static PRIVATE taffy_cpp_c_static)
    target_link_libraries(${PROJECT_NAME}_shared PRIVATE taffy_cpp_c_shared)

    Reference: https://discourse.cmake.org/t/build-external-library-in-subdirectory/6342/8
]]

option(BUILD_STATIC_LIB "Build the static library" ON)
option(BUILD_SHARED_LIB "Build the shared library" ON)

set(TAFFY_CPP_C_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/../c/
)

add_subdirectory(
    ${TAFFY_CPP_C_DIR}

    "taffy_cpp_c_build_for_${PROJECT_NAME}"
)

set(LIBRARY_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/taffy_cpp_lua.h
)
set(LIBRARY_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/taffy_cpp_lua.c
)

set(C_COMPILER_FLAGS
    -std=c89 -pedantic #-ansi
    -Wall -Wextra #-Werror

    -Wmissing-prototypes -Wstrict-prototypes
    -Wold-style-definition

    -fPIC
)

set(LUA_INCLUDE_PATH
    "/usr/include/lua5.3"
)

if(BUILD_STATIC_LIB)

    add_library(
        ${PROJECT_NAME}_static STATIC

        ${LIBRARY_HEADERS} ${LIBRARY_SOURCES}
    )

    set_target_properties(
        ${PROJECT_NAME}_static PROPERTIES OUTPUT_NAME ${PROJECT_NAME}
    )

    target_compile_options(
        ${PROJECT_NAME}_static PRIVATE

        ${C_COMPILER_FLAGS}
    )

    target_compile_definitions(
        ${PROJECT_NAME}_static PRIVATE

        LUA_C89_NUMBERS
    )

    target_include_directories(
        ${PROJECT_NAME}_static PRIVATE

        ${CMAKE_CURRENT_SOURCE_DIR}/include/ # this library includes

        ${TAFFY_CPP_C_DIR}/include/ # `taffy_cpp_c` library includes

        ${LUA_INCLUDE_PATH}
    )

    target_link_libraries(
        ${PROJECT_NAME}_static PRIVATE

        # NOTE: to build 'taffy_cpp_c' library (via add_library()), here is we 
        # explicetely use 'taffy_cpp_c_static' for linking, not 'taffy_cpp_c' 
        taffy_cpp_c_static
    )

endif()

if(BUILD_SHARED_LIB)

    add_library(
        ${PROJECT_NAME}_shared SHARED

        ${LIBRARY_HEADERS} ${LIBRARY_SOURCES}
    )

    set_target_properties(
        ${PROJECT_NAME}_shared PROPERTIES OUTPUT_NAME ${PROJECT_NAME}
    )

    target_compile_options(
        ${PROJECT_NAME}_shared PRIVATE

        ${C_COMPILER_FLAGS}
    )

    target_compile_definitions(
        ${PROJECT_NAME}_shared PRIVATE

        LUA_C89_NUMBERS
    )

    target_include_directories(
        ${PROJECT_NAME}_shared PRIVATE

        ${CMAKE_CURRENT_SOURCE_DIR}/include/ # this library includes

        ${TAFFY_CPP_C_DIR}/include/ # `taffy_cpp_c` library includes

        ${LUA_INCLUDE_PATH}
    )

    target_link_libraries(
        ${PROJECT_NAME}_shared PRIVATE


        # NOTE: to build 'taffy_cpp_c' library (via add_library()), here is we 
        # explicetely use 'taffy_cpp_c_shared' for linking, not 'taffy_cpp_c' 
        taffy_cpp_c_shared # TODO: maybe link with 'static' lib here ?
    )

endif()
