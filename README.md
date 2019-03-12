# memoiprof - Memoization Profiler

A library that collects function call argument data useful in memoization. This library has been developed and tested in Ubuntu 18. Use in other systems at your own risk.



## Dependencies

This library has the following library dependencies:

* GLib2 (package libglib2.0-dev)



## Building

If you have the needed dependencies installed, you can build this library automatically using the provided CMakeLists.txt file.

    cd memoiprof
    
    mkdir build
    cd build
    
    cmake ..
    make mp

The build script provides the following targets:

* default (`make`) - builds libmp.a and libcJSON.a
* mp (`make mp`) - builds libmp.a and libcJSON.a
* cJSON (`make cJSON`) - builds libcJSON.a
* mp-run (`make mp-run`) - builds mp-run, libmp.a and libcJSON.a



## Using with other cmake projects

To use this library with other projects using cmake as their build system, you can just copy the memoiprof directory to your code (where it makes sense to you) and add the following two lines to your CMakeLists.txt:

    add_subdirectory(memoiprof)
    target_link_libraries(<your-target> mp)    

Adjust the location of the directory and the CMakeLists lines as you see fit.



## IDE usage

Our current preferred IDE is CLion (https://www.jetbrains.com/clion/). You can create an account with a university email to have access to CLion. This directory can be imported directly as a CLion project.
