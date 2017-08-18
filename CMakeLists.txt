cmake_minimum_required(VERSION 2.8)

project(psychec)

set(PSYCHEC_CXX_FLAGS)
set(PSYCHEC_CXX_FLAGS "${PSYCHEC_CXX_FLAGS} -std=c++1y -g")
set(PSYCHEC_CXX_FLAGS "${PSYCHEC_CXX_FLAGS} -Wall -Wsign-compare")
set(PSYCHEC_CXX_FLAGS "${PSYCHEC_CXX_FLAGS} -DCPLUSPLUS_WITHOUT_QT")

set(PSYCHEC_SOURCES
    ${PROJECT_SOURCE_DIR}/Config.h
    # Front-end's files
    ${PROJECT_SOURCE_DIR}/frontend/AST.cpp
    ${PROJECT_SOURCE_DIR}/frontend/AST.h
    ${PROJECT_SOURCE_DIR}/frontend/ASTClone.cpp
    ${PROJECT_SOURCE_DIR}/frontend/ASTIdentityMatcher.h
    ${PROJECT_SOURCE_DIR}/frontend/ASTIdentityMatcher.cpp
    ${PROJECT_SOURCE_DIR}/frontend/ASTDumper.h
    ${PROJECT_SOURCE_DIR}/frontend/ASTDumper.cpp
    ${PROJECT_SOURCE_DIR}/frontend/ASTMatch0.cpp
    ${PROJECT_SOURCE_DIR}/frontend/ASTMatcher.cpp
    ${PROJECT_SOURCE_DIR}/frontend/ASTMatcher.h
    ${PROJECT_SOURCE_DIR}/frontend/ASTNormalizer.h
    ${PROJECT_SOURCE_DIR}/frontend/ASTNormalizer.cpp
    ${PROJECT_SOURCE_DIR}/frontend/ASTPatternBuilder.h
    ${PROJECT_SOURCE_DIR}/frontend/ASTVisit.cpp
    ${PROJECT_SOURCE_DIR}/frontend/ASTVisitor.cpp
    ${PROJECT_SOURCE_DIR}/frontend/ASTVisitor.h
    ${PROJECT_SOURCE_DIR}/frontend/ASTfwd.h
    ${PROJECT_SOURCE_DIR}/frontend/Bind.cpp
    ${PROJECT_SOURCE_DIR}/frontend/Bind.h
    ${PROJECT_SOURCE_DIR}/frontend/CPlusPlus.h
    ${PROJECT_SOURCE_DIR}/frontend/CPlusPlusForwardDeclarations.h
    ${PROJECT_SOURCE_DIR}/frontend/Control.cpp
    ${PROJECT_SOURCE_DIR}/frontend/Control.h
    ${PROJECT_SOURCE_DIR}/frontend/CoreTypes.cpp
    ${PROJECT_SOURCE_DIR}/frontend/CoreTypes.h
    ${PROJECT_SOURCE_DIR}/frontend/Dialect.h
    ${PROJECT_SOURCE_DIR}/frontend/DiagnosticCollector.cpp
    ${PROJECT_SOURCE_DIR}/frontend/DiagnosticCollector.h
    ${PROJECT_SOURCE_DIR}/frontend/FullySpecifiedType.cpp
    ${PROJECT_SOURCE_DIR}/frontend/FullySpecifiedType.h
    ${PROJECT_SOURCE_DIR}/frontend/Keywords.cpp
    ${PROJECT_SOURCE_DIR}/frontend/Lexer.cpp
    ${PROJECT_SOURCE_DIR}/frontend/Lexer.h
    ${PROJECT_SOURCE_DIR}/frontend/LiteralTable.h
    ${PROJECT_SOURCE_DIR}/frontend/Literals.cpp
    ${PROJECT_SOURCE_DIR}/frontend/Literals.h
    ${PROJECT_SOURCE_DIR}/frontend/Lookup.h
    ${PROJECT_SOURCE_DIR}/frontend/Lookup.cpp
    ${PROJECT_SOURCE_DIR}/frontend/MemoryPool.cpp
    ${PROJECT_SOURCE_DIR}/frontend/MemoryPool.h
    ${PROJECT_SOURCE_DIR}/frontend/Name.cpp
    ${PROJECT_SOURCE_DIR}/frontend/Name.h
    ${PROJECT_SOURCE_DIR}/frontend/NameVisitor.cpp
    ${PROJECT_SOURCE_DIR}/frontend/Names.cpp
    ${PROJECT_SOURCE_DIR}/frontend/Names.h
    ${PROJECT_SOURCE_DIR}/frontend/ObjectiveCAtKeywords.cpp
    ${PROJECT_SOURCE_DIR}/frontend/ObjectiveCTypeQualifiers.cpp
    ${PROJECT_SOURCE_DIR}/frontend/ObjectiveCTypeQualifiers.h
    ${PROJECT_SOURCE_DIR}/frontend/Parser.cpp
    ${PROJECT_SOURCE_DIR}/frontend/Parser.h
    ${PROJECT_SOURCE_DIR}/frontend/QtContextKeywords.cpp
    ${PROJECT_SOURCE_DIR}/frontend/QtContextKeywords.h
    ${PROJECT_SOURCE_DIR}/frontend/Scope.cpp
    ${PROJECT_SOURCE_DIR}/frontend/Scope.h
    ${PROJECT_SOURCE_DIR}/frontend/Symbol.cpp
    ${PROJECT_SOURCE_DIR}/frontend/Symbol.h
    ${PROJECT_SOURCE_DIR}/frontend/SymbolPP.h
    ${PROJECT_SOURCE_DIR}/frontend/SymbolPP.cpp
    ${PROJECT_SOURCE_DIR}/frontend/SymbolVisitor.h
    ${PROJECT_SOURCE_DIR}/frontend/Symbols.cpp
    ${PROJECT_SOURCE_DIR}/frontend/Symbols.h
    ${PROJECT_SOURCE_DIR}/frontend/SyntaxAmbiguity.h
    ${PROJECT_SOURCE_DIR}/frontend/Templates.cpp
    ${PROJECT_SOURCE_DIR}/frontend/Templates.h
    ${PROJECT_SOURCE_DIR}/frontend/Token.cpp
    ${PROJECT_SOURCE_DIR}/frontend/Token.h
    ${PROJECT_SOURCE_DIR}/frontend/TranslationUnit.cpp
    ${PROJECT_SOURCE_DIR}/frontend/TranslationUnit.h
    ${PROJECT_SOURCE_DIR}/frontend/Type.cpp
    ${PROJECT_SOURCE_DIR}/frontend/Type.h
    ${PROJECT_SOURCE_DIR}/frontend/TypeOfExpression.cpp
    ${PROJECT_SOURCE_DIR}/frontend/TypeOfExpression.h
    ${PROJECT_SOURCE_DIR}/frontend/TypeSpeller.h
    ${PROJECT_SOURCE_DIR}/frontend/TypeSpeller.cpp
    ${PROJECT_SOURCE_DIR}/frontend/TypeVisitor.cpp
    ${PROJECT_SOURCE_DIR}/frontend/TypeVisitor.h
    # Generator's file
    ${PROJECT_SOURCE_DIR}/generator/Assert.h
    ${PROJECT_SOURCE_DIR}/generator/BuiltinNames.h
    ${PROJECT_SOURCE_DIR}/generator/ConstraintGenerator.h
    ${PROJECT_SOURCE_DIR}/generator/ConstraintGenerator.cpp
    ${PROJECT_SOURCE_DIR}/generator/ConstraintWriter.h
    ${PROJECT_SOURCE_DIR}/generator/ConstraintWriter.cpp
    ${PROJECT_SOURCE_DIR}/generator/ConstraintSyntax.h
    ${PROJECT_SOURCE_DIR}/generator/CSyntax.h
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
    ${PROJECT_SOURCE_DIR}/generator/Tester.h
    ${PROJECT_SOURCE_DIR}/generator/Tester.cpp
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
    # Utils' files
    ${PROJECT_SOURCE_DIR}/utility/IO.h
    ${PROJECT_SOURCE_DIR}/utility/IO.cpp
    ${PROJECT_SOURCE_DIR}/utility/Process.h
    ${PROJECT_SOURCE_DIR}/utility/Process.cpp
    # Data structures
    ${PROJECT_SOURCE_DIR}/data-structures/VersionedMap.h
)

set(GENERATOR_SOURCES
    ${PROJECT_SOURCE_DIR}/Main.cpp)

foreach(file ${PSYCHEC_SOURCES} ${GENERATOR_SOURCES})
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
    ${PROJECT_SOURCE_DIR}/command/cxxopts/include
)

set(LIBRARY psychec)
add_library(${LIBRARY} STATIC ${PSYCHEC_SOURCES})

set(GENERATOR psychecgen)
add_executable(${GENERATOR} ${GENERATOR_SOURCES})

target_link_libraries(${GENERATOR} ${LIBRARY})
