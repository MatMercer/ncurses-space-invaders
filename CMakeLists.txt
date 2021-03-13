cmake_minimum_required(VERSION 2.8.12)

project(NCURSES_SPACE_INVADERS)

# Send all the executables to bin
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Ncurses library
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

# SFX assets
file(COPY src/spcinv/sfx DESTINATION ${CMAKE_BINARY_DIR}/bin)

# Space invaders library
file(GLOB spcinv_SOURCES src/spcinv/*.c src/spcinv/*.h)
add_library(spcinv ${spcinv_SOURCES})

# Space invaders main source code
file(GLOB ncurses-space-invaders_SOURCES src/*.c)
add_executable(ncurses-space-invaders ${ncurses-space-invaders_SOURCES})

# Creates the executable linking ncurses and the space invaders library
target_link_libraries(ncurses-space-invaders ${CURSES_LIBRARY} spcinv)

