cmake_minimum_required(VERSION 3.22.1)

project(
    taffy_cpp_c

    LANGUAGES CXX C
)

option(TAFFY_CPP_PATH   "Path to 'taffy_cpp' directory")
option(BUILD_STATIC_LIB "Build the static library" ON)
option(BUILD_SHARED_LIB "Build the shared library" ON)

add_subdirectory(
    ${TAFFY_CPP_PATH}

    "taffy_cpp_build_for_${PROJECT_NAME}"
)

set(LIBRARY_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/taffy_cpp_c.h
)
set(LIBRARY_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/taffy_cpp_c.cpp
)

set(CPP_COMPILER_FLAGS
    -std=c++11 -pedantic
    -Wall -Wextra #-Werror
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

        ${CPP_COMPILER_FLAGS}
    )

    target_include_directories(
        ${PROJECT_NAME}_static PRIVATE

        ${CMAKE_CURRENT_SOURCE_DIR}/include/
    )

    target_compile_definitions(${PROJECT_NAME}_static PRIVATE TAFFY_FEATURE_TAFFY_TREE)
    target_compile_definitions(${PROJECT_NAME}_static PRIVATE TAFFY_FEATURE_BLOCK_LAYOUT)
    target_compile_definitions(${PROJECT_NAME}_static PRIVATE TAFFY_FEATURE_FLEXBOX)
    target_compile_definitions(${PROJECT_NAME}_static PRIVATE TAFFY_FEATURE_GRID)

    target_link_libraries(
        ${PROJECT_NAME}_static PRIVATE

        taffy_cpp
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

        ${CPP_COMPILER_FLAGS}
    )

    target_include_directories(
        ${PROJECT_NAME}_shared PRIVATE

        ${CMAKE_CURRENT_SOURCE_DIR}/include/
    )

    target_compile_definitions(${PROJECT_NAME}_shared PRIVATE TAFFY_FEATURE_TAFFY_TREE)
    target_compile_definitions(${PROJECT_NAME}_shared PRIVATE TAFFY_FEATURE_BLOCK_LAYOUT)
    target_compile_definitions(${PROJECT_NAME}_shared PRIVATE TAFFY_FEATURE_FLEXBOX)
    target_compile_definitions(${PROJECT_NAME}_shared PRIVATE TAFFY_FEATURE_GRID)

    target_link_libraries(
        ${PROJECT_NAME}_shared PRIVATE

        taffy_cpp
    )

endif()
