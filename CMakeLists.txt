cmake_minimum_required(VERSION 2.8.12)

project(NCURSES_SPACE_INVADERS)

# Send all the executables to bin
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

file(GLOB main_SOURCES src/*.c)
add_executable(main ${main_SOURCES})

file(GLOB spcinv_SOURCES src/spcinv/*.c src/spcinv/*.h)

add_library(spcinv ${spcinv_SOURCES})

target_link_libraries(main ${CURSES_LIBRARY} spcinv)
