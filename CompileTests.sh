#!/bin/bash

CC=clang

for FIXED_FILE in solver/test/cases/*_fixed.c; do
    rm "$FIXED_FILE"
done

for GEN_FILE in solver/test/cases/*_gen.h; do
    rm "$GEN_FILE"
done

for C_FILE in solver/test/cases/*.c ; do
    FIXED_FILE=$(echo "$C_FILE" | sed -e 's/\.c/_fixed.c/g')
    echo "$C_FILE"

    ./Reconstruct.py "$C_FILE" # > /dev/null

    # Avoid warnings not related to typing.
    "$CC" -Wall\
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
	  -c "$FIXED_FILE" # &> /dev/null
    OK=$?
    if [ $OK -ne 0 ]; then
        printf "*****\nError compiling $FIXED_FILE\n*****\n"
    fi
done

echo "Done"
