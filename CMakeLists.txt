cmake_minimum_required(VERSION 2.8)

project(psychec)

# Build frontend's library.
add_subdirectory(frontend)

set(PSYCHEC_CXX_FLAGS)
set(PSYCHEC_CXX_FLAGS "${PSYCHEC_CXX_FLAGS} -std=c++1y -g")
set(PSYCHEC_CXX_FLAGS "${PSYCHEC_CXX_FLAGS} -Wall -Wsign-compare -Wno-inconsistent-missing-override")
set(PSYCHEC_CXX_FLAGS "${PSYCHEC_CXX_FLAGS} -DCPLUSPLUS_WITHOUT_QT")

set(PSYCHEC_SOURCES
    ${PROJECT_SOURCE_DIR}/Main.cpp
    ${PROJECT_SOURCE_DIR}/Config.h
    # Generator's file
    ${PROJECT_SOURCE_DIR}/generator/Assert.h
    ${PROJECT_SOURCE_DIR}/generator/BuiltinNames.h
    ${PROJECT_SOURCE_DIR}/generator/ConstraintGenerator.h
    ${PROJECT_SOURCE_DIR}/generator/ConstraintGenerator.cpp
    ${PROJECT_SOURCE_DIR}/generator/ConstraintWriter.h
    ${PROJECT_SOURCE_DIR}/generator/ConstraintWriter.cpp
    # ${PROJECT_SOURCE_DIR}/generator/ConstraintSyntax.h
    # ${PROJECT_SOURCE_DIR}/generator/CSyntax.h
    ${PROJECT_SOURCE_DIR}/generator/DeclarationInterceptor.h
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
    ${PROJECT_SOURCE_DIR}/generator/Observer.h
    ${PROJECT_SOURCE_DIR}/generator/PrintfScanner.h
    ${PROJECT_SOURCE_DIR}/generator/PrintfScanner.cpp
    ${PROJECT_SOURCE_DIR}/generator/StdLibIndex.h
    ${PROJECT_SOURCE_DIR}/generator/StdLibIndex.cpp
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
    # Utils' files
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
    ${PROJECT_SOURCE_DIR}/command/cxxopts/include
)

set(GENERATOR psychecgen)
add_executable(${GENERATOR} ${PSYCHEC_SOURCES})

target_link_libraries(${GENERATOR} psychecfe)
