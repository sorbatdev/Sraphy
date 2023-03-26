# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.24)

project(
    "Sraphy"
    VERSION 0.0.1
    LANGUAGES CXX C
)

option(BUILD_SHARED_LIBS "Whether the library should be built as a library or an executable" OFF)

if(${CMAKE_BUILD_TYPE} MATCHES "Release" OR ${CMAKE_BUILD_TYPE} MATCHES "MinSizeRel")
    add_compile_definitions(SR_RELEASE)
    message("-- Selected Release mode.")
else()
    add_compile_definitions(SR_DEBUG)
    message("-- Selected Debug mode.")
endif()

find_package(fmt CONFIG REQUIRED) # fmt::fmt
find_package(glfw3 CONFIG REQUIRED) # glfw
find_package(glm CONFIG REQUIRED) # glm::glm
find_package(glad CONFIG REQUIRED) # glad::glad

# Include sub-projects.
add_subdirectory("Source")
