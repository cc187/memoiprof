# logger options
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -DZF_LOG_DEF_TAG=\\\"memoiprof\\\"")

# SOURCES
set(MP_SRC
        MemoiProfiler.c
        MemoiProfiler.h
        MemoiRecord.c
        MemoiRecord.h
        MemoiUtils.h
        MemoiProfilerUtils.c
        MemoiProfilerUtils.h
        MemoiUtils.c)

set(MP_RUN_SRC
        main.c)

# LOAD SUBDIRECTORIES
add_subdirectory(cJSON)
add_subdirectory(zf_log)

# BUILD LIBRARY
if (SHARED)
    add_library(mp SHARED ${MP_SRC})
else ()
    add_library(mp STATIC ${MP_SRC})
endif ()

target_include_directories(mp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# LINK LIBRARIES FOR LIBRARY
target_link_libraries(mp cJSON zf_log uuid)
include_directories(${GTK2_INCLUDE_DIRS})
target_link_libraries(mp ${GTK2_LIBRARIES})

# BUILD EXECUTABLE
add_executable(mp-run ${MP_RUN_SRC})
set_target_properties(mp-run PROPERTIES EXCLUDE_FROM_ALL TRUE)

# LINK LIBRARIES FOR EXECUTABLE
target_link_libraries(mp-run mp)







