//=============================================================================
// FILE:
//      input_for_hello.c
//
// DESCRIPTION:
//      Sample input file for HelloWorld and InjectFuncCall
//
// License: MIT
//=============================================================================

int main(int argc, char *argv[]) {
  int factor = 4;
  int size = 10;
  int arry[10];

  for (int i = 0; i < 10; i++) {
    int product = factor * 2 + i;
    arry[i] = product;

  }

  return 0;
}
