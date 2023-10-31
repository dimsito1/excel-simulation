#!/bin/bash
# build.sh
#
# Purpose: This script is used to configure and build the excel-simulation project.
# It assumes that CMake is properly installed and available in the system's PATH.
# The script navigates to the build directory, invokes CMake to configure the project,
# and then uses make to build it with 12 parallel jobs.
#
# Usage: ./build.sh

# Defining the number of parallel jobs for make
# CHANGE THIS NUMBER TO MAX NUMBER OF CPU CORES 
JOBS=12

# Check if CMake is installed
if ! command -v cmake &> /dev/null; then
    echo "Error: CMake is not installed. Please install CMake and try again."
    exit 1
fi

# Navigate to the project's root directory (assumes the script is in the project's root)
# If the script is located elsewhere, you will need to adjust the path accordingly.
cd "$(dirname "$0")" || { echo "Error: Failed to navigate to the script's directory."; exit 1; }

# Create the build directory if it does not exist
if [ ! -d "build" ]; then
    mkdir build
    if [ $? -ne 0 ]; then
        echo "Error: Failed to create the build directory."
        exit 1
    fi
fi

# Navigate to the build directory
cd build || { echo "Error: Failed to navigate to the build directory."; exit 1; }

# Configure the project with CMake
cmake .. || { echo "Error: CMake configuration failed."; exit 1; }

# Build the project
make -j "$JOBS" || { echo "Error: Build failed."; exit 1; }

echo "Build successfull"