//=============================================================================
// FILE:
//      input_for_hello.c
//
// DESCRIPTION:
//      Sample input file for HelloWorld and InjectFuncCall
//
// License: MIT
//=============================================================================

int main(int argc, char **argv) {
    int a = argc;       // runtime value, not constant
    int b = 10;         // constant is fine here
    int c = 0;

    for (int i = 0; i < 10; i++) {
        int x = a * b;  // loop-invariant, not folded
        int y = i * 2;  // variant
        c += x + y;     // not invariant
    }

    return c;
}

