#!/usr/bin/env sh

set -u


for C_FILE in solver/test/cases/*.c; do
    rm -f "${C_FILE%.c}".o \
          "${C_FILE%.c}".cnip \
          "${C_FILE%.c}".cstr \
          "${C_FILE%.c}".inc
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

SKIP_plain=(
    'T118.c'
)

SKIP_stdlib=(
    'T118.c'
)

EXPECT_ERROR_plain=(
    'T211.c'
)

EXPECT_ERROR_stdilb=(
    'T211.c'
)

KNOWN_ISSUE_plain=("T165.c" "T229.c")

KNOWN_ISSUE_stdlib=(
)


is_element_of()
{
    for EL in $2; do
        if [ $1 = "solver/test/cases/$EL" ]; then
            return 0
        fi
    done
    return 1
}

infer_and_compile()
{
    CNIPPET_FLAGS=$1
    SKIP=$2
    EXPECT_ERROR=$3
    KNOWN_ISSUE=$4

    printf "$C_FILE\n"

    if is_element_of $C_FILE $SKIP; then
        printf '\tSKIPPED\n'
        return
    fi

    ./cnip.sh -f $CNIPPET_FLAGS $CC $CC_FLAGS $C_FILE -o "${C_FILE%.c}".o

    OK=$?
    if [ $OK -ne 0 ]; then
        printf "\n ^^^^^\n "
        if is_element_of $C_FILE $EXPECT_ERROR; then
            printf "EXPECTED\n\n"
        elif is_element_of $C_FILE $KNOWN_ISSUE; then
            printf "KNOWN ISSUE\n\n"
        else
            printf "FAIL, FAIL, FAIL $CNIPPET_FLAGS ($C_FILE)\n"
            exit 1
        fi
    fi
}

printf "ignoring stdblib...\n"
for C_FILE in solver/test/cases/*.c ; do
    infer_and_compile --no-stdlib \
                      ${SKIP_plain[@]} \
                      ${EXPECT_ERROR_plain[@]} \
                      ${KNOWN_ISSUE_plain[@]}
done

printf "\nrecognizing stdlib...\n"
for C_FILE in solver/test/cases/*.c ; do
    infer_and_compile "" \
                      "${SKIP_stdlib[@]}" \
                      "${EXPECT_ERROR_stdlib[@]}" \
                      "${KNOWN_ISSUE_stdlib[@]}"
done

print 'done'
