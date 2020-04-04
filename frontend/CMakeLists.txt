cmake_minimum_required(VERSION 2.8)

project(cfe)

# C++ standard.
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags.
set(CFE_CXX_FLAGS)
set(CFE_CXX_FLAGS "${CFE_CXX_FLAGS} -g")
set(CFE_CXX_FLAGS "${CFE_CXX_FLAGS} -Wall \
                                    -Wsign-compare \
                                    -Wno-inconsistent-missing-override")
set(CFE_CXX_FLAGS "${CFE_CXX_FLAGS} -DCPLUSPLUS_WITHOUT_QT")

set(CMAKE_MACOSX_RPATH TRUE)

set(CFE_SOURCES
    ${PROJECT_SOURCE_DIR}/Assert.h
    ${PROJECT_SOURCE_DIR}/AST.cpp
    ${PROJECT_SOURCE_DIR}/AST.h
    ${PROJECT_SOURCE_DIR}/ASTClone.cpp
    ${PROJECT_SOURCE_DIR}/ASTFwds.h
    ${PROJECT_SOURCE_DIR}/ASTDotWriter.h
    ${PROJECT_SOURCE_DIR}/ASTDotWriter.cpp
    ${PROJECT_SOURCE_DIR}/ASTIdentityMatcher.h
    ${PROJECT_SOURCE_DIR}/ASTIdentityMatcher.cpp
    ${PROJECT_SOURCE_DIR}/ASTDumper.h
    ${PROJECT_SOURCE_DIR}/ASTMatch0.cpp
    ${PROJECT_SOURCE_DIR}/ASTMatcher.cpp
    ${PROJECT_SOURCE_DIR}/ASTMatcher.h
    ${PROJECT_SOURCE_DIR}/ASTNormalizer.h
    ${PROJECT_SOURCE_DIR}/ASTNormalizer.cpp
    ${PROJECT_SOURCE_DIR}/ASTPatternBuilder.h
    ${PROJECT_SOURCE_DIR}/ASTVisit.cpp
    ${PROJECT_SOURCE_DIR}/ASTVisitor.cpp
    ${PROJECT_SOURCE_DIR}/ASTVisitor.h
    ${PROJECT_SOURCE_DIR}/Binder.cpp
    ${PROJECT_SOURCE_DIR}/Binder.h
    ${PROJECT_SOURCE_DIR}/BuiltinNames.h
    ${PROJECT_SOURCE_DIR}/Control.cpp
    ${PROJECT_SOURCE_DIR}/Control.h
    ${PROJECT_SOURCE_DIR}/CoreTypes.cpp
    ${PROJECT_SOURCE_DIR}/CoreTypes.h
    ${PROJECT_SOURCE_DIR}/Dialect.h
    ${PROJECT_SOURCE_DIR}/DiagnosticCollector.cpp
    ${PROJECT_SOURCE_DIR}/DiagnosticCollector.h
    ${PROJECT_SOURCE_DIR}/ExpressionResolver.cpp
    ${PROJECT_SOURCE_DIR}/ExpressionResolver.h
    ${PROJECT_SOURCE_DIR}/ExpressionTypeEvaluator.cpp
    ${PROJECT_SOURCE_DIR}/ExpressionTypeEvaluator.h
    ${PROJECT_SOURCE_DIR}/FrontendConfig.h
    ${PROJECT_SOURCE_DIR}/FrontendFwds.h
    ${PROJECT_SOURCE_DIR}/FullySpecifiedType.cpp
    ${PROJECT_SOURCE_DIR}/FullySpecifiedType.h
    ${PROJECT_SOURCE_DIR}/Keywords.cpp
    ${PROJECT_SOURCE_DIR}/Lexer.cpp
    ${PROJECT_SOURCE_DIR}/Lexer.h
    ${PROJECT_SOURCE_DIR}/LiteralTable.h
    ${PROJECT_SOURCE_DIR}/Literals.cpp
    ${PROJECT_SOURCE_DIR}/Literals.h
    ${PROJECT_SOURCE_DIR}/Lookup.h
    ${PROJECT_SOURCE_DIR}/Lookup.cpp
    ${PROJECT_SOURCE_DIR}/MemoryPool.cpp
    ${PROJECT_SOURCE_DIR}/MemoryPool.h
    ${PROJECT_SOURCE_DIR}/Name.cpp
    ${PROJECT_SOURCE_DIR}/Name.h
    ${PROJECT_SOURCE_DIR}/NameVisitor.cpp
    ${PROJECT_SOURCE_DIR}/Names.cpp
    ${PROJECT_SOURCE_DIR}/Names.h
    ${PROJECT_SOURCE_DIR}/Parser.cpp
    ${PROJECT_SOURCE_DIR}/Parser.h
    ${PROJECT_SOURCE_DIR}/Scope.cpp
    ${PROJECT_SOURCE_DIR}/Scope.h
    ${PROJECT_SOURCE_DIR}/Symbol.cpp
    ${PROJECT_SOURCE_DIR}/Symbol.h
    ${PROJECT_SOURCE_DIR}/SymbolVisitor.h
    ${PROJECT_SOURCE_DIR}/Symbols.cpp
    ${PROJECT_SOURCE_DIR}/Symbols.h
    ${PROJECT_SOURCE_DIR}/SyntaxAmbiguity.h
    ${PROJECT_SOURCE_DIR}/Templates.cpp
    ${PROJECT_SOURCE_DIR}/Templates.h
    ${PROJECT_SOURCE_DIR}/Token.cpp
    ${PROJECT_SOURCE_DIR}/Token.h
    ${PROJECT_SOURCE_DIR}/TranslationUnit.cpp
    ${PROJECT_SOURCE_DIR}/TranslationUnit.h
    ${PROJECT_SOURCE_DIR}/Type.cpp
    ${PROJECT_SOURCE_DIR}/Type.h
    ${PROJECT_SOURCE_DIR}/TypePP.h
    ${PROJECT_SOURCE_DIR}/TypePP.cpp
    ${PROJECT_SOURCE_DIR}/TypeVisitor.cpp
    ${PROJECT_SOURCE_DIR}/TypeVisitor.h
    ${PROJECT_SOURCE_DIR}/Unparser.cpp
    ${PROJECT_SOURCE_DIR}/Unparser.h
)

foreach(file ${CFE_SOURCES})
    set_source_files_properties(
        ${file} PROPERTIES
        COMPILE_FLAGS "${CFE_CXX_FLAGS}"
    )
endforeach()

include_directories(
    .
    ${PROJECT_SOURCE_DIR}
)

set(LIBRARY psychecfe)
add_library(${LIBRARY} SHARED ${CFE_SOURCES})

# Install setup
install(TARGETS ${LIBRARY} DESTINATION ${PROJECT_SOURCE_DIR}/../../../Deliverable)
