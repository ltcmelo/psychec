cmake_minimum_required(VERSION 2.8)

project(cfe)

# Build the stdlib plugin.
add_subdirectory(stdlib-support)

# C++ standard.
set(CMAKE_CXX_STANDARD 17)
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
    # Main
    ${PROJECT_SOURCE_DIR}/API.h
    ${PROJECT_SOURCE_DIR}/APIFwds.h
    ${PROJECT_SOURCE_DIR}/Compilation.h
    ${PROJECT_SOURCE_DIR}/Compilation.cpp
    ${PROJECT_SOURCE_DIR}/LanguageDialect.h
    ${PROJECT_SOURCE_DIR}/LanguageDialect.cpp
    ${PROJECT_SOURCE_DIR}/LanguageExtensions.h
    ${PROJECT_SOURCE_DIR}/LanguageExtensions.cpp
    ${PROJECT_SOURCE_DIR}/List.h
    ${PROJECT_SOURCE_DIR}/Managed.h
    ${PROJECT_SOURCE_DIR}/Managed.cpp
    ${PROJECT_SOURCE_DIR}/MemoryPool.h
    ${PROJECT_SOURCE_DIR}/MemoryPool.cpp
    ${PROJECT_SOURCE_DIR}/SemanticModel.h
    ${PROJECT_SOURCE_DIR}/SemanticModel.cpp
    ${PROJECT_SOURCE_DIR}/SyntaxTree.h
    ${PROJECT_SOURCE_DIR}/SyntaxTree.cpp
    ${PROJECT_SOURCE_DIR}/Unparser.h
    ${PROJECT_SOURCE_DIR}/Unparser.cpp

    # Parser
    ${PROJECT_SOURCE_DIR}/parser/Binder.h
    ${PROJECT_SOURCE_DIR}/parser/Binder.cpp
    ${PROJECT_SOURCE_DIR}/parser/DiagnosticsReporter_Lexer.cpp
    ${PROJECT_SOURCE_DIR}/parser/DiagnosticsReporter_Parser.cpp
    ${PROJECT_SOURCE_DIR}/parser/Keywords.cpp
    ${PROJECT_SOURCE_DIR}/parser/LexedTokens.h
    ${PROJECT_SOURCE_DIR}/parser/LexedTokens.cpp
    ${PROJECT_SOURCE_DIR}/parser/Lexer.h
    ${PROJECT_SOURCE_DIR}/parser/Lexer.cpp
    ${PROJECT_SOURCE_DIR}/parser/LineDirective.h
    ${PROJECT_SOURCE_DIR}/parser/LineDirective.cpp
    ${PROJECT_SOURCE_DIR}/parser/Parser.h
    ${PROJECT_SOURCE_DIR}/parser/Parser__IMPL__.inc
    ${PROJECT_SOURCE_DIR}/parser/Parser.cpp
    ${PROJECT_SOURCE_DIR}/parser/Parser_Declarations.cpp
    ${PROJECT_SOURCE_DIR}/parser/Parser_Expressions.cpp
    ${PROJECT_SOURCE_DIR}/parser/Parser_Statements.cpp
    ${PROJECT_SOURCE_DIR}/parser/ParseOptions.h
    ${PROJECT_SOURCE_DIR}/parser/ParseOptions.cpp
    ${PROJECT_SOURCE_DIR}/parser/PreprocessorOptions.h
    ${PROJECT_SOURCE_DIR}/parser/PreprocessorOptions.cpp
    ${PROJECT_SOURCE_DIR}/parser/TypeChecker.h
    ${PROJECT_SOURCE_DIR}/parser/TypeChecker.cpp

    # Syntax
    ${PROJECT_SOURCE_DIR}/syntax/SyntaxAmbiguity.h
    ${PROJECT_SOURCE_DIR}/syntax/SyntaxDisambiguator.h
    ${PROJECT_SOURCE_DIR}/syntax/SyntaxDisambiguator.cpp
    ${PROJECT_SOURCE_DIR}/syntax/SyntaxDumper.h
    ${PROJECT_SOURCE_DIR}/syntax/SyntaxFacts.h
    ${PROJECT_SOURCE_DIR}/syntax/SyntaxHolder.h
    ${PROJECT_SOURCE_DIR}/syntax/SyntaxHolder.cpp
    ${PROJECT_SOURCE_DIR}/syntax/SyntaxKind.h
    ${PROJECT_SOURCE_DIR}/syntax/SyntaxLexeme.h
    ${PROJECT_SOURCE_DIR}/syntax/SyntaxLexeme.cpp
    ${PROJECT_SOURCE_DIR}/syntax/SyntaxLexemes.h
    ${PROJECT_SOURCE_DIR}/syntax/SyntaxLexemes.cpp
    ${PROJECT_SOURCE_DIR}/syntax/SyntaxNamePrinter.h
    ${PROJECT_SOURCE_DIR}/syntax/SyntaxNamePrinter.cpp
    ${PROJECT_SOURCE_DIR}/syntax/SyntaxNode.cpp
    ${PROJECT_SOURCE_DIR}/syntax/SyntaxNode.h
    ${PROJECT_SOURCE_DIR}/syntax/SyntaxNodeList.h
    ${PROJECT_SOURCE_DIR}/syntax/SyntaxNodes.h
    ${PROJECT_SOURCE_DIR}/syntax/SyntaxNodes_Declarations.h
    ${PROJECT_SOURCE_DIR}/syntax/SyntaxNodes_Expressions.h
    ${PROJECT_SOURCE_DIR}/syntax/SyntaxNodes_Statements.h
    ${PROJECT_SOURCE_DIR}/syntax/SyntaxToken.cpp
    ${PROJECT_SOURCE_DIR}/syntax/SyntaxToken.h
    ${PROJECT_SOURCE_DIR}/syntax/SyntaxUtilities.cpp
    ${PROJECT_SOURCE_DIR}/syntax/SyntaxUtilities.h
    ${PROJECT_SOURCE_DIR}/syntax/SyntaxVisitor.cpp
    ${PROJECT_SOURCE_DIR}/syntax/SyntaxVisitor.h
    ${PROJECT_SOURCE_DIR}/syntax/SyntaxWriterDOTFormat.h
    ${PROJECT_SOURCE_DIR}/syntax/SyntaxWriterDOTFormat.cpp

    # Names
    ${PROJECT_SOURCE_DIR}/names/DeclarationName.cpp
    ${PROJECT_SOURCE_DIR}/names/DeclarationName.h
    ${PROJECT_SOURCE_DIR}/names/DeclarationNames.cpp
    ${PROJECT_SOURCE_DIR}/names/DeclarationNames.h
 
    # Plugin API files
    ${PROJECT_SOURCE_DIR}/plugin-api/PluginConfig.h
    ${PROJECT_SOURCE_DIR}/plugin-api/DeclarationInterceptor.h
    ${PROJECT_SOURCE_DIR}/plugin-api/SourceInspector.h
    ${PROJECT_SOURCE_DIR}/plugin-api/VisitorObserver.h

    # Tests
    ${PROJECT_SOURCE_DIR}/tests/TestBinder.h
    ${PROJECT_SOURCE_DIR}/tests/TestBinder.cpp
    ${PROJECT_SOURCE_DIR}/tests/TestFrontend.h
    ${PROJECT_SOURCE_DIR}/tests/TestFrontend.cpp
    ${PROJECT_SOURCE_DIR}/tests/TestParser.h
    ${PROJECT_SOURCE_DIR}/tests/TestParser.cpp
    ${PROJECT_SOURCE_DIR}/tests/TestParser_0000_0999.cpp
    ${PROJECT_SOURCE_DIR}/tests/TestParser_1000_1999.cpp
    ${PROJECT_SOURCE_DIR}/tests/TestParser_2000_2999.cpp
    ${PROJECT_SOURCE_DIR}/tests/TestParser_3000_3999.cpp
    ${PROJECT_SOURCE_DIR}/tests/TestTypeChecker.h
    ${PROJECT_SOURCE_DIR}/tests/TestTypeChecker.cpp
)

foreach(file ${CFE_SOURCES})
    set_source_files_properties(
        ${file} PROPERTIES
        COMPILE_FLAGS "${CFE_CXX_FLAGS}"
    )
endforeach()

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/..
)

set(LIBRARY psychecfe)
add_library(${LIBRARY} SHARED ${CFE_SOURCES})

target_link_libraries(${LIBRARY} psychecommon)

# Install setup
install(TARGETS ${LIBRARY} DESTINATION ${PROJECT_SOURCE_DIR}/../../../Deliverable)
