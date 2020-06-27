project(cstl_source)
cmake_minimum_required(VERSION 2.8)
MESSAGE(STATUS "#########################")
SET(CMAKE_BUILD_TYPE Release)
ADD_DEFINITIONS(-g -Wall )

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_subdirectory(example/1.template)
add_subdirectory(example/2.alloc)
add_subdirectory(example/3.iterators)
add_subdirectory(example/4.sequence_container)