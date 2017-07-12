#!/bin/bash

if [ $# -eq 0 ]; then
    echo "Test case number?"
    exit 1
fi

cp test.c solver/test/cases/T"$1".c
touch solver/test/cases/T"$1".c.expected
git add solver/test/cases/T"$1".c
git add solver/test/cases/T"$1".c.expected

echo "Files created and added to git"
