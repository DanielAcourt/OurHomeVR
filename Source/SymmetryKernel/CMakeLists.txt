# Copyright (c) 2013-2025 Daniel Acourt. All Rights Reserved. Confidential & Proprietary.
# Define the minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

# Project Name: This is what will appear in IDEs (Visual Studio/Xcode)
project(Wisp_Symmetry_Kernel)

# Enforce C++17: Required for std::any, std::function, and modern memory management.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include Directories:
include_directories(Public Private .)

# Source Files:
file(GLOB SOURCES "Private/*.cpp")

# The Executable:
add_executable(Wisp_Symmetry_Emulation ${SOURCES})

# Professional Touch: Display a message after configuration
message(STATUS "----------------------------------------------------")
message(STATUS "Wisp Symmetry Kernel: Acourt-Negru Reference Build")
message(STATUS "Standard: C++17 | Target: Wisp_Symmetry_Emulation")
message(STATUS "----------------------------------------------------")
