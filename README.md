# PsycheC

PsycheC is a compiler frontend infrastructure for the C language that is enabled with an ML/Haskell-style (unification-based) type inference engine.
[This online interface](http://cuda.dcc.ufmg.br/psyche-c/) illustrates the essential functionality of PsycheC.

## Requirements

* Cmake
* C++14 compiler
* Haskell Stack
* Python 3

## Building

    cmake CMakeLists.txt  
    make

## Cnippet

The simplest way to use PsycheC is through the [Cnippet](http://www.cnippet.cc) compiler adaptor.
Let us see an example.

Consider the file *node.c* below.

```c
// node.c
void f()
{
    T v = 0;
    v->value = 42;
    v->next = v;
}
```

If you were to compile *node.c* with GCC/Clang, you would issue a command similar to this one:

    gcc -c node.c

As a result, an object file *node.o* would have been produced.
However, this will not happen, since a declaration for `T` is not available.
Instead, you will get an "undeclared identifier" error.

Now, if you invoke Cnippet, the compilation succeeds (flag `-f` is for non-commercial use).

    ./cnip.sh -f gcc -c node.c

That is because, under the hood, PsycheC will infer the necessary missing definitions.

```c
struct T
{
    int value;
    struct T* next;
};
typedef struct T* T;
```

## Testing

Tests exist at varying layers. Currently, this is the best you can do.

    ./psychecgen -t
    ./CompileTests.sh
    cd solver && stack test && cd -

## Publications

PsycheC is an ongoing research project.
It has been introduced at:

- [Inference of static semantics for incomplete C programs](https://dl.acm.org/citation.cfm?id=3158117);
Proceedings of the ACM on Programming Languages, Volume 2 Issue **POPL** (Principles of Programming Languages),
January 2018, Article No. 29.

Below is a list of papers whose developed tooling rely on PsycheC:

- [Generation of in-bounds inputs for arrays in memory-unsafe languages](https://dl.acm.org/citation.cfm?id=3314890);
Proceedings of the 2019 IEEE/ACM International Symposium on **CGO** (Code Generation and Optimization),
February 2019, Pages 136-148.

- [Automatic annotation of tasks in structured code](https://dl.acm.org/citation.cfm?id=3243200);
Proceedings of the 27th International Conference on **PACT** (Parallel Architectures and Compilation Techniques),
November 2018, Article No. 31.

This is an online article about PsycheC:

- [Programming in C with Type Inference](https://www.codeproject.com/Articles/1238603/Programming-in-C-with-Type-Inference)
