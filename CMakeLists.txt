# TODO: make install options
cmake_minimum_required(VERSION 3.0)

# PROJECT
project(memoiprof C)

# PACKAGES
find_package(GTK2 2.6 REQUIRED gtk)

# OPTIONS
option(SHARED "Build shared library" OFF)

# OUTPUT FILES
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin CACHE PATH "Output directory for runtime binaries")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib CACHE PATH "Output directory for shared libraries")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib CACHE PATH "Output directory for static libraries")


# COMPILER OPTIONS
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pipe")
set(CMAKE_C_FLAGS_DEBUG "-O0 -g -fsanitize=address")
set(CMAKE_C_FLAGS_RELEASE  "-O3 -fomit-frame-pointer -march=native -Wno-strict-aliasing -DNDEBUG")
set(CMAKE_BUILD_TYPE_INIT "Release")
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
if (SHARED)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
endif ()

# LOAD SUBDIRECTORIES
add_subdirectory(src)
