cmake_minimum_required(VERSION 2.8)

project(psychec)

set(PSYCHEC_CXX_FLAGS)
set(PSYCHEC_CXX_FLAGS "${PSYCHEC_CXX_FLAGS} -std=c++1y -g")
set(PSYCHEC_CXX_FLAGS "${PSYCHEC_CXX_FLAGS} -Wall -Wsign-compare")
set(PSYCHEC_CXX_FLAGS "${PSYCHEC_CXX_FLAGS} -DCPLUSPLUS_WITHOUT_QT")

set(PSYCHEC_SOURCES
    ${PROJECT_SOURCE_DIR}/Config.h
    # Parser's files
    ${PROJECT_SOURCE_DIR}/parser/AST.cpp
    ${PROJECT_SOURCE_DIR}/parser/AST.h
    ${PROJECT_SOURCE_DIR}/parser/ASTClone.cpp
    ${PROJECT_SOURCE_DIR}/parser/ASTMatch0.cpp
    ${PROJECT_SOURCE_DIR}/parser/ASTMatcher.cpp
    ${PROJECT_SOURCE_DIR}/parser/ASTMatcher.h
    ${PROJECT_SOURCE_DIR}/parser/ASTNormalizer.h
    ${PROJECT_SOURCE_DIR}/parser/ASTNormalizer.cpp
    ${PROJECT_SOURCE_DIR}/parser/ASTPatternBuilder.h
    ${PROJECT_SOURCE_DIR}/parser/ASTVisit.cpp
    ${PROJECT_SOURCE_DIR}/parser/ASTVisitor.cpp
    ${PROJECT_SOURCE_DIR}/parser/ASTVisitor.h
    ${PROJECT_SOURCE_DIR}/parser/ASTfwd.h
    ${PROJECT_SOURCE_DIR}/parser/Bind.cpp
    ${PROJECT_SOURCE_DIR}/parser/Bind.h
    ${PROJECT_SOURCE_DIR}/parser/CPlusPlus.h
    ${PROJECT_SOURCE_DIR}/parser/CPlusPlusForwardDeclarations.h
    ${PROJECT_SOURCE_DIR}/parser/Control.cpp
    ${PROJECT_SOURCE_DIR}/parser/Control.h
    ${PROJECT_SOURCE_DIR}/parser/CoreTypes.cpp
    ${PROJECT_SOURCE_DIR}/parser/CoreTypes.h
    ${PROJECT_SOURCE_DIR}/parser/DiagnosticClient.cpp
    ${PROJECT_SOURCE_DIR}/parser/DiagnosticClient.h
    ${PROJECT_SOURCE_DIR}/parser/FullySpecifiedType.cpp
    ${PROJECT_SOURCE_DIR}/parser/FullySpecifiedType.h
    ${PROJECT_SOURCE_DIR}/parser/Keywords.cpp
    ${PROJECT_SOURCE_DIR}/parser/Lexer.cpp
    ${PROJECT_SOURCE_DIR}/parser/Lexer.h
    ${PROJECT_SOURCE_DIR}/parser/LiteralTable.h
    ${PROJECT_SOURCE_DIR}/parser/Literals.cpp
    ${PROJECT_SOURCE_DIR}/parser/Literals.h
    ${PROJECT_SOURCE_DIR}/parser/MemoryPool.cpp
    ${PROJECT_SOURCE_DIR}/parser/MemoryPool.h
    ${PROJECT_SOURCE_DIR}/parser/Name.cpp
    ${PROJECT_SOURCE_DIR}/parser/Name.h
    ${PROJECT_SOURCE_DIR}/parser/NameVisitor.cpp
    ${PROJECT_SOURCE_DIR}/parser/Names.cpp
    ${PROJECT_SOURCE_DIR}/parser/Names.h
    ${PROJECT_SOURCE_DIR}/parser/ObjectiveCAtKeywords.cpp
    ${PROJECT_SOURCE_DIR}/parser/ObjectiveCTypeQualifiers.cpp
    ${PROJECT_SOURCE_DIR}/parser/ObjectiveCTypeQualifiers.h
    ${PROJECT_SOURCE_DIR}/parser/Parser.cpp
    ${PROJECT_SOURCE_DIR}/parser/Parser.h
    ${PROJECT_SOURCE_DIR}/parser/QtContextKeywords.cpp
    ${PROJECT_SOURCE_DIR}/parser/QtContextKeywords.h
    ${PROJECT_SOURCE_DIR}/parser/Scope.cpp
    ${PROJECT_SOURCE_DIR}/parser/Scope.h
    ${PROJECT_SOURCE_DIR}/parser/Symbol.cpp
    ${PROJECT_SOURCE_DIR}/parser/Symbol.h
    ${PROJECT_SOURCE_DIR}/parser/SymbolVisitor.h
    ${PROJECT_SOURCE_DIR}/parser/Symbols.cpp
    ${PROJECT_SOURCE_DIR}/parser/Symbols.h
    ${PROJECT_SOURCE_DIR}/parser/SyntaxAmbiguity.h
    ${PROJECT_SOURCE_DIR}/parser/Templates.cpp
    ${PROJECT_SOURCE_DIR}/parser/Templates.h
    ${PROJECT_SOURCE_DIR}/parser/Token.cpp
    ${PROJECT_SOURCE_DIR}/parser/Token.h
    ${PROJECT_SOURCE_DIR}/parser/TranslationUnit.cpp
    ${PROJECT_SOURCE_DIR}/parser/TranslationUnit.h
    ${PROJECT_SOURCE_DIR}/parser/Type.cpp
    ${PROJECT_SOURCE_DIR}/parser/Type.h
    ${PROJECT_SOURCE_DIR}/parser/TypeVisitor.cpp
    ${PROJECT_SOURCE_DIR}/parser/TypeVisitor.h
    # Generator's file
    ${PROJECT_SOURCE_DIR}/generator/Assert.h
    ${PROJECT_SOURCE_DIR}/generator/BuiltinNames.h
    ${PROJECT_SOURCE_DIR}/generator/CanonicalASTMatcher.h
    ${PROJECT_SOURCE_DIR}/generator/ConstraintGenerator.h
    ${PROJECT_SOURCE_DIR}/generator/ConstraintGenerator.cpp
    ${PROJECT_SOURCE_DIR}/generator/ConstraintWriter.h
    ${PROJECT_SOURCE_DIR}/generator/ConstraintWriter.cpp
    ${PROJECT_SOURCE_DIR}/generator/ConstraintSyntax.h
    ${PROJECT_SOURCE_DIR}/generator/Debug.h
    ${PROJECT_SOURCE_DIR}/generator/Debug.cpp
    ${PROJECT_SOURCE_DIR}/generator/DiagnosticCollector.h
    ${PROJECT_SOURCE_DIR}/generator/DiagnosticCollector.cpp
    ${PROJECT_SOURCE_DIR}/generator/Driver.h
    ${PROJECT_SOURCE_DIR}/generator/Driver.cpp
    ${PROJECT_SOURCE_DIR}/generator/Dumper.h
    ${PROJECT_SOURCE_DIR}/generator/Dumper.cpp
    ${PROJECT_SOURCE_DIR}/generator/Factory.h
    ${PROJECT_SOURCE_DIR}/generator/Factory.cpp
    ${PROJECT_SOURCE_DIR}/generator/FreshVarSupply.h
    ${PROJECT_SOURCE_DIR}/generator/FreshVarSupply.cpp
    ${PROJECT_SOURCE_DIR}/generator/Lookup.h
    ${PROJECT_SOURCE_DIR}/generator/Lookup.cpp
    ${PROJECT_SOURCE_DIR}/generator/Observer.h
    ${PROJECT_SOURCE_DIR}/generator/PrintfScanner.h
    ${PROJECT_SOURCE_DIR}/generator/PrintfScanner.cpp
    ${PROJECT_SOURCE_DIR}/generator/ScalarTypeLattice.h
    ${PROJECT_SOURCE_DIR}/generator/ScalarTypeLattice.cpp
    ${PROJECT_SOURCE_DIR}/generator/SymbolPP.h
    ${PROJECT_SOURCE_DIR}/generator/SymbolPP.cpp
    ${PROJECT_SOURCE_DIR}/generator/Tester.h
    ${PROJECT_SOURCE_DIR}/generator/Tester.cpp
    ${PROJECT_SOURCE_DIR}/generator/TypeOfExpr.h
    ${PROJECT_SOURCE_DIR}/generator/TypeOfExpr.cpp
    ${PROJECT_SOURCE_DIR}/generator/Utils.h
    ${PROJECT_SOURCE_DIR}/generator/Utils.cpp
    # Analysis files
    ${PROJECT_SOURCE_DIR}/analysis/Range.h
    ${PROJECT_SOURCE_DIR}/analysis/Range.cpp
    ${PROJECT_SOURCE_DIR}/analysis/RangeAnalysis.h
    ${PROJECT_SOURCE_DIR}/analysis/RangeAnalysis.cpp
    # Stubber's files
    ${PROJECT_SOURCE_DIR}/stubber/BasicStubber.h
    ${PROJECT_SOURCE_DIR}/stubber/BasicStubber.cpp
    # Utils' files
    ${PROJECT_SOURCE_DIR}/utils/CSyntax.h
    ${PROJECT_SOURCE_DIR}/utils/FunctionWriter.h
    ${PROJECT_SOURCE_DIR}/utils/FunctionWriter.cpp
    ${PROJECT_SOURCE_DIR}/utils/IO.h
    ${PROJECT_SOURCE_DIR}/utils/IO.cpp
    ${PROJECT_SOURCE_DIR}/utils/TypeSpeller.h
    ${PROJECT_SOURCE_DIR}/utils/TypeSpeller.cpp
    ${PROJECT_SOURCE_DIR}/utils/VersionedMap.h
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
    ${PROJECT_SOURCE_DIR}/parser
    ${PROJECT_SOURCE_DIR}/analysis
    ${PROJECT_SOURCE_DIR}/stubber
    ${PROJECT_SOURCE_DIR}/utils
    ${PROJECT_SOURCE_DIR}/command/cxxopts/include
)

set(LIBRARY psychec)
add_library(${LIBRARY} STATIC ${PSYCHEC_SOURCES})

set(GENERATOR psychecgen)
add_executable(${GENERATOR} ${GENERATOR_SOURCES})

target_link_libraries(${GENERATOR} ${LIBRARY})
