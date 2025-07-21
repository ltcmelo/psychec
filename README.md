_Note (unrelated to Psyche-C)_  
C language draft proposal: [Enabling Generic Functions and Parametric Types in C](https://ltcmelo.com/n2698.pdf) ([prototype](http://www.genericsinc.info/index.php)).
<br><br><br>
![](https://github.com/ltcmelo/psychec/workflows/build/badge.svg)
![](https://github.com/ltcmelo/psychec/workflows/test-suite/badge.svg)

![](https://docs.google.com/drawings/d/e/2PACX-1vT-pCvcuO4U63ERkXWfBzOfVKwMQ_kh-ntzANYyNrnkt8FUV2wRHd5fN6snq33u5hWmnNQR3E3glsnH/pub?w=375&h=150)


# Psyche-C

Psyche-C is a platform for implementing static analysis of C programs.
At its core, it includes a C compiler frontend that performs both syntactic and semantic analysis.
Yet, as opposed to actual C compilers, Psyche-C doesn't build a symbol table during parsing.
Despite this, even with zero setup or in broken build setups,
Psyche-C still offers accurate syntax analysis (through syntax disambiguation) and partial semantic analysis.  

Bellow are the main characteristics of Psyche-C:

- Clean separation between the syntactic and semantic compiler phases.
- Algorithmic and heuristic syntax disambiguation.
- Optional type inference as a recovery mechanism from `#include` failures (not yet in master).
- API inspired by that of the [Roslyn .NET compiler](https://github.com/dotnet/roslyn) and [LLVM's Clang](https://clang.llvm.org/).

## Driver

Psyche-C is written as a C++ library, but it comes with a builtin driver: *cnippet*.
You can use it by passing to it either
the typical command line arguments of an actual compiler
or
the actual compiler's whole invocation as a subcommand.

Example with *cnippet* only:

```
cnip -analysis /path/to/analysis.dylib -I/path/to/whatever file.c
```

Example with compiler's invocation as a subcommand:

```
cnip -analysis /path/to/analysis.dylib -- gcc -I/path/to/whatever file.c
```


See [psychec-analysis](https://github.com/ltcmelo/psychec-analysis) for a trivial example of how to implement an analysis.

### Type Inference

Psyche-C began as a type inference tool for C, aimed at enabling static analysis of incomplete programs.
However, the compiler frontend at its core wasn't good enough, so I decided to rewrite it pretty much from scratch.
I used this rewrite also as an opportunity to extend Psyche-C into a platform for static analysis in general.
The result of this work is what exists today in the master branch,
but that doesn't yet include a port of the type inference from the [original branch](https://github.com/ltcmelo/psychec/tree/original).  

With type inference enabled, if you "compile" the snippet below with *cnippet*, Psyche-C will infer `T` and synthesize a declaration for it.

```c
void f()
{
    T v = 0;
    v->value = 42;
    v->next = v;
}
```

Synthesized declaration for `T`.

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

- Enable, on incomplete programs, analyses that depend on complete programs.
- Generate test-input/mocks to validate functions in isolation.
- Prototype an algorithm while only sketching its data-structures
- Compile a snippet for inspection of static properties of its object code.


## Documentation and Resources

- The Doxygen-generated [API](https://ltcmelo.github.io/psychec/api-docs/html/index.html).
- A contributor's [wiki](https://github.com/ltcmelo/psychec/wiki).
- An [online interface](http://cuda.dcc.ufmg.br/psyche-c/) that offers a glimpse of Psyche-C's type inference.
- Articles/blogs:
  - [Dumping a C program’s AST with Psyche-C](https://ltcmelo.github.io/psychec/2021/03/03/c-ast-dump-psyche.html)  
    (pt-BR) [Visualizando a AST de um programa C com o Psyche-C](https://www.embarcados.com.br/visualizando-a-ast-psyche-c/)
  - [Programming in C with type inference](https://www.codeproject.com/Articles/1238603/Programming-in-C-with-Type-Inference)  
    (pt-BR) [Programando em C com inferência de tipos usando PsycheC](https://www.embarcados.com.br/inferencia-de-tipos-em-c-usando-psychec/)


## Building and Testing

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
