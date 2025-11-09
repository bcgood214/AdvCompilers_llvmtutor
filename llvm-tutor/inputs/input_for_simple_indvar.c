//=============================================================================
// FILE:
//      input_for_hello.c
//
// DESCRIPTION:
//      Sample input file for HelloWorld and InjectFuncCall
//
// License: MIT
//=============================================================================

// inputs/input_for_simple_indvar.c
#include <stdio.h>

int main() {
    int sum = 0;
    int i = 0;               // declare outside the loop
    for (; i < 10; i++) {
        sum += i;
    }
    printf("%d\n", sum);
    return 0;
}



