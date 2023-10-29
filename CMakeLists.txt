cmake_minimum_required(VERSION 3.10)

project(excel VERSION 1.0)

# Set cmake standard dependencies
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set Sources variable for src files
set(SOURCES
    src/cell.cpp
    src/excel.cpp
    src/excelManager.cpp
    src/main.cpp
    src/terminal.cpp
    src/utility.cpp
    src/validator.cpp
    src/vector.cpp
)

# Add an executable
add_executable(excel ${SOURCES})

# Include the "/include" headers to the executable
target_include_directories(excel
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
)

# Install executable to usr/local/bin with command "sudo make install"
install(TARGETS excel DESTINATION bin)