cmake_minimum_required(VERSION 2.8)

project(psychecstd)

set(STD_CXX_FLAGS)
set(STD_CXX_FLAGS "${STD_CXX_FLAGS} -std=c++1y -g")
set(STD_CXX_FLAGS "${STD_CXX_FLAGS} -Wall -Wsign-compare -Wno-inconsistent-missing-override")

set(CMAKE_MACOSX_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "\$ORIGIN;@executable_path;@loader_path")

set(STD_SOURCES
    ${PROJECT_SOURCE_DIR}/CnippetPlugin.cpp
    ${PROJECT_SOURCE_DIR}/StdLibInterceptor.h
    ${PROJECT_SOURCE_DIR}/StdLibInterceptor.cpp
    ${PROJECT_SOURCE_DIR}/StdLibIndex.h
    ${PROJECT_SOURCE_DIR}/StdLibIndex.cpp
    ${PROJECT_SOURCE_DIR}/StdLibInspector.h
    ${PROJECT_SOURCE_DIR}/StdLibInspector.cpp
)

foreach(file ${STD_SOURCES})
    set_source_files_properties(
        ${file} PROPERTIES
        COMPILE_FLAGS "${STD_CXX_FLAGS}"
    )
endforeach()

include_directories(
    .
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/../generator
    ${PROJECT_SOURCE_DIR}/../frontend
    ${PROJECT_SOURCE_DIR}/../tools
    ${PROJECT_SOURCE_DIR}/../utility
    ${PROJECT_SOURCE_DIR}/../data-structures
    ${PROJECT_SOURCE_DIR}/../plugin-api
)

set(PLUGIN psychecstd)
add_library(${PLUGIN} SHARED ${STD_SOURCES})
set_target_properties(${PLUGIN}
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../)

# Link against the frontend.
target_link_libraries(${PLUGIN} psychecfe)

# Install setup
install(TARGETS ${PLUGIN} DESTINATION ${PROJECT_SOURCE_DIR}/../)
