![](https://github.com/ltcmelo/psychec/workflows/build/badge.svg)
![](https://github.com/ltcmelo/psychec/workflows/test-suite/badge.svg)

![](https://docs.google.com/drawings/d/e/2PACX-1vT-pCvcuO4U63ERkXWfBzOfVKwMQ_kh-ntzANYyNrnkt8FUV2wRHd5fN6snq33u5hWmnNQR3E3glsnH/pub?w=375&h=150)

Check the [open bounties and tasks](https://docs.google.com/document/d/1_xVLVXnrEJKGOXcJcy78NqdK2vnmIiJO33iOY7xaSGQ/edit?usp=sharing)!

# Psyche-C

Psyche is a compiler frontend for the C programming language that is specifically designed for the implementation of static analysis tools. These are the distinct features that make Psyche-C rather unique:

- Clean separation between the syntactic and semantic compiler phases.
- Algorithmic- and heuristic-based syntax disambiguation.
- Type inference for missing `struct`, `union`, `enum`, and `typedef` (i.e., tolerance against `#include` failures)
- API inspired by that of the [Roslyn .NET compiler](https://github.com/dotnet/roslyn).
- Parser's AST resembling that of the [LLVM's Clang frontend](https://clang.llvm.org/).

## Library and API

Psyche-C offers a C++ library/API for the implementation of static analysis tools of C programs.

```
void analyse(const FileInfo& fi)
{
    auto compilation = Compilation::create("code-analysis");
    auto tree = SyntaxTree::parseText(srcText,
                                      TextPreprocessingState::Preprocessed,
                                      ParseOptions(),
                                      fi.fileName());
    compilation->addSyntaxTrees({ tree.get() });

    CustomSyntaxVisitor analysis(tree.get(), compilation->semanticModel(tree.get()));
    analysis.run(tree->translationUnitRoot());
}
```

## The *cnippet* Driver

Psyche-C comes with the *cnippet* driver and may be used as an ordinary C parser.


```c
void f()
{
    int ;
}
```

If you compile the snippet above with *cnippet*, you'll see a diagnostic similar/equal to what you would see with GCC or Clang.

```
~ cnip test.c
test.c:4:4 error: declaration does not declare anything
int ;
    ^
```

Note: Semantic analysis isn't yet complete.

### Type Inference

*cnippet* understands code snippets (a.k.a. as incomplete programs or program fragments) through Psyche-C's type inference. 

```c
void f()
{
    T v = 0;
    v->value = 42;
    v->next = v;
}
```

If you compile the snippet above with GCC or Clang, you'll see a diagnostic such as _"declaration for_`T`_is not available"_.  
With *cnippet*, "compilation" succeeds, as the following definitions are implicitly produced.

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
- Blogs:
  - [Dumping a C program’s AST with Psyche-C](https://ltcmelo.github.io/psychec/2021/03/03/c-ast-dump-psyche.html) /
    [Visualizando a AST de um programa C com o Psyche-C](https://www.embarcados.com.br/visualizando-a-ast-psyche-c/)
- Articles:
  - [Programming in C with type inference](https://www.codeproject.com/Articles/1238603/Programming-in-C-with-Type-Inference) /
    [Programando em C com inferência de tipos usando PsycheC](https://www.embarcados.com.br/inferencia-de-tipos-em-c-usando-psychec/)

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
