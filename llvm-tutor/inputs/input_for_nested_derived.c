//=============================================================================
// FILE:
//      input_for_hello.c
//
// DESCRIPTION:
//      Sample input file for HelloWorld and InjectFuncCall
//
// License: MIT
//=============================================================================

// inputs/input_for_nested_derived.c
// Test case generated with ChatGPT.
// In general, ChatGPT was used for guidance
// and code was only copied/pasted for the test inputs.
#include <stdio.h>

int main() {
    int result = 0;

    // Outer loop: i is a basic induction variable
    for (int i = 0; i < 3; i++) {

        // Inner loop: j is derived from i
        for (int j = 2 * i; j < 2 * i + 5; j++) {
            result += i + j;
        }
    }

    printf("%d\n", result);
    return 0;
}




