#!/bin/bash

CC=gcc

for FIXED_FILE in new_*; do
    rm "$FIXED_FILE"
done

echo "building..."
ghc -prof -fprof-auto -fprof-cafs -o runner muC.hs
OK=$?
if [ $OK -ne 0 ]; then
    echo "error in Haskell"
    exit 1
fi

FAIL_EXPECT=('new_test024.c'
             'new_test025.c'
             'new_test034.c'
             'new_test036.c'
             'new_test072.c'
             'new_test087.c'
             'new_test092.c'
             'new_test096.c'
             'new_test104.c'
             'new_test105.c'
             'new_test106.c')

for C_FILE in *.c ; do
    echo "  "

    echo "type inference for ${C_FILE}"
    ./runner "$C_FILE" &> /dev/null

    FIXED_FILE="new_${C_FILE}"

    for CUR in "${FAIL_EXPECT[@]}"; do
        if [ $FIXED_FILE = $CUR ]; then
            echo "  !!!!!  EXPECT ERROR !!!!!"
        fi
    done

    echo "compile ${FIXED_FILE}"
    "$CC"\
        -Wall\
        -Werror\
        -Wconversion\
        -Wfloat-conversion\
        -Wno-unused-but-set-variable\
        -Wno-incompatible-library-redeclaration\
        -Wno-uninitialized\
        -Wno-unused-variable\
        -Wno-unused-function\
        -Wno-switch\
        -Wno-unused-value\
        -Wno-implicit-int\
        -Wno-return-type\
        -Wno-builtin-requires-header\
        -Wno-infinite-recursion\
        -c "$FIXED_FILE"  #  &> /dev/null
    OK=$?
    if [ $OK -ne 0 ]; then
        printf "  <<error in $FIXED_FILE>>\n"
    fi
done
