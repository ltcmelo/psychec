cmake_minimum_required(VERSION 2.8)

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

# Project sources.
set(PSYCHEC_SOURCES
    # Abstract data types
    ${PROJECT_SOURCE_DIR}/data-types/Substitution.h
    ${PROJECT_SOURCE_DIR}/data-types/Substitution.cpp
    ${PROJECT_SOURCE_DIR}/data-types/VersionedMap.h

    # Tools
    ${PROJECT_SOURCE_DIR}/tools/GnuCompilerFacade.h
    ${PROJECT_SOURCE_DIR}/tools/GnuCompilerFacade.cpp

    # Utilities
    ${PROJECT_SOURCE_DIR}/utility/FileInfo.h
    ${PROJECT_SOURCE_DIR}/utility/FileInfo.cpp
    ${PROJECT_SOURCE_DIR}/utility/IO.h
    ${PROJECT_SOURCE_DIR}/utility/IO.cpp
    ${PROJECT_SOURCE_DIR}/utility/Process.h
    ${PROJECT_SOURCE_DIR}/utility/Process.cpp
)

set(CNIPPET_SOURCES
    ${PSYCHEC_SOURCES}
    ${PROJECT_SOURCE_DIR}/cnippet/Main.cpp
    ${PROJECT_SOURCE_DIR}/cnippet/CompilerFrontend.h
    ${PROJECT_SOURCE_DIR}/cnippet/CompilerFrontend.cpp
    ${PROJECT_SOURCE_DIR}/cnippet/CompilerFrontend_C.h
    ${PROJECT_SOURCE_DIR}/cnippet/CompilerFrontend_C.cpp
    ${PROJECT_SOURCE_DIR}/cnippet/Configuration.h
    ${PROJECT_SOURCE_DIR}/cnippet/Configuration.cpp
    ${PROJECT_SOURCE_DIR}/cnippet/Configuration_C.h
    ${PROJECT_SOURCE_DIR}/cnippet/Configuration_C.cpp
    ${PROJECT_SOURCE_DIR}/cnippet/Driver.h
    ${PROJECT_SOURCE_DIR}/cnippet/Driver.cpp
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
    ${PROJECT_SOURCE_DIR}/command/cxxopts/include
    ${PROJECT_SOURCE_DIR}/C
    ${PROJECT_SOURCE_DIR}/adt
    ${PROJECT_SOURCE_DIR}/cnippet
    ${PROJECT_SOURCE_DIR}/tester
    ${PROJECT_SOURCE_DIR}/tools
    ${PROJECT_SOURCE_DIR}/utility
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
