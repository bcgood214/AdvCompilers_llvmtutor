This is the repository for llvm-tutor, starting from assignment #1 in the Fall 2025 semester.

Commands for assignment 1:

Compiling input_for_nested_derive.c:
- run the command (from llvm-tutor): clang -O0 -Xclang -disable-O0-optnone -emit-llvm -c inputs/input_for_nested_derived.c -o input_for_nested_derived.bc

Testing induction variable elimination:
- run the command: opt -passes="mem2reg,loop-simplify,lcssa,indvars,induction-var-elim"     -load-pass-plugin ./build/lib/libInductionVarElim.so     -S input_for_nested_derived.bc -o eliminated.ll
- Relevant output will be found in eliminated.ll

Testing LICM:
- run the command: opt -load-pass-plugin ./build/lib/libSimpleLICM.so -passes="simple-licm" -disable-output simplified.bc

ChatGPT was used for advice, guidance, and generation of test inputs.
