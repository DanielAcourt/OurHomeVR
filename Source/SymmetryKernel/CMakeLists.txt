# Define the minimum version of CMake required to build this project.
cmake_minimum_required(VERSION 3.10)

# Set the project name. This will be the name of the solution/project in Visual Studio.
project(Wisp_Symmetry_Emulation)

# Enforce the C++17 standard, which is required for std::any and other modern features.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the 'Public' and 'Private' subdirectories to the include path.
# This allows source files to find headers with simple #include "header.h" syntax.
include_directories(Public Private)

# Use a glob to automatically find all .cpp source files in the Private directory.
# This will include SymmetryKernel.cpp and the new Main_Emulation.cpp.
file(GLOB SOURCES "Private/*.cpp")

# Define the executable target.
# It will be named "Wisp_Symmetry_Emulation" and will be built from the source files found above.
add_executable(Wisp_Symmetry_Emulation ${SOURCES})
