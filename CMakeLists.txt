cmake_minimum_required(VERSION 3.1)

project(Zero
    VERSION 0.3.0
    LANGUAGES C)

# ------------------------------------------------------------------------------

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# ------------------------------------------------------------------------------

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU"
    OR "${CMAKE_C_COMPILER_ID}" STREQUAL "CLANG")
    add_compile_options(
        -Wpedantic -Wall -Wextra -Waggregate-return -Wcast-align -Wcast-qual
        -Wconversion -Wfloat-equal -Wpointer-arith -Wshadow -Wstrict-overflow=5
        -Wswitch -Wswitch-default -Wundef -Wunreachable-code -Wwrite-strings)
endif()

# ------------------------------------------------------------------------------

macro(zr_add_library target)
    set(ZR_ADD_LIBRARY_OPTIONS)
    set(ZR_ADD_LIBRARY_SINGLE_VALUE_ARGS)
    set(ZR_ADD_LIBRARY_MULTI_VALUE_ARGS DEPENDS)
    cmake_parse_arguments(
        ZR_ADD_LIBRARY
        "${ZR_ADD_LIBRARY_OPTIONS}"
        "${ZR_ADD_LIBRARY_SINGLE_VALUE_ARGS}"
        "${ZR_ADD_LIBRARY_MULTI_VALUE_ARGS}"
        ${ARGN})

    add_library(${target} INTERFACE)
    target_include_directories(${target}
        INTERFACE
            "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
            "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")
    target_link_libraries(${target}
        INTERFACE ${ZR_ADD_LIBRARY_DEPENDS})
    install(
        TARGETS ${target}
        EXPORT ${PROJECT_NAME}Targets)
    add_library(${PROJECT_NAME}::${target} ALIAS ${target})
endmacro()

zr_add_library(allocator)
zr_add_library(dynamicarray)
zr_add_library(logger)
zr_add_library(timer)

# ------------------------------------------------------------------------------

set(ZR_TOOL_TARGETS)

macro(zr_add_tool)
    set(ZR_ADD_TOOL_OPTIONS)
    set(ZR_ADD_TOOL_SINGLE_VALUE_ARGS NAME)
    set(ZR_ADD_TOOL_MULTI_VALUE_ARGS FILES)
    cmake_parse_arguments(
        ZR_ADD_TOOL
        "${ZR_ADD_TOOL_OPTIONS}"
        "${ZR_ADD_TOOL_SINGLE_VALUE_ARGS}"
        "${ZR_ADD_TOOL_MULTI_VALUE_ARGS}"
        ${ARGN})

    add_executable(tool-${ZR_ADD_TOOL_NAME} ${ZR_ADD_TOOL_FILES})
    set_target_properties(tool-${ZR_ADD_TOOL_NAME}
        PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY bin/tools
            OUTPUT_NAME ${ZR_ADD_TOOL_NAME})
    list(APPEND ZR_TOOL_TARGETS tool-${ZR_ADD_TOOL_NAME})
endmacro()

zr_add_tool(
    NAME build
    FILES tools/build/main.c)

add_custom_target(tools DEPENDS ${ZR_TOOL_TARGETS})

# ------------------------------------------------------------------------------

install(
    DIRECTORY include/zero
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# ------------------------------------------------------------------------------

set(ZR_CMAKE_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

configure_package_config_file(
    cmake/Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION ${ZR_CMAKE_INSTALL_DIR})

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    DESTINATION ${ZR_CMAKE_INSTALL_DIR})

install(
    EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${ZR_CMAKE_INSTALL_DIR})
