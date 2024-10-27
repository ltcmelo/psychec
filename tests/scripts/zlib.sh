#!/bin/sh

set -e
set -u
set -x

git clone --depth 1 --branch v1.3.1 git@github.com:madler/zlib.git
cd zlib

./configure
make -j 5

cnip() {
    ../../../cnip --C-pp-includes --WIP "$@"
}

echo "\n\n\n******"

cnip adler32.c
cnip crc32.c
cnip deflate.c
cnip infback.c
cnip inffast.c
cnip inflate.c
cnip inftrees.c
cnip trees.c
cnip zutil.c
cnip compress.c
cnip uncompr.c
cnip gzclose.c
cnip gzlib.c
cnip gzread.c
cnip gzwrite.c

cd ..
rm -rf zlib
echo "zlib ok"
