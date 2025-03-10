
# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name
project(gsim VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -O2")

# Find Python development files (required for matplotlibcpp)
find_package(Python3 COMPONENTS Development REQUIRED)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${Python3_INCLUDE_DIRS}
)

# Source files
set(SOURCES
    src/main.cpp
    src/iniparser.cpp
    src/world.cpp
    src/particle.cpp
)

# Header files (for IDE visibility, not strictly necessary for compilation)
set(HEADERS
    include/iniparser.h
    include/world.h
    include/particle.h
    include/matplotlibcpp.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(${PROJECT_NAME}
    ${Python3_LIBRARIES}
)

# Ensure Python and HDF5 are found
if(NOT Python3_FOUND)
    message(FATAL_ERROR "Python3 development files not found. Please install python3-dev or equivalent.")
endif()

# Set output directory for the executable
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)