#!/bin/bash

BASE_DIR="solver/test/cases/"
CNT=1
MAX=10

while [[ $CNT -le 141 ]]; do
    CUR="${BASE_DIR}T${CNT}.c"
    echo $CUR
    ./Reconstruct.py $CUR
    clang -c "${BASE_DIR}T${CNT}.fixed.c"
    if [[ $? -ne 0 ]]; then
        echo "***** ERROR *****"
    fi
    let CNT=$((CNT+1))
done

echo "Bye"
