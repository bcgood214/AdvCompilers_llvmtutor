; tiny.ll
define void @main() {
entry:
  br label %loop

loop:
  %i = phi i32 [ 0, %entry ], [ %inc, %loop ]
  %cmp = icmp slt i32 %i, 10
  br i1 %cmp, label %body, label %exit

body:
  %inc = add i32 %i, 1
  br label %loop

exit:
  ret void
}
