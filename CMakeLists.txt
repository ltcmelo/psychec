cmake_minimum_required(VERSION 2.8)

project(psychec)

# Loading and rpath setup.
set(CMAKE_MACOSX_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "\$ORIGIN;@executable_path;@loader_path")

# Build frontend's library.
add_subdirectory(frontend)

set(PSYCHEC_CXX_FLAGS)
set(PSYCHEC_CXX_FLAGS "${PSYCHEC_CXX_FLAGS} -std=c++1y -g")
set(PSYCHEC_CXX_FLAGS "${PSYCHEC_CXX_FLAGS} -Wall -Wsign-compare -Wno-inconsistent-missing-override")
set(PSYCHEC_CXX_FLAGS "${PSYCHEC_CXX_FLAGS} -DCPLUSPLUS_WITHOUT_QT")

set(PSYCHEC_SOURCES
    ${PROJECT_SOURCE_DIR}/Main.cpp
    # Generator's file
    ${PROJECT_SOURCE_DIR}/generator/Assert.h
    ${PROJECT_SOURCE_DIR}/generator/BuiltinNames.h
    ${PROJECT_SOURCE_DIR}/generator/ConstraintGenerator.h
    ${PROJECT_SOURCE_DIR}/generator/ConstraintGenerator.cpp
    ${PROJECT_SOURCE_DIR}/generator/ConstraintWriter.h
    ${PROJECT_SOURCE_DIR}/generator/ConstraintWriter.cpp
    # ${PROJECT_SOURCE_DIR}/generator/ConstraintSyntax.h
    # ${PROJECT_SOURCE_DIR}/generator/CSyntax.h
    ${PROJECT_SOURCE_DIR}/generator/Debug.h
    ${PROJECT_SOURCE_DIR}/generator/Debug.cpp
    ${PROJECT_SOURCE_DIR}/generator/DomainLattice.h
    ${PROJECT_SOURCE_DIR}/generator/DomainLattice.cpp
    ${PROJECT_SOURCE_DIR}/generator/Driver.h
    ${PROJECT_SOURCE_DIR}/generator/Driver.cpp
    ${PROJECT_SOURCE_DIR}/generator/Factory.h
    ${PROJECT_SOURCE_DIR}/generator/Factory.cpp
    ${PROJECT_SOURCE_DIR}/generator/FreshVarSupply.h
    ${PROJECT_SOURCE_DIR}/generator/FreshVarSupply.cpp
    ${PROJECT_SOURCE_DIR}/generator/Plugin.h
    ${PROJECT_SOURCE_DIR}/generator/Plugin.cpp
    ${PROJECT_SOURCE_DIR}/generator/PrintfScanner.h
    ${PROJECT_SOURCE_DIR}/generator/PrintfScanner.cpp
    # Analysis files
    ${PROJECT_SOURCE_DIR}/static-analysis/Range.h
    ${PROJECT_SOURCE_DIR}/static-analysis/Range.cpp
    ${PROJECT_SOURCE_DIR}/static-analysis/RangeAnalysis.h
    ${PROJECT_SOURCE_DIR}/static-analysis/RangeAnalysis.cpp
    # Tools' files
    ${PROJECT_SOURCE_DIR}/tools/BasicStubber.h
    ${PROJECT_SOURCE_DIR}/tools/BasicStubber.cpp
    ${PROJECT_SOURCE_DIR}/tools/CompilerFacade.h
    ${PROJECT_SOURCE_DIR}/tools/CompilerFacade.cpp
    ${PROJECT_SOURCE_DIR}/tools/FunctionWriter.h
    ${PROJECT_SOURCE_DIR}/tools/FunctionWriter.cpp
    ${PROJECT_SOURCE_DIR}/tools/ProgramValidator.h
    ${PROJECT_SOURCE_DIR}/tools/ProgramValidator.cpp
    # Testing files
    ${PROJECT_SOURCE_DIR}/testing/BaseTester.h
    ${PROJECT_SOURCE_DIR}/testing/BaseTester.cpp
    ${PROJECT_SOURCE_DIR}/testing/TestDisambiguator.h
    ${PROJECT_SOURCE_DIR}/testing/TestDisambiguator.cpp
    ${PROJECT_SOURCE_DIR}/testing/TestParser.h
    ${PROJECT_SOURCE_DIR}/testing/TestParser.cpp
    # Plugin API files
    ${PROJECT_SOURCE_DIR}/plugin-api/PluginConfig.h
    ${PROJECT_SOURCE_DIR}/plugin-api/DeclarationInterceptor.h
    ${PROJECT_SOURCE_DIR}/plugin-api/SourceInspector.h
    ${PROJECT_SOURCE_DIR}/plugin-api/VisitorObserver.h
    # Utility files
    ${PROJECT_SOURCE_DIR}/utility/FileInfo.h
    ${PROJECT_SOURCE_DIR}/utility/FileInfo.cpp
    ${PROJECT_SOURCE_DIR}/utility/IO.h
    ${PROJECT_SOURCE_DIR}/utility/IO.cpp
    ${PROJECT_SOURCE_DIR}/utility/Process.h
    ${PROJECT_SOURCE_DIR}/utility/Process.cpp
    # Data structures
    ${PROJECT_SOURCE_DIR}/data-structures/VersionedMap.h
)

foreach(file ${PSYCHEC_SOURCES})
    set_source_files_properties(
        ${file} PROPERTIES
        COMPILE_FLAGS "${PSYCHEC_CXX_FLAGS}"
    )
endforeach()

include_directories(
    .
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/generator
    ${PROJECT_SOURCE_DIR}/frontend
    ${PROJECT_SOURCE_DIR}/static-analysis
    ${PROJECT_SOURCE_DIR}/tools
    ${PROJECT_SOURCE_DIR}/utility
    ${PROJECT_SOURCE_DIR}/data-structures
    ${PROJECT_SOURCE_DIR}/testing
    ${PROJECT_SOURCE_DIR}/plugin-api
    ${PROJECT_SOURCE_DIR}/command/cxxopts/include
)

set(GENERATOR psychecgen)
add_executable(${GENERATOR} ${PSYCHEC_SOURCES})

target_link_libraries(${GENERATOR} psychecfe dl)

# Install setup
install(TARGETS ${GENERATOR} DESTINATION ${PROJECT_SOURCE_DIR}/../../Deliverable)
