cmake_minimum_required(VERSION 3.10)

project(NCURSES_SPACE_INVADERS)

# Directories for output files
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Find ncurses library
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

# Copy Sound Effects (SFX) assets to the output directory
file(COPY src/spcinv/sfx DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/sfx)

# Space Invaders Library
file(GLOB spcinv_SOURCES src/spcinv/*.c)
add_library(spcinv STATIC ${spcinv_SOURCES})

# Space Invaders Main Source Code
file(GLOB ncurses_space_invaders_SOURCES src/*.c)
add_executable(ncurses-space-invaders ${ncurses_space_invaders_SOURCES})

# Link libraries to the main executable
target_link_libraries(ncurses-space-invaders ${CURSES_LIBRARY} spcinv)
