cmake_minimum_required(VERSION 2.8)

project(psychecstd)

# C++ standard.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags.
set(STD_CXX_FLAGS)
set(STD_CXX_FLAGS "${STD_CXX_FLAGS} -g")
set(STD_CXX_FLAGS "${STD_CXX_FLAGS} -Wall \
                                    -Wsign-compare")

set(STD_CXX_FLAGS "${STD_CXX_FLAGS} -DEXPORT_C_API -DEXPORT_PLUGIN_API")

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
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/..
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
