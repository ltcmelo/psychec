![](https://github.com/ltcmelo/psychec/workflows/build/badge.svg)
![](https://github.com/ltcmelo/psychec/workflows/test-suite/badge.svg)

![](https://docs.google.com/drawings/d/e/2PACX-1vT-pCvcuO4U63ERkXWfBzOfVKwMQ_kh-ntzANYyNrnkt8FUV2wRHd5fN6snq33u5hWmnNQR3E3glsnH/pub?w=375&h=150)

# Psyche-C

Psyche is a rather unique compiler frontend for the C programming language that is specifically designed for the implementation of static analysis tools. This is where the "uniqueness" of Psyche-C comes from:

- Clean separation between the syntactic and semantic compiler phases.
- Algorithmic and heuristic syntax disambiguation.
- Type inference of missing `struct`, `union`, `enum`, and `typedef`  
  (i.e., tolerance and "recovery" against `#include` failures).
- API inspired by that of the [Roslyn .NET compiler](https://github.com/dotnet/roslyn).
- AST resembling that of the [LLVM's Clang frontend](https://clang.llvm.org/).

## Library and API

Psyche-C is implemented as a library. Its native API is in C++ (APIs for other languages are [on the way](https://github.com/ltcmelo/psychec/issues/112)).

```cpp
void analyse(const SourceText& srcText, const FileInfo& fi)
{
    ParseOptions parseOpts;
    parseOpts.setTreatmentOfAmbiguities(ParseOptions::TreatmentOfAmbiguities::DisambiguateAlgorithmically);
    
    auto tree = SyntaxTree::parseText(srcText,
                                      TextPreprocessingState::Preprocessed,
                                      TextCompleteness::Fragment,
                                      parseOpts,
                                      fi.fileName());

    auto compilation = Compilation::create("code-analysis");
    compilation->addSyntaxTree(tree.get());

    AnalysisVisitor analysis(tree.get(), compilation->semanticModel(tree.get()));
    analysis.run(tree->translationUnitRoot());
}
```

```cpp
SyntaxVisitor::Action AnalysisVisitor::visitFunctionDefinition(const FunctionDefinitionSyntax* node) override
{
    const sym = semaModel->declaredSymbol(node);
    if (sym->kind() == SymbolKind::Function) {
        const FunctionSymbol* funSym = sym->asFunction();
        // ...
    }
    return Action::Skip;
}

```


## The *cnippet* Driver

Psyche-C comes with the *cnippet* driver so that it can also be used as an ordinary C parser.


```c
void f()
{
    int ;
}
```

If you "compile" the snippet above with *cnippet*, you'll see a diagnostic similar/equal to what you would see with GCC or Clang.

```
~ cnip test.c
test.c:4:4 error: declaration does not declare anything
int ;
    ^
```

NOTE: Semantic analysis isn't yet complete.

## Type Inference

Psyche-C can infer the missing types of a code snippet (a.k.a. as an incomplete program or program fragment).

```c
void f()
{
    T v = 0;
    v->value = 42;
    v->next = v;
}
```

If you compile the snippet above with GCC or Clang, you'll see a diagnostic such as _"declaration for_`T`_is not available"_.  
With *cnippet*, "compilation" succeeds, as the following definitions are (implicitly) synthesised.

```c
typedef struct TYPE_2__ TYPE_1__;
struct TYPE_2__ 
{
    int value;
    struct TYPE_2__* next;
} ;
typedef TYPE_1__* T;
```

These are a few application of type inference for C:

- Enabling, on incomplete source-code, static analysis techniques that require fully-typed programs.
- Compiling partial code (e.g., a snippet retrieved from a bug tracker) for object-code inspection.
- Generating test-input data for a function in isolation (without its dependencies).
- Quick prototyping of an algorithm, without the need of explicit types.

NOTE: Type inference isn't yet available on master, only in the [original branch](https://github.com/ltcmelo/psychec/tree/original).

## Documentation and Resources

- The Doxygen-generated [API](https://ltcmelo.github.io/psychec/api-docs/html/index.html).
- A contributor's [wiki](https://github.com/ltcmelo/psychec/wiki).
- An [online interface](http://cuda.dcc.ufmg.br/psyche-c/) that offers a glimpse of Psyche-C's type inference functionality.
- Articles/blogs:
  - [Dumping a C program’s AST with Psyche-C](https://ltcmelo.github.io/psychec/2021/03/03/c-ast-dump-psyche.html)  
    (pt-BR) [Visualizando a AST de um programa C com o Psyche-C](https://www.embarcados.com.br/visualizando-a-ast-psyche-c/)
  - [Programming in C with type inference](https://www.codeproject.com/Articles/1238603/Programming-in-C-with-Type-Inference)  
    (pt-BR) [Programando em C com inferência de tipos usando PsycheC](https://www.embarcados.com.br/inferencia-de-tipos-em-c-usando-psychec/)

## Building and Testing

Except for type inference, which is written in Haskell, Psyche-C is written in C++17; *cnippet* is written in Python 3.

To build:

    cmake CMakeLists.txt && make -j 4

To run the tests:

    ./test-suite

## Related Publications

- [Type Inference for C: Applications to the Static Analysis of Incomplete Programs](https://dl.acm.org/doi/10.1145/3421472)<br/>
ACM Transactions on Programming Languages and Systems — **TOPLAS**, Volume 42, Issue 3, Article No. 15, Dec. 2020.

- [Inference of static semantics for incomplete C programs](https://dl.acm.org/doi/10.1145/3158117)<br/>
Proceedings of the ACM on Programming Languages, Volume 2, Issue **POPL**, Jan. 2018, Article No. 29.

- [AnghaBench: a Suite with One Million Compilable C Benchmarks for Code-Size Reduction](https://conf.researchr.org/info/cgo-2021/accepted-papers)<br/>
Proceedings of the IEEE/ACM International Symposium on Code Generation and Optimization — **CGO**, 2021.

- [Generation of in-bounds inputs for arrays in memory-unsafe languages](https://dl.acm.org/citation.cfm?id=3314890)<br/>
Proceedings of the IEEE/ACM International Symposium on Code Generation and Optimization — **CGO**, Feb. 2019, p. 136-148.

- [Automatic annotation of tasks in structured code](https://dl.acm.org/citation.cfm?id=3243200)<br/>
Proceedings of the International Conference on Parallel Architectures and Compilation Techniques — **PACT**, Nov. 2018, Article No. 31.
