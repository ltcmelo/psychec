# PsycheC

PsycheC is a Hindley/Milner unification-based type inference engine for C. You can try it online [here](http://cuda.dcc.ufmg.br/psyche-c/).

## Prerequisites

* Cmake
* C++14 compiler
* Haskell Stack
* Python

## Installing and running

    cmake CMakeLists.txt  
    make  
    cd solver  
    stack setup  
    stack build  
    cd ..  
    python Reconstruct.py path/to/file.c

## How it works

The science behind our type inference is explained in the following paper:

- *Inference of Static Semantics for Incomplete C Programs*

This paper has been accepted for publication in [POPL 2018](https://popl18.sigplan.org).


## About

This is a research project. While we strive to keep the code clean and to make the tool easily accessible, this is an academic effort. Neverless, feel free to provide feedback or to report bugs.

For a wrapper around PsycheC that integrates with your host C compiler, check [Cnippet](http://www.cnippet.cc/).
