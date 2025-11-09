; ModuleID = 'simplified.bc'
source_filename = "inputs/input_for_loopinv.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main(i32 noundef %0, ptr noundef %1) #0 {
  %3 = mul nsw i32 %0, 10
  br label %4

4:                                                ; preds = %10, %2
  %.01 = phi i32 [ 0, %2 ], [ %9, %10 ]
  %.0 = phi i32 [ 0, %2 ], [ %11, %10 ]
  %5 = icmp slt i32 %.0, 10
  br i1 %5, label %6, label %12

6:                                                ; preds = %4
  %7 = mul nsw i32 %.0, 2
  %8 = add nsw i32 %3, %7
  %9 = add nsw i32 %.01, %8
  br label %10

10:                                               ; preds = %6
  %11 = add nsw i32 %.0, 1
  br label %4, !llvm.loop !6

12:                                               ; preds = %4
  %.01.lcssa = phi i32 [ %.01, %4 ]
  ret i32 %.01.lcssa
}

attributes #0 = { noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"Ubuntu clang version 22.0.0 (++20251005081829+c793782b03ab-1~exp1~20251005202002.1209)"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
