cmake_minimum_required(VERSION 3.19)
project(dots-example-gsp LANGUAGES CXX)
set(TARGET_NAME dots-example-gsp)

# includes
include(FetchContent)

# CMake options
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    # use default build type if none was specified
    set(DEFAULT_BUILD_TYPE "Release")
    message(STATUS "Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()
if (MSVC)
    # use standard C++ exception model and assume no exceptions for
    # external C functions note that this needs to be set globally to
    # include external targets because MSVC is disadvising from mixing
    # exceptions modes
    add_compile_options(/EHsc)
    # replace default warning level that CMake sets when using MSVC. the
    # override code is based on an example given by Kitware in [1]. note
    # that this issue should already have been fixed since CMake 3.15 (see
    # [2]), but for some reason still occurs with the CMake
    # 3.19.20122902-MSVC_2 version used by Visual Studio 16.9.3
    #
    # References:
    #
    #  - [1] https://gitlab.kitware.com/cmake/community/wikis/FAQ#dynamic-replace
    #
    #  - [2] https://gitlab.kitware.com/cmake/cmake/-/issues/19084
    foreach (flag_var
             CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
             CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
        string(REPLACE "/W3" "/W4" ${flag_var} "${${flag_var}}")
    endforeach()
endif()

# dependencies
option(DOTS_BUILD_EXAMPLES "Build the examples" OFF)
option(DOTS_BUILD_UNIT_TESTS "Build the unit tests" OFF)
FetchContent_Declare(
    dots-cpp
    GIT_REPOSITORY https://github.com/pnxs/dots-cpp.git
    GIT_TAG        9ef435eea60cd39fb4c9accf0fb6610afbff3358
)

FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG        8.0.1
)

FetchContent_MakeAvailable(dots-cpp fmt)

add_subdirectory(models/gsp-model)
add_subdirectory(services/alarm-service)
add_subdirectory(services/hmi-service)
add_subdirectory(services/sensor-service)
