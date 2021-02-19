![](https://github.com/ltcmelo/psychec/workflows/generator-build/badge.svg)
![](https://github.com/ltcmelo/psychec/workflows/solver-build/badge.svg)
![](https://github.com/ltcmelo/psychec/workflows/parser-tests/badge.svg)
![](https://github.com/ltcmelo/psychec/workflows/inference-tests/badge.svg)
![](https://github.com/ltcmelo/psychec/workflows/compilability-tests/badge.svg)

# Psyche-C

Psyche is a compiler frontend for the C programming language. Psyche-C is specifically designed for the implementation of static analysis tools. Below are the distinct features that make Psyche-C a rather unique C frontend:

- Clean separation between the syntactic and semantic phases.
- Both algorithmic- and heuristic-based syntax disambiguation strategies.
- Agnostic of `#include`, with type inference for missing `struct`, `union`, `enum`, and `typedef`.
- API inspired by that of the [Roslyn .NET compiler](https://github.com/dotnet/roslyn).
- Resemblance between its parser's AST that of the [LLVM's Clang frontend](https://clang.llvm.org/).

Check out [this online interface](http://cuda.dcc.ufmg.br/psyche-c/).

Applications:

- Enabling, on incomplete source-code, static analysis techniques that depend on fully-typed programs.
- Compiling a code snippet (e.g., retrieved from a bug tracker) for object-code inspection.
- Generating test-input data for individual functions.
- Quick prototyping of an algorithm, without the need of explicit types.

**NOTE**: The master branch is going through a major overhaul; at this point, it's expected that syntax analysis (parsing and AST construction) is functional, though. The original version of Psyche-C is available in [this branch](https://github.com/ltcmelo/psychec/tree/original).

## The Cnippet Driver Adaptor

While Psyche-C is primarily used as a library for the implementation of static analysis tools, it still is a compiler frontend, and may also be used as such — this is done through the *cnippet* driver adaptor.

```c
// node.c
void f()
{
    T v = 0;
    v->value = 42;
    v->next = v;
}
```

If you compile this snippet with GCC or Clang, you'd see a diagnostic _"the declaration for"_ `T` _"is not available"_. But with the *cnippet* driver adaptor (command line `./cnip.sh -f gcc -c node.c`), a definition for `T` is inferred and the compilation succeeds.

## Building and Testing

Requirements: cmake, a C++17 compiler, and the Haskell Stack.

    cmake CMakeLists.txt  
    make -j 4
    
    ./test-suite

## Related Publications

PsycheC is an ongoing research project.

- [Type Inference for C: Applications to the Static Analysis of Incomplete Programs](https://dl.acm.org/doi/10.1145/3421472)<br/>
ACM Transactions on Programming Languages and Systems — **TOPLAS**, Volume 42, Issue 3, Artible No. 15, December 2020.

- [Inference of static semantics for incomplete C programs](https://dl.acm.org/doi/10.1145/3158117)<br/>
Proceedings of the ACM on Programming Languages, Volume 2, Issue **POPL**, January 2018, Article No. 29.

- [AnghaBench: a Suite with One Million Compilable C Benchmarks for Code-Size Reduction](https://conf.researchr.org/info/cgo-2021/accepted-papers)<br/>
Proceedings of the IEEE/ACM International Symposium on Code Generation and Optimization — **CGO**, 2021.

- [Generation of in-bounds inputs for arrays in memory-unsafe languages](https://dl.acm.org/citation.cfm?id=3314890)<br/>
Proceedings of the IEEE/ACM International Symposium on Code Generation and Optimization — **CGO**, February 2019, p. 136-148.

- [Automatic annotation of tasks in structured code](https://dl.acm.org/citation.cfm?id=3243200)<br/>
Proceedings of the International Conference on Parallel Architectures and Compilation Techniques — **PACT**, November 2018, Article No. 31.

- [Synthesis of Benchmarks for the C Programming Language by Mining Software Repositories](https://dl.acm.org/citation.cfm?id=3355378.3355380)<br/>
Proceedings of the _Simpósio Brasileiro de Linguagens de Programacao_ — **SBLP**, September 2019.

An small article about PsycheC:

- [Programming in C with Type Inference](https://www.codeproject.com/Articles/1238603/Programming-in-C-with-Type-Inference)
