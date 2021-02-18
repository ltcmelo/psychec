![](https://github.com/ltcmelo/psychec/workflows/generator-build/badge.svg)
![](https://github.com/ltcmelo/psychec/workflows/solver-build/badge.svg)
![](https://github.com/ltcmelo/psychec/workflows/parser-tests/badge.svg)
![](https://github.com/ltcmelo/psychec/workflows/inference-tests/badge.svg)
![](https://github.com/ltcmelo/psychec/workflows/compilability-tests/badge.svg)

# NOTE
## The Psyche project is going through a major overhaul in a private branch which will (hopefully) be merged soon.

# PsycheC

PsycheC is a compiler frontend infrastructure for the C language that is enabled with an ML/Haskell-style (unification-based) type inference engine.
[This online interface](http://cuda.dcc.ufmg.br/psyche-c/) illustrates the essential functionality of PsycheC.

Applications:

- Enabling static analysis on partial programs.
- Supporting semantic tools despite of `#include` failures.
- Compiling a code snippet (e.g., retrieved from a bug tracker).
- Generating test-input data for a function, individually.
- Prototyping of an algorithm without a type specification.

Be creative!


## Requirements

To *build* PsycheC:

* Cmake
* C++14 compiler
* Haskell Stack

To *use* PsycheC (download [here](http://www.cnippet.cc/)):

* Python 3


## Building

    cmake CMakeLists.txt  
    make


## Setting Up

    export PATH=$PATH:/path/to/psychec


## Using

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
typedef struct TYPE_1__
{
    int value;
    struct TYPE_1__* next;
}* T;
```

### Generic Programming

*This is a work-in-progress, feedback is welcome through [this form](https://forms.gle/oJj1YEhAk3jwvHRo8).*

PsycheC provides an alternative to `void*` and `#macros` for writing generic code in C.
This is how you would implement a generic linked-list's `prepend` function.

```c
_Template void prepend(_Forall(node_t)** head,
                      _Forall(value_t) value)
{
    node_t* n = malloc(sizeof(node_t));
    n->value = value;
    n->next = *head;
    *head = n;
}
```

It is not necessary to define neither `node_t` nor `value_t`.
The definition of `prepend` applies "for all" kinds of nodes and values.
This way, you can focus on the *algorithms* (the essence of generic programming).

Let us create 2 lists, one of `int` and another of `point_t`, and insert an new head to them.

```c
int main()
{
    _Exists(node_t)* ilst = 0;
    prepend(&ilst, 42);

    _Exists(node_t)* plst = 0;
    struct point_t p;
    p.x = p.y = 42;
    prepend(&plst, p);
}

```

Now, PsycheC infers that there "exists" a (node) type whose value is an `int`,
together with an specialization of `prepend` for such arguments.
A different (node) type for `point_t` "exists" too, with its corresponding specialization of `prepend`.

Check the examples directory for [this snippet](https://github.com/ltcmelo/psychec/blob/master/examples/generic_list.c).


## Running the Tests

    ./psychecgen -t
    ./test_infer_and_compile.sh
    cd solver && stack test && cd -
    cd formalism && ./test_muC.sh

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
