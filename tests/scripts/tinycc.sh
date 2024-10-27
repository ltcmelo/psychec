#!/bin/sh

set -e
set -u
set -x

git clone --depth 1 --branch release_0_9_27 git@github.com:TinyCC/tinycc.git
cd tinycc

./configure
make -j 5

cnip() {
    ../../../cnip --C-pp-includes --WIP "$@"
}

echo "\n\n\n******"

cnip tcc.c
cnip libtcc.c
cnip tccpp.c
cnip tccgen.c
cnip tccelf.c
cnip tccasm.c
cnip tccrun.c
cnip x86_64-gen.c
cnip x86_64-link.c
cnip i386-asm.c

cd ..
rm -rf tinycc
echo "tinycc ok"
