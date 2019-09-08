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

### Running the Tests

    ./psychecgen -t
    ./CompileTests.sh
    cd solver && stack test

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
typedef struct TYPE_1__
{
    int value;
    struct TYPE_1__* next;
}* T;
```

## Features

Besides type inference, PsycheC supports the following features.

### Generic Programming

*This is a work-in-progress, feedback is welcome through [this form](https://forms.gle/oJj1YEhAk3jwvHRo8).*

PsycheC provides an alternative to `void*` and `#macros` for writing generic code in C.
This is how you would implement a generic linked-list's `prepend` function.

```c
_Generic void prepend(_Forall(node_t)** head,
                      _Forall(value_t) value)
{
    node_t* n = malloc(sizeof(node_t));
    n->value = value;
    n->next = *head;
    *head = n;
}
```

It is not necessary to define neither `node_t` nor `value_t`, they will be inferred.
This definition if `prepend` applies "for all" kinds of nodes and values.
This way, you can focus on the *algorithms* - the essence of generic programming.

Let us create 2 lists, one of `int`'s and another of `point_t`s, and insert an element as their head.

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

Now, PsycheC understands that there "exists" a (node) type whose value is an `int`,
and a specialization of `prepend` for such arguments.
Similarly, a different (node) type for `point_t` "exists" too, with a corresponding
specialization of `prepend`.

Check the examples directory for this snippet.


## Publications

PsycheC is an ongoing research project.
It has been presented at:

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
