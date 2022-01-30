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
                                    -Wsign-compare")

set(CFE_CXX_FLAGS "${CFE_CXX_FLAGS} -DEXPORT_C_API")
set(PLUGIN_CXX_FLAGS "${CFE_CXX_FLAGS} -DEXPORT_PLUGIN_API")

set(CMAKE_MACOSX_RPATH TRUE)

set(PLUGIN_SOURCES
    # Plugin API files
    ${PROJECT_SOURCE_DIR}/plugin-api/PluginConfig.h
    ${PROJECT_SOURCE_DIR}/plugin-api/DeclarationInterceptor.h
    ${PROJECT_SOURCE_DIR}/plugin-api/SourceInspector.h
)

set(CFE_SOURCES
    # Main
    ${PROJECT_SOURCE_DIR}/API.h
    ${PROJECT_SOURCE_DIR}/Fwds.h
    ${PROJECT_SOURCE_DIR}/SyntaxTree.h
    ${PROJECT_SOURCE_DIR}/SyntaxTree.cpp

    # Infra
    ${PROJECT_SOURCE_DIR}/infra/List.h
    ${PROJECT_SOURCE_DIR}/infra/Managed.h
    ${PROJECT_SOURCE_DIR}/infra/Managed.cpp
    ${PROJECT_SOURCE_DIR}/infra/MemoryPool.h
    ${PROJECT_SOURCE_DIR}/infra/MemoryPool.cpp

    # Syntax
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
    ${PROJECT_SOURCE_DIR}/syntax/SyntaxNodeList.cpp
    ${PROJECT_SOURCE_DIR}/syntax/SyntaxNodes.h
    ${PROJECT_SOURCE_DIR}/syntax/SyntaxNodes_Common.h
    ${PROJECT_SOURCE_DIR}/syntax/SyntaxNodes_Declarations.h
    ${PROJECT_SOURCE_DIR}/syntax/SyntaxNodes_Expressions.h
    ${PROJECT_SOURCE_DIR}/syntax/SyntaxNodes_Statements.h
    ${PROJECT_SOURCE_DIR}/syntax/SyntaxNodes_MIXIN.h
    ${PROJECT_SOURCE_DIR}/syntax/SyntaxReference.cpp
    ${PROJECT_SOURCE_DIR}/syntax/SyntaxReference.h
    ${PROJECT_SOURCE_DIR}/syntax/SyntaxToken.cpp
    ${PROJECT_SOURCE_DIR}/syntax/SyntaxToken.h
    ${PROJECT_SOURCE_DIR}/syntax/SyntaxUtilities.cpp
    ${PROJECT_SOURCE_DIR}/syntax/SyntaxUtilities.h
    ${PROJECT_SOURCE_DIR}/syntax/SyntaxVisitor.cpp
    ${PROJECT_SOURCE_DIR}/syntax/SyntaxVisitor.h
    ${PROJECT_SOURCE_DIR}/syntax/SyntaxWriterDOTFormat.h
    ${PROJECT_SOURCE_DIR}/syntax/SyntaxWriterDOTFormat.cpp

    # Parser
    ${PROJECT_SOURCE_DIR}/parser/DiagnosticsReporter_Lexer.cpp
    ${PROJECT_SOURCE_DIR}/parser/DiagnosticsReporter_Parser.cpp
    ${PROJECT_SOURCE_DIR}/parser/Keywords.cpp
    ${PROJECT_SOURCE_DIR}/parser/LanguageDialect.h
    ${PROJECT_SOURCE_DIR}/parser/LanguageDialect.cpp
    ${PROJECT_SOURCE_DIR}/parser/LanguageExtensions.h
    ${PROJECT_SOURCE_DIR}/parser/LanguageExtensions.cpp
    ${PROJECT_SOURCE_DIR}/parser/LexedTokens.h
    ${PROJECT_SOURCE_DIR}/parser/LexedTokens.cpp
    ${PROJECT_SOURCE_DIR}/parser/Lexer.h
    ${PROJECT_SOURCE_DIR}/parser/Lexer.cpp
    ${PROJECT_SOURCE_DIR}/parser/LineDirective.h
    ${PROJECT_SOURCE_DIR}/parser/LineDirective.cpp
    ${PROJECT_SOURCE_DIR}/parser/MacroTranslations.h
    ${PROJECT_SOURCE_DIR}/parser/MacroTranslations.cpp
    ${PROJECT_SOURCE_DIR}/parser/Parser.h
    ${PROJECT_SOURCE_DIR}/parser/Parser__IMPL__.inc
    ${PROJECT_SOURCE_DIR}/parser/Parser.cpp
    ${PROJECT_SOURCE_DIR}/parser/Parser_Common.cpp
    ${PROJECT_SOURCE_DIR}/parser/Parser_Declarations.cpp
    ${PROJECT_SOURCE_DIR}/parser/Parser_Expressions.cpp
    ${PROJECT_SOURCE_DIR}/parser/Parser_Statements.cpp
    ${PROJECT_SOURCE_DIR}/parser/ParseOptions.h
    ${PROJECT_SOURCE_DIR}/parser/ParseOptions.cpp
    ${PROJECT_SOURCE_DIR}/parser/TextPreprocessingState.h
    ${PROJECT_SOURCE_DIR}/parser/Unparser.h
    ${PROJECT_SOURCE_DIR}/parser/Unparser.cpp

    # Symbols
    ${PROJECT_SOURCE_DIR}/symbols/Accessibility.h
    ${PROJECT_SOURCE_DIR}/symbols/BuiltinTypeFacts.h
    ${PROJECT_SOURCE_DIR}/symbols/BuiltinTypeFacts.cpp
    ${PROJECT_SOURCE_DIR}/symbols/BuiltinTypeKind.h
    ${PROJECT_SOURCE_DIR}/symbols/Symbol.cpp
    ${PROJECT_SOURCE_DIR}/symbols/Symbol.h
    ${PROJECT_SOURCE_DIR}/symbols/Symbol__IMPL__.inc
    ${PROJECT_SOURCE_DIR}/symbols/SymbolKind.h
    ${PROJECT_SOURCE_DIR}/symbols/Symbol_Function.cpp
    ${PROJECT_SOURCE_DIR}/symbols/Symbol_Function.h
    ${PROJECT_SOURCE_DIR}/symbols/Symbol_LinkUnit.cpp
    ${PROJECT_SOURCE_DIR}/symbols/Symbol_LinkUnit.h
    ${PROJECT_SOURCE_DIR}/symbols/Symbol_Type.cpp
    ${PROJECT_SOURCE_DIR}/symbols/Symbol_Type.h
    ${PROJECT_SOURCE_DIR}/symbols/Symbol_Value.cpp
    ${PROJECT_SOURCE_DIR}/symbols/Symbol_Value.h
    ${PROJECT_SOURCE_DIR}/symbols/Symbols.h
    ${PROJECT_SOURCE_DIR}/symbols/SymbolName.cpp
    ${PROJECT_SOURCE_DIR}/symbols/SymbolName.h
    ${PROJECT_SOURCE_DIR}/symbols/SymbolName_Empty.cpp
    ${PROJECT_SOURCE_DIR}/symbols/SymbolName_Empty.h
    ${PROJECT_SOURCE_DIR}/symbols/SymbolName_Plain.cpp
    ${PROJECT_SOURCE_DIR}/symbols/SymbolName_Plain.h
    ${PROJECT_SOURCE_DIR}/symbols/SymbolName_Tag.cpp
    ${PROJECT_SOURCE_DIR}/symbols/SymbolName_Tag.h
    ${PROJECT_SOURCE_DIR}/symbols/SymbolNames.h
    ${PROJECT_SOURCE_DIR}/symbols/TypeKind.h
    ${PROJECT_SOURCE_DIR}/symbols/TypeSymbol__IMPL__.inc
    ${PROJECT_SOURCE_DIR}/symbols/TypeSymbol_Array.cpp
    ${PROJECT_SOURCE_DIR}/symbols/TypeSymbol_Array.h
    ${PROJECT_SOURCE_DIR}/symbols/TypeSymbol_Named.cpp
    ${PROJECT_SOURCE_DIR}/symbols/TypeSymbol_Named.h
    ${PROJECT_SOURCE_DIR}/symbols/TypeSymbol_Pointer.cpp
    ${PROJECT_SOURCE_DIR}/symbols/TypeSymbol_Pointer.h
    ${PROJECT_SOURCE_DIR}/symbols/ValueKind.h
    ${PROJECT_SOURCE_DIR}/symbols/ValueSymbol_Field.cpp
    ${PROJECT_SOURCE_DIR}/symbols/ValueSymbol_Field.h
    ${PROJECT_SOURCE_DIR}/symbols/ValueSymbol_Parameter.cpp
    ${PROJECT_SOURCE_DIR}/symbols/ValueSymbol_Parameter.h
    ${PROJECT_SOURCE_DIR}/symbols/ValueSymbol_Variable.cpp
    ${PROJECT_SOURCE_DIR}/symbols/ValueSymbol_Variable.h

    # Binder
    ${PROJECT_SOURCE_DIR}/binder/Binder.h
    ${PROJECT_SOURCE_DIR}/binder/Binder.cpp
    ${PROJECT_SOURCE_DIR}/binder/Binder_Declarators.cpp
    ${PROJECT_SOURCE_DIR}/binder/Binder_Specifiers.cpp
    ${PROJECT_SOURCE_DIR}/binder/DiagnosticsReporter_Binder.cpp
    ${PROJECT_SOURCE_DIR}/binder/DiagnosticsReporter_TypeChecker.cpp
    ${PROJECT_SOURCE_DIR}/binder/Semantics_TypeQualifiers.h
    ${PROJECT_SOURCE_DIR}/binder/Semantics_TypeQualifiers.cpp
    ${PROJECT_SOURCE_DIR}/binder/Semantics_TypeSpecifiers.h
    ${PROJECT_SOURCE_DIR}/binder/Semantics_TypeSpecifiers.cpp
    ${PROJECT_SOURCE_DIR}/binder/Scope.h
    ${PROJECT_SOURCE_DIR}/binder/Scope.cpp
    ${PROJECT_SOURCE_DIR}/binder/Scope_Block.cpp
    ${PROJECT_SOURCE_DIR}/binder/Scope_Block.h
    ${PROJECT_SOURCE_DIR}/binder/Scope_File.cpp
    ${PROJECT_SOURCE_DIR}/binder/Scope_File.h
    ${PROJECT_SOURCE_DIR}/binder/Scope_Function.cpp
    ${PROJECT_SOURCE_DIR}/binder/Scope_Function.h
    ${PROJECT_SOURCE_DIR}/binder/Scopes.h
    ${PROJECT_SOURCE_DIR}/binder/TypeChecker.h
    ${PROJECT_SOURCE_DIR}/binder/TypeChecker.cpp

    # Compilation
    ${PROJECT_SOURCE_DIR}/compilation/Assembly.cpp
    ${PROJECT_SOURCE_DIR}/compilation/Assembly.h
    ${PROJECT_SOURCE_DIR}/compilation/Compilation.h
    ${PROJECT_SOURCE_DIR}/compilation/Compilation.cpp
    ${PROJECT_SOURCE_DIR}/compilation/SemanticModel.h
    ${PROJECT_SOURCE_DIR}/compilation/SemanticModel.cpp

    # Tests
    ${PROJECT_SOURCE_DIR}/tests/TestBinder.h
    ${PROJECT_SOURCE_DIR}/tests/TestBinder.cpp
    ${PROJECT_SOURCE_DIR}/tests/TestBinder_0000_0999.cpp
    ${PROJECT_SOURCE_DIR}/tests/TestBinder_1000_1999.cpp
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

foreach(file ${PLUGIN_SOURCES})
    set_source_files_properties(
        ${file} PROPERTIES
        COMPILE_FLAGS "${PLUGIN_CXX_FLAGS}"
    )
endforeach()

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
add_library(${LIBRARY} SHARED ${CFE_SOURCES} ${PLUGIN_SOURCES})

target_link_libraries(${LIBRARY} psychecommon)

# Install setup
install(TARGETS ${LIBRARY} DESTINATION ${PROJECT_SOURCE_DIR}/../../../Deliverable)
