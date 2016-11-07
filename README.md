# Psyche-c

Psyche-c is a compiler for incomplete C code. It features a Hindley/Milner-inspired type inference engine for C. You can try it online [here](http://cuda.dcc.ufmg.br/psyche-c/).

## Prerequisites

* Cmake
* C++14 compiler
* Haskell Stack
* Python

## Installing and running

$cmake CMakeLists.txt  
$make  
$cd ..  
$cd solver  
$stack setup  
$stack build  
$cd ..  
$python Reconstruct path/to/file.c  

## About

This is a research project. While we strive to keep the code clean and to make the tool easily accessible, this is an academic effort. Neverless, feel free to provide feedback or to report bugs.
