#!/bin/sh

set -e
set -u
set -x

git clone --depth 1 --branch v1.6.44 git@github.com:pnggroup/libpng.git
cd libpng

./configure
make -j 5

cnip() {
    ../../../cnip --C-pp-includes --WIP "$@"
}

echo "\n\n\n******"

cnip png.c
cnip pngerror.c
cnip pngget.c
cnip pngmem.c
cnip pngpread.c
cnip pngread.c
cnip pngrio.c
cnip pngrtran.c
cnip pngrutil.c
cnip pngset.c
cnip pngtest.c
cnip pngtrans.c
cnip pngwio.c
cnip pngwrite.c
cnip pngwtran.c
cnip pngwutil.c

cd ..
rm -rf libpng
echo "libpng ok"
