#!/bin/sh

set -e
set -u
set -x

git clone --depth 1 --branch v5.4.7 git@github.com:lua/lua.git
cd lua

make -j 5 MYLDFLAGS='$(LOCAL) -Wl,-export_dynamic'

cnip() {
    ../../../cnip --C-pp-includes --WIP "$@"
}

echo "\n\n\n******"

cnip lapi.c
cnip lcode.c
cnip lctype.c
cnip ldebug.c
cnip ldo.c
cnip ldump.c
cnip lfunc.c
cnip lgc.c
cnip llex.c
cnip lmem.c
cnip lobject.c
cnip lparser.c
cnip lstate.c
cnip lstring.c
cnip ltable.c
cnip ltm.c
cnip lundump.c
cnip lvm.c
cnip lzio.c
cnip ltests.c
cnip lauxlib.c
cnip lbaselib.c
cnip ldblib.c
cnip liolib.c
cnip lmathlib.c
cnip loslib.c
cnip ltablib.c
cnip lstrlib.c
cnip lutf8lib.c
cnip loadlib.c
cnip lcorolib.c
cnip linit.c

cd ..
rm -rf lua
echo "lua ok"
