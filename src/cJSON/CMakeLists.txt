# SOURCES
set(cJSON_SRC
	cJSON.c
	cJSON.h
	)

# BUILD LIBRARY
add_library(cJSON STATIC ${cJSON_SRC})
target_include_directories(cJSON PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# LINK LIBRARIES FOR LIBRARY
target_link_libraries(cJSON m)
