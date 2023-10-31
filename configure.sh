#!/bin/bash
# configure.sh
#
# Purpose: This script is used to configure the excel-simulation project using CMake.
# It assumes that CMake is properly installed and available in the system's PATH.
# The script creates a build directory (if it doesn't exist) and invokes CMake to 
# generate the necessary build files.
#
# Usage: ./configure.sh

# Check if CMake is installed
if ! command -v cmake &> /dev/null; then
    echo "Error: CMake is not installed. Please install CMake and try again."
    exit 1
fi

# Find the project's root directory by looking for the .project_root identifier
PROJECT_ROOT=$(git rev-parse --show-toplevel 2> /dev/null)
if [ -z "$PROJECT_ROOT" ]; then
    echo "Error: Unable to find the project's root directory. Are you sure you're inside the project?"
    exit 1
fi

# Navigate to the project's root directory
cd "$PROJECT_ROOT" || { echo "Error: Failed to navigate to the project's root directory."; exit 1; }

# Create the build directory if it does not exist
if [ ! -d "build" ]; then
    echo "Creating build directory..."
    mkdir build
    if [ $? -ne 0 ]; then
        echo "Error: Failed to create the build directory."
        exit 1
    fi
fi

# Configure the project with CMake
echo "Configuring the project..."
cmake -S . -B build || { echo "Error: CMake configuration failed."; exit 1; }

echo "Configuration successful."
