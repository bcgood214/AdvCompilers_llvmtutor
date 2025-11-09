; ModuleID = 'input_for_loopinv.c'
source_filename = "input_for_loopinv.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %c = alloca i32, align 4
  %i = alloca i32, align 4
  store i32 5, ptr %a, align 4
  store i32 10, ptr %b, align 4
  store i32 0, ptr %c, align 4
  store i32 0, ptr %i, align 4
  br label %loop

loop:                                             ; preds = %loop, %entry
  %i_val = load i32, ptr %i, align 4
  %a_val = load i32, ptr %a, align 4
  %b_val = load i32, ptr %b, align 4
  %mul = mul i32 %a_val, %b_val             ; invariant (a*b)
  %twice = mul i32 %i_val, 2                ; variant (depends on i)
  %add = add i32 %mul, %twice               ; variant (depends on i)
  %c_val = load i32, ptr %c, align 4
  %c_next = add i32 %c_val, %add
  store i32 %c_next, ptr %c, align 4
  %i_next = add i32 %i_val, 1
  store i32 %i_next, ptr %i, align 4
  %cond = icmp slt i32 %i_next, 10
  br i1 %cond, label %loop, label %exit

exit:                                             ; preds = %loop
  %res = load i32, ptr %c, align 4
  ret i32 %res
}

