
cmake_minimum_required(VERSION 3.3)

project("jsolver")

set(DEPS_DIR "${PROJECT_SOURCE_DIR}/deps")

include("cmake/functions.cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# setup configurations types
set(CMAKE_CONFIGURATION_TYPES Devel Release)

# check build configuration
if(NOT BUILD_TYPE)
    message(STATUS "Setting build type to 'Devel' as none was specified")
    set(BUILD_TYPE "Devel")
elseif(NOT (${BUILD_TYPE} IN_LIST CMAKE_CONFIGURATION_TYPES))
    message(FATAL_ERROR "Unknown build configuration (${BUILD_TYPE}). Use of supported configurations: ${CMAKE_CONFIGURATION_TYPES}")
endif()

set(CMAKE_BUILD_TYPE "${BUILD_TYPE}")
message(STATUS "Build type: ${BUILD_TYPE}")

# search for required libraries
find_package(Boost COMPONENTS program_options REQUIRED)
find_package(OpenCV 4 COMPONENTS core imgproc imgcodecs REQUIRED)

# Devel: optimal for regular development, with optimization and asserts
set(CMAKE_C_FLAGS_DEVEL "-g -O2")
set(CMAKE_CXX_FLAGS_DEVEL "-g -O2")

# Release: for making release, without asserts
set(CMAKE_C_FLAGS_RELEASE "-O2 -DRELEASE")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DRELEASE")

# common compile settings
add_compile_options(-std=gnu++17 -Wall -Wpedantic)

#add_subdirectory(deps)

set(LINK_BOOST
    boost_system
    boost_program_options
)


add_executable_from_dir(jsolver "jsolver")
target_include_directories(jsolver PUBLIC "." ${DEPS_DIR})
target_link_libraries(jsolver ${LINK_BOOST} ${OpenCV_LIBS})
#add_dependencies(jsolver OpenCV)
