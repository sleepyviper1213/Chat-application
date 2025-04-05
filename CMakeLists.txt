cmake_minimum_required(VERSION 3.24)

project(ChatApplication LANGUAGES CXX)

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# guard against in-source builds
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(
        FATAL_ERROR
            "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt. "
    )
endif()

include(FetchContent)

find_package (CLI11)
find_package (Boost 1.83.0 COMPONENTS system CONFIG QUIET)
find_package(OpenSSL REQUIRED)

if(NOT Boost_FOUND)
    message(STATUS "Fetching Boost Asio from git repo.")

    FetchContent_Declare(
        Boost_asio
        GIT_REPOSITORY https://github.com/boostorg/asio.git
        GIT_TAG boost-1.83.0)

    FetchContent_MakeAvailable(Boost_asio)
endif()

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.12.0
    FIND_PACKAGE_ARGS)
FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
    FIND_PACKAGE_ARGS NAMES GTest)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.4.0 # or a later release
    FIND_PACKAGE_ARGS NAMES catch2)
FetchContent_MakeAvailable(Catch2)

# CMakeLists.txt
find_package(nlohmann_json 3.2.0 REQUIRED)


if(PROJECT_IS_TOP_LEVEL)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    # Let's ensure -std=c++xx instead of -std=g++xx
	set(CMAKE_CXX_STANDARD 23)
    set(CMAKE_CXX_EXTENSIONS OFF)
    # Testing only available if this is the main app. add_subdirectory(tests)
endif()

add_subdirectory(src)
