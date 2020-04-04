cmake_minimum_required(VERSION 2.8)

project(psychec)

# Directory setup.
set(PSYCHE_DIR ${PROJECT_SOURCE_DIR})
set(SOLVER_DIR ${PROJECT_SOURCE_DIR}/solver)

# Loading and rpath.
set(CMAKE_MACOSX_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "\$ORIGIN;@executable_path;@loader_path")

# Build the frontend.
add_subdirectory(frontend)

# Build the stdlib plugin.
add_subdirectory(stdlib-support)

# Build the solver.
execute_process(COMMAND
                stack install --local-bin-path ${PSYCHE_DIR}
                WORKING_DIRECTORY ${SOLVER_DIR}
		RESULT_VARIABLE SOLVER_RV
		OUTPUT_VARIABLE SOLVER_OV
		ERROR_VARIABLE  SOLVER_EV
		OUTPUT_STRIP_TRAILING_WHITESPACE
		ERROR_STRIP_TRAILING_WHITESPACE)
message("-- Solver build: ${SOLVER_RV} ${SOLVER_OV} ${SOLVER_EV}")

# C++ standard.
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags.
set(PSYCHEC_CXX_FLAGS)
set(PSYCHEC_CXX_FLAGS "${PSYCHEC_CXX_FLAGS} -g")
set(PSYCHEC_CXX_FLAGS "${PSYCHEC_CXX_FLAGS} -Wall \
                                            -Wsign-compare \
                                            -Wno-inconsistent-missing-override")
set(PSYCHEC_CXX_FLAGS "${PSYCHEC_CXX_FLAGS} -DCPLUSPLUS_WITHOUT_QT")

# Project sources.
set(PSYCHEC_SOURCES
    ${PROJECT_SOURCE_DIR}/Main.cpp

    # Data structures
    ${PROJECT_SOURCE_DIR}/adt/Substitution.h
    ${PROJECT_SOURCE_DIR}/adt/Substitution.cpp
    ${PROJECT_SOURCE_DIR}/adt/VersionedMap.h

    # Constraint generator
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
    ${PROJECT_SOURCE_DIR}/generator/FreshVarSupply.h
    ${PROJECT_SOURCE_DIR}/generator/FreshVarSupply.cpp
    ${PROJECT_SOURCE_DIR}/generator/PrintfScanner.h
    ${PROJECT_SOURCE_DIR}/generator/PrintfScanner.cpp

    # Driver
    ${PROJECT_SOURCE_DIR}/driver/Driver.h
    ${PROJECT_SOURCE_DIR}/driver/Driver.cpp
    ${PROJECT_SOURCE_DIR}/driver/Factory.h
    ${PROJECT_SOURCE_DIR}/driver/Factory.cpp
    ${PROJECT_SOURCE_DIR}/driver/GenericsInstantiatior.h
    ${PROJECT_SOURCE_DIR}/driver/GenericsInstantiatior.cpp
    ${PROJECT_SOURCE_DIR}/driver/Plugin.h
    ${PROJECT_SOURCE_DIR}/driver/Plugin.cpp

    # Plugin API files
    ${PROJECT_SOURCE_DIR}/plugin-api/PluginConfig.h
    ${PROJECT_SOURCE_DIR}/plugin-api/DeclarationInterceptor.h
    ${PROJECT_SOURCE_DIR}/plugin-api/SourceInspector.h
    ${PROJECT_SOURCE_DIR}/plugin-api/VisitorObserver.h

    # Range analysis
    ${PROJECT_SOURCE_DIR}/range-analysis/Range.h
    ${PROJECT_SOURCE_DIR}/range-analysis/Range.cpp
    ${PROJECT_SOURCE_DIR}/range-analysis/RangeAnalysis.h
    ${PROJECT_SOURCE_DIR}/range-analysis/RangeAnalysis.cpp

    # Tests
    ${PROJECT_SOURCE_DIR}/testing/BaseTester.h
    ${PROJECT_SOURCE_DIR}/testing/BaseTester.cpp
    ${PROJECT_SOURCE_DIR}/testing/TestDisambiguator.h
    ${PROJECT_SOURCE_DIR}/testing/TestDisambiguator.cpp
    ${PROJECT_SOURCE_DIR}/testing/TestParser.h
    ${PROJECT_SOURCE_DIR}/testing/TestParser.cpp

    # Tools
    ${PROJECT_SOURCE_DIR}/tools/BasicStubber.h
    ${PROJECT_SOURCE_DIR}/tools/BasicStubber.cpp
    ${PROJECT_SOURCE_DIR}/tools/CompilerFacade.h
    ${PROJECT_SOURCE_DIR}/tools/CompilerFacade.cpp
    ${PROJECT_SOURCE_DIR}/tools/FunctionWriter.h
    ${PROJECT_SOURCE_DIR}/tools/FunctionWriter.cpp
    ${PROJECT_SOURCE_DIR}/tools/ProgramValidator.h
    ${PROJECT_SOURCE_DIR}/tools/ProgramValidator.cpp

    # Utility files
    ${PROJECT_SOURCE_DIR}/utility/FileInfo.h
    ${PROJECT_SOURCE_DIR}/utility/FileInfo.cpp
    ${PROJECT_SOURCE_DIR}/utility/IO.h
    ${PROJECT_SOURCE_DIR}/utility/IO.cpp
    ${PROJECT_SOURCE_DIR}/utility/Process.h
    ${PROJECT_SOURCE_DIR}/utility/Process.cpp
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
    ${PROJECT_SOURCE_DIR}/adt
    ${PROJECT_SOURCE_DIR}/command/cxxopts/include
    ${PROJECT_SOURCE_DIR}/driver
    ${PROJECT_SOURCE_DIR}/frontend
    ${PROJECT_SOURCE_DIR}/generator
    ${PROJECT_SOURCE_DIR}/plugin-api
    ${PROJECT_SOURCE_DIR}/range-analysis
    ${PROJECT_SOURCE_DIR}/testing
    ${PROJECT_SOURCE_DIR}/tools
    ${PROJECT_SOURCE_DIR}/utility
)

set(GENERATOR psychecgen)
add_executable(${GENERATOR} ${PSYCHEC_SOURCES})

target_link_libraries(${GENERATOR} psychecfe dl)

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
