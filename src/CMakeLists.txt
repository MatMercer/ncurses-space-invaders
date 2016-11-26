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
add_executable(spaceinvaders ${SOURCES})
target_link_libraries(spaceinvaders ${CURSES_LIBRARIES})

# Debug
add_executable(spacedebug ${SOURCES})
target_link_libraries(spacedebug ${CURSES_LIBRARIES})
# Define the DEBUG flag for debug
set_target_properties (spacedebug PROPERTIES COMPILE_DEFINITIONS "DEBUG")

# Send the sound effects to bin folder
FILE(COPY ./spcinv/sfx DESTINATION ../bin/)