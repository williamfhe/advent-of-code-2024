cmake_minimum_required(VERSION 3.10)

# Set project name
project(AdventOfCode C)

# Specify the C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# Specify output directory for all binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

# Custom target to create the build directory if it doesn't exist
add_custom_target(create_build_dir ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    COMMENT "Ensuring build directory exists: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
)

# Add the Part 1 target
add_executable(part1 part1.c)
add_dependencies(part1 create_build_dir)

# Add the Part 2 target
add_executable(part2 part2.c)
add_dependencies(part2 create_build_dir)

# Custom target to build and run Part 1
add_custom_target(run_part1
    COMMAND $<TARGET_FILE:part1>
    DEPENDS part1
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Building and Running Part 1"
)

# Custom target to build and run Part 2
add_custom_target(run_part2
    COMMAND $<TARGET_FILE:part2>
    DEPENDS part2
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Building and Running Part 2"
)

# Optional: Message to show build directories
message(STATUS "Build directory: ${CMAKE_BINARY_DIR}")
message(STATUS "Binaries will be placed in: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
