#!/bin/bash

CC=clang

for FIXED_FILE in solver/test/cases/*.fixed.c; do
    rm "$FIXED_FILE"
done

for C_FILE in solver/test/cases/*.c ; do
    ./Reconstruct.py "$C_FILE" > /dev/null

    FIXED_FILE=$(echo "$C_FILE" | sed -e 's/\.c/.fixed.c/g')
    "$CC" -c "$FIXED_FILE" &> /dev/null
    OK=$?
    if [ $OK -ne 0 ]; then
        echo "*** Error compiling $FIXED_FILE"
    else
        echo "."
    fi
done
