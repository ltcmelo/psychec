#!/bin/bash

for FIXED_FILE in solver/test/cases/*_fixed.c; do
    rm "$FIXED_FILE"
done

for GEN_FILE in solver/test/cases/*_gen.h; do
    rm "$GEN_FILE"
done

CC=clang
CC_FLAGS="-Werror\
          -Wall\
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
          -c"

for C_FILE in solver/test/cases/*.c ; do
    printf "\n\n$C_FILE"

    ./cnip.sh -f --no-stdlib "$CC" "$CC_FLAGS" "$C_FILE" # > /dev/null

    OK=$?
    if [ $OK -ne 0 ]; then
        printf "*** ^^^\n*** Error in $C_FILE !!!\n\n"
    fi

    ./cnip.sh -f "$CC" "$CC_FLAGS" "$C_FILE" # > /dev/null

    OK=$?
    if [ $OK -ne 0 ]; then
        printf "*** ^^^\n*** Error in $C_FILE (with STDlib) !!!\n\n"
    fi
done

echo "Done"
