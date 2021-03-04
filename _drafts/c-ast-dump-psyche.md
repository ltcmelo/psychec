# Dumping a C program's AST with Psyche-C

Given that the Psyche-C compiler frontend is being renewed, I decided to start a blog about the project too. This is my first post.

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

To produce and dump its AST with Psyche-C, we invoke the *cnippet* driver adaptor with command line option `--C-dump-AST`. The output is as follows:

<img width="609" alt="Screen Shot 2021-03-03 at 20 56 16" src="https://user-images.githubusercontent.com/2905588/109888794-e8d75f00-7c62-11eb-98bc-c62247df0c3e.png">

Now, let's look at the AST that Clang produces for that same program:

<img width="789" alt="Screen Shot 2021-03-03 at 21 03 44" src="https://user-images.githubusercontent.com/2905588/109889407-17096e80-7c64-11eb-9ecd-b0bd56af902e.png">


Apart from the colors and extra decoration, what are the main differences between the AST produce by Clang's and that by Psyche-C?

- Psyche-C's AST is a bit closer to the grammar of C. For instance, you can't see *declarator*s in Clang's AST — they are bundled within the *declaration*s.

Whether or not offering, through the AST, detailed insight about a program's syntax is good or bad will depend on a given application. I believe that, for the purpose of static analysis, the AST from Psyche-C has an advantage in this regard.

Does it mean that every *node* in a program's AST should correspond to a *terminal* of the grammar?

No... otherwise, we'd end up with a rather **concrete** syntax/parse tree, instead of an **abstract** syntax tree. The "best" AST will encapsulate just enough language semantics to make it comprehensible. (The AST produced by the [old parser of Psyche-C](https://github.com/ltcmelo/psychec/tree/original) was too grammar-oriented, but with little language meaning.)

Consider this snippet from the program above:

```c
int v, f(int d);
```

In Clang's AST, it's represented through `VarDecl` and a `FunctionDecl`; these 2 nodes [inherit from](https://clang.llvm.org/doxygen/classclang_1_1DeclaratorDecl.html) `DeclaratorDecl`, which is a *declaration* with a *declarator*. But, in pedantic terms, that isn't quite correct, given a single *declaration* — starting at `int` and ending at `;` — exists in that snippet.

In Psyche-C, I opted for a more rigorous representation in the AST: 

- A `VariableAndOrFunctionDeclaration` is created (just like Clang, this node [inherits from](https://ltcmelo.github.io/psychec/api-docs/html/classpsy_1_1_c_1_1_variable_and_or_function_declaration_syntax.html) a `DeclaratorDeclaration`), but with 2 child nodes: an `IdentifierDeclarator` that designated an object of type `int`, and a `FunctionDeclarator` that designates a function whose return type is `int`.

The design "spirit" of Psyche-C's AST is more aligned with that of [Roslyn](https://github.com/dotnet/roslyn), the .NET compiler platform. To wrap this up, I'll leave a C# version of our program, together with its AST (thanks to [SharpLab](https://sharplab.io/).

<img width="830" alt="Screen Shot 2021-03-03 at 22 01 44" src="https://user-images.githubusercontent.com/2905588/109894341-0bba4100-7c6c-11eb-983a-d716cb6c5960.png">






