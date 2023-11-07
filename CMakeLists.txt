cmake_minimum_required(VERSION 3.10)

project(excel VERSION 1.0)

# Set cmake standard dependencies
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Allow the developer to select if they want to build debug or release
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

# Set the debug and release flags
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall")

# Set Sources variable for src files
set(SOURCES
    src/cell.cpp
    src/excel.cpp
    src/excelManager.cpp
    src/main.cpp
    src/terminal.cpp
    src/utility.cpp
    src/validator.cpp
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