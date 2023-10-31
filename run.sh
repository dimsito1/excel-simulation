#!/bin/bash
# run.sh
#
# Purpose: Run the excel-simulation executable.
#
# Usage: ./run.sh

# Define the root directory of your project
export EXCEL_SIMULATION_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Define the path to the executable
EXECUTABLE="$EXCEL_SIMULATION_ROOT/build/excel"

# Check if the executable exists
if [ ! -x "$EXECUTABLE" ]; then
    echo "Error: The executable '$EXECUTABLE' does not exist or is not executable."
    echo "Have you built the project using 'build.sh'?"
    exit 1
fi

# Run the executable
echo "Running the project..."
"$EXECUTABLE" || { echo "Error: Execution failed."; exit 1; }

echo "Execution successful."