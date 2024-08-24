![](https://github.com/ltcmelo/psychec/workflows/build/badge.svg)
![](https://github.com/ltcmelo/psychec/workflows/test-suite/badge.svg)

C language (draft) proposal [Enabling Generic Functions and Parametric Types in C](https://ltcmelo.com/n2698.pdf); prototype available [here](http://www.genericsinc.info/index.php).

![](https://docs.google.com/drawings/d/e/2PACX-1vT-pCvcuO4U63ERkXWfBzOfVKwMQ_kh-ntzANYyNrnkt8FUV2wRHd5fN6snq33u5hWmnNQR3E3glsnH/pub?w=375&h=150)


# Psyche-C

Psyche-C is a compiler frontend for the C language, designed as a library that can be used to implement static analysis tools for C.
Bellow are the main characteristics of Psyche-C:

- Clean separation between the syntactic and semantic compiler phases.
- Algorithmic and heuristic syntax disambiguation.
- Optional type inference for missing `struct`, `union`, `enum`, and `typedef` (due to `#include` failures).
- API inspired by that of the [Roslyn .NET compiler](https://github.com/dotnet/roslyn) and [LLVM's Clang frontend](https://clang.llvm.org/).

## Library and API

Psyche-C's native API is in C++, but APIs for other languages are [planned](https://github.com/ltcmelo/psychec/issues/112).

```cpp
void analyse(const SourceText& srcText, const FileInfo& fi)
{
    ParseOptions parseOpts;
    parseOpts.setAmbiguityMode(AmbiguityMode::DisambiguateAlgorithmically);

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

Psyche-C comes with the *cnippet* driver and it can be used as an ordinary parser for C snippets.
For instance, if you compile the snippet below with *cnippet*, you'll see a diagnostic similar to the one you'd see with GCC or Clang.

```c
void f()
{
    int ;
}
```

```
~ cnip test.c
test.c:4:4 error: declaration does not declare anything
int ;
    ^
```


## Type Inference

Psyche-C can optionally (only available in the [original branch](https://github.com/ltcmelo/psychec/tree/original)) infer the missing types of a C snippet.
For instance, for the snippet below, Psyche-C can infer a (compilable) declaration for `T` an synthesize it during compilation.

```c
void f()
{
    T v = 0;
    v->value = 42;
    v->next = v;
}
```


```c
typedef struct TYPE_2__ TYPE_1__;
struct TYPE_2__ 
{
    int value;
    struct TYPE_2__* next;
} ;
typedef TYPE_1__* T;
```

You might want to use this functionality to:

- Enable, on incomplete programs, static analysis tools that require complete programs.
- Compile a snippet (e.g. retrieved from a bug tracker) for object-code inspection.
- Generate test-input data for a function in isolation..
- Prototype an algorithm without specific data-structures.


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

Of Psyche-C itself:

- [Type Inference for C: Applications to the Static Analysis of Incomplete Programs](https://dl.acm.org/doi/10.1145/3421472)<br/>
ACM Transactions on Programming Languages and Systems — **TOPLAS**, Volume 42, Issue 3, Article No. 15, Dec. 2020.

- [Inference of static semantics for incomplete C programs](https://dl.acm.org/doi/10.1145/3158117)<br/>
Proceedings of the ACM on Programming Languages, Volume 2, Issue **POPL**, Jan. 2018, Article No. 29.

That use Psyche-C:

- [SLaDe: A Portable Small Language Model Decompiler for Optimized Assembly](https://ieeexplore.ieee.org/abstract/document/10444788)<br/>
Proceedings of the IEEE/ACM International Symposium on Code Generation and Optimization - **CGO**, 2024.

- [AnghaBench: a Suite with One Million Compilable C Benchmarks for Code-Size Reduction](https://conf.researchr.org/info/cgo-2021/accepted-papers)<br/>
Proceedings of the IEEE/ACM International Symposium on Code Generation and Optimization — **CGO**, 2021.

- [Generation of in-bounds inputs for arrays in memory-unsafe languages](https://dl.acm.org/citation.cfm?id=3314890)<br/>
Proceedings of the IEEE/ACM International Symposium on Code Generation and Optimization — **CGO**, Feb. 2019, p. 136-148.

- [Automatic annotation of tasks in structured code](https://dl.acm.org/citation.cfm?id=3243200)<br/>
Proceedings of the International Conference on Parallel Architectures and Compilation Techniques — **PACT**, Nov. 2018, Article No. 31.