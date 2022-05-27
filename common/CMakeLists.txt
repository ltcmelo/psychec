cmake_minimum_required(VERSION 2.8)

project(psychecommon)

# C++ standard.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags.
set(COMMON_CXX_FLAGS)
set(COMMON_CXX_FLAGS "${CFE_CXX_FLAGS} -g")
set(COMMON_CXX_FLAGS "${CFE_CXX_FLAGS} -Wall \
                                       -Wsign-compare")
set(COMMON_CXX_FLAGS "${CFE_CXX_FLAGS} -DEXPORT_PSY_API")


set(CMAKE_MACOSX_RPATH TRUE)

set(COMMON_SOURCES
    # Main
    ${PROJECT_SOURCE_DIR}/API.h

    # Infra
    ${PROJECT_SOURCE_DIR}/infra/Assertions.h
    ${PROJECT_SOURCE_DIR}/infra/Escape.h
    ${PROJECT_SOURCE_DIR}/infra/InternalAccess.h
    ${PROJECT_SOURCE_DIR}/infra/Pimpl.h

    # Text
    ${PROJECT_SOURCE_DIR}/text/SourceText.h
    ${PROJECT_SOURCE_DIR}/text/SourceText.cpp
    ${PROJECT_SOURCE_DIR}/text/TextElement.h
    ${PROJECT_SOURCE_DIR}/text/TextElement.cpp
    ${PROJECT_SOURCE_DIR}/text/TextElementTable.h
    ${PROJECT_SOURCE_DIR}/text/TextSpan.h
    ${PROJECT_SOURCE_DIR}/text/TextSpan.cpp

    # Location
    ${PROJECT_SOURCE_DIR}/location/FileLinePositionSpan.cpp
    ${PROJECT_SOURCE_DIR}/location/FileLinePositionSpan.h
    ${PROJECT_SOURCE_DIR}/location/LinePosition.cpp
    ${PROJECT_SOURCE_DIR}/location/LinePosition.h
    ${PROJECT_SOURCE_DIR}/location/LinePositionSpan.cpp
    ${PROJECT_SOURCE_DIR}/location/LinePositionSpan.h
    ${PROJECT_SOURCE_DIR}/location/Location.cpp
    ${PROJECT_SOURCE_DIR}/location/Location.h

    # Diagnostics
    ${PROJECT_SOURCE_DIR}/diagnostics/Diagnostic.h
    ${PROJECT_SOURCE_DIR}/diagnostics/Diagnostic.cpp
    ${PROJECT_SOURCE_DIR}/diagnostics/DiagnosticCategory.cpp
    ${PROJECT_SOURCE_DIR}/diagnostics/DiagnosticCategory.h
    ${PROJECT_SOURCE_DIR}/diagnostics/DiagnosticDescriptor.cpp
    ${PROJECT_SOURCE_DIR}/diagnostics/DiagnosticDescriptor.h
    ${PROJECT_SOURCE_DIR}/diagnostics/DiagnosticSeverity.cpp
    ${PROJECT_SOURCE_DIR}/diagnostics/DiagnosticSeverity.h
)

foreach(file ${COMMON_SOURCES})
    set_source_files_properties(
        ${file} PROPERTIES
        COMPILE_FLAGS "${COMMON_CXX_FLAGS}"
    )
endforeach()

set(LIBRARY psychecommon)
add_library(${LIBRARY} SHARED ${COMMON_SOURCES})

# Install setup
install(TARGETS ${LIBRARY} DESTINATION ${PROJECT_SOURCE_DIR}/../../../Deliverable)
