cmake_minimum_required(VERSION 2.4)

# Project setup, C language and name
enable_language(C)
project(space_invaders_game)

# Curses library
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

# Source files definitions
set(SOURCES main.c spcinv/spcinv.h spcinv/spcinv.c)

# Release
add_executable(space_invaders ${SOURCES})
target_link_libraries(space_invaders ${CURSES_LIBRARIES})

# Debug
add_definitions(-DDEBUG)
add_executable(space_debug ${SOURCES})
target_link_libraries(space_debug ${CURSES_LIBRARIES})
