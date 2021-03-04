---
layout: post
title:  "astdump"
---

# Dumping a C program's AST with Psyche-C

Given that the Psyche-C compiler frontend is being renewed, I decided to start a blog about the project. This is my first post.

Without further ado, consider this C program (assume that its name is `test.c`):

```c
int v, f(int d);

int f(int p)
{
    if (p == v)
        return(p - 1);
    return 0;
}
```

To produce and dump an AST with Psyche-C, we invoke the *cnippet* driver adaptor with command line option `--C-dump-AST`. The output is as follows:

<img width="609" alt="Screen Shot 2021-03-03 at 20 56 16" src="https://user-images.githubusercontent.com/2905588/109888794-e8d75f00-7c62-11eb-98bc-c62247df0c3e.png">

Now, I'll briefly correlate the above AST with the one produced by [Clang](https://clang.llvm.org/) for the same `test.c` program. Let's take a look at it:

<img width="789" alt="Screen Shot 2021-03-03 at 21 03 44" src="https://user-images.githubusercontent.com/2905588/109889407-17096e80-7c64-11eb-9ecd-b0bd56af902e.png">

Apart from the colors and extra decoration, what are the main differences between the two?

- _It's that Psyche-C's AST is a bit closer to the grammar of C._ For instance, you can't see *declarator*s in Clang's AST, they are absorbed by their containing *declaration*s.

Whether or not this characteristic is good or bad will depend on a given application. For the purpose of static analysis implementation, I consider the refined syntax insights of Psyche-C's AST an advantage.

I'm not saying that every *node* in a AST should correspond to a *terminal* of the grammar. If so, we'd end up with a rather **concrete** syntax (or parse) tree, instead of an **abstract** syntax tree. A comprehensible AST will contain just enough of a language's meaning embedded into it. (The AST produced by the [old parser of Psyche-C](https://github.com/ltcmelo/psychec/tree/original), e.g., was too grammar-oriented.)

Consider this snippet from our `test.c` program:

```c
int v, f(int d);
```

In Clang's AST, it's represented through a `VarDecl` and a `FunctionDecl`; both these nodes [inherit from](https://clang.llvm.org/doxygen/classclang_1_1DeclaratorDecl.html) `DeclaratorDecl` (i.e., a *declaration* with a *declarator*). But, in pedantic terms, that representation isn't quite correct, given that a single *declaration* — one starting at `int` and ending at `;` — exists in that snippet.

In Psyche-C, I opted for a more rigorous representation in the AST: 

- A `VariableAndOrFunctionDeclaration` is created (just like Clang, this node [inherits from](https://ltcmelo.github.io/psychec/api-docs/html/classpsy_1_1_c_1_1_variable_and_or_function_declaration_syntax.html) a `DeclaratorDeclaration`), and has two child nodes: an `IdentifierDeclarator`, designating object of type `int`, and a `FunctionDeclarator`, designating a function whose return type is `int`.

In general, the design "spirit" of Psyche-C's AST is more aligned with that of [Roslyn](https://github.com/dotnet/roslyn), the .NET compiler platform. Although, because Psyche-C is a frontend for C, its AST will, of course, resembles that of Clang.

To wrap up, I'll leave a C# version of `test.c` (well, `test.cs`), together with its AST.

<img width="830" alt="Screen Shot 2021-03-03 at 22 01 44" src="https://user-images.githubusercontent.com/2905588/109894341-0bba4100-7c6c-11eb-983a-d716cb6c5960.png">

(Screenshot from [SharpLab](https://sharplab.io/))





