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

find_package(CLI11)
find_package(Boost 1.83.0 COMPONENTS system CONFIG QUIET)
find_package(OpenSSL REQUIRED)

if(NOT Boost_FOUND)
	message(STATUS "Fetching Boost Asio from git repo.")

	FetchContent_Declare(
		Boost_asio
		GIT_REPOSITORY https://github.com/boostorg/asio.git
		GIT_TAG boost-1.83.0
	)

    FetchContent_MakeAvailable(Boost_asio)
endif()

find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(GTest 1.12.1 REQUIRED)
find_package(Catch2 REQUIRED)
find_package(spdlog REQUIRED)
find_package(Qt6 6.7 REQUIRED COMPONENTS QuickControls2 Sql)

if(PROJECT_IS_TOP_LEVEL)
	set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
	set(CMAKE_CXX_STANDARD_REQUIRED ON)

	# Let's ensure -std=c++xx instead of -std=g++xx
	set(CMAKE_CXX_STANDARD 23)
	set(CMAKE_CXX_EXTENSIONS OFF)
	# Testing only available if this is the main app. add_subdirectory(tests)
endif()

add_subdirectory(src)
