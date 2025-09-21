cmake_minimum_required(VERSION 3.5)

project(psychec)

# Directory setup.
set(PSYCHE_DIR ${PROJECT_SOURCE_DIR})

# Loading and rpath.
set(CMAKE_MACOSX_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "\$ORIGIN;@executable_path;@loader_path")

# Build the common lib.
add_subdirectory(common)

# Build the C frontend.
add_subdirectory(C)

# C++ standard.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags.
set(PSYCHEC_CXX_FLAGS)
set(PSYCHEC_CXX_FLAGS "${PSYCHEC_CXX_FLAGS} -g")
set(PSYCHEC_CXX_FLAGS "${PSYCHEC_CXX_FLAGS} -Wall \
                                            -Wsign-compare")

set(PSYCHEC_SOURCES
    # Data structures
    ${PROJECT_SOURCE_DIR}/data-structures/Substitution.h
    ${PROJECT_SOURCE_DIR}/data-structures/Substitution.cpp
    ${PROJECT_SOURCE_DIR}/data-structures/VersionedMap.h

    # Compiler support
    ${PROJECT_SOURCE_DIR}/compiler_support/gnu/GNUDirectorySearchOptions.h
    ${PROJECT_SOURCE_DIR}/compiler_support/gnu/GNUDirectorySearchOptions.cpp
    ${PROJECT_SOURCE_DIR}/compiler_support/gnu/GNUPreprocessorCommandOptions.h
    ${PROJECT_SOURCE_DIR}/compiler_support/gnu/GNUPreprocessorCommandOptions.cpp
    ${PROJECT_SOURCE_DIR}/compiler_support/gnu/GNUCompilerFacade.h
    ${PROJECT_SOURCE_DIR}/compiler_support/gnu/GNUCompilerFacade.cpp

    # Utility
    ${PROJECT_SOURCE_DIR}/utility/FileInfo.h
    ${PROJECT_SOURCE_DIR}/utility/FileInfo.cpp
    ${PROJECT_SOURCE_DIR}/utility/IO.h
    ${PROJECT_SOURCE_DIR}/utility/IO.cpp
    ${PROJECT_SOURCE_DIR}/utility/Process.h
    ${PROJECT_SOURCE_DIR}/utility/Process.cpp
)

set(CNIPPET_SOURCES
    ${PSYCHEC_SOURCES}
    ${PROJECT_SOURCE_DIR}/cnippet/CommandOptions.h
    ${PROJECT_SOURCE_DIR}/cnippet/CommandOptions.cpp
    ${PROJECT_SOURCE_DIR}/cnippet/CommandLineParser.h
    ${PROJECT_SOURCE_DIR}/cnippet/CommandLineParser.cpp
    ${PROJECT_SOURCE_DIR}/cnippet/Driver.h
    ${PROJECT_SOURCE_DIR}/cnippet/Driver.cpp
    ${PROJECT_SOURCE_DIR}/cnippet/Main.cpp
    ${PROJECT_SOURCE_DIR}/cnippet/Plugin.h
    ${PROJECT_SOURCE_DIR}/cnippet/Plugin.cpp
)

set(PSYCHE_TESTS_SOURCES
    ${PROJECT_SOURCE_DIR}/TestSuiteRunner.cpp
    ${PROJECT_SOURCE_DIR}/tests/Tester.h
    ${PROJECT_SOURCE_DIR}/tests/TestSuite.h
    ${PROJECT_SOURCE_DIR}/tests/TestSuite.cpp
)

foreach(file ${CNIPPET_SOURCES} ${PSYCHE_TESTS_SOURCES})
    set_source_files_properties(
        ${file} PROPERTIES
        COMPILE_FLAGS "${PSYCHEC_CXX_FLAGS}"
    )
endforeach()

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/C
)

set(GENERATOR cnip)
add_executable(${GENERATOR} ${CNIPPET_SOURCES})
target_link_libraries(${GENERATOR} psychecfe psychecommon dl)

#if (NOT WIN32 AND NOT MINGW)
    set(PSYCHE_TESTS test-suite)
    add_executable(${PSYCHE_TESTS} ${PSYCHE_TESTS_SOURCES})
    target_link_libraries(${PSYCHE_TESTS} psychecfe psychecommon dl)
#endif()

# Install setup
install(TARGETS ${GENERATOR}
    DESTINATION ${PROJECT_SOURCE_DIR}
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
	            GROUP_EXECUTE GROUP_READ
		        WORLD_EXECUTE WORLD_READ)
install(FILES ${PSYCHE_DIR}/psychecsolver-exe
	DESTINATION ${PROJECT_SOURCE_DIR}
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
	            GROUP_EXECUTE GROUP_READ
		        WORLD_EXECUTE WORLD_READ)
