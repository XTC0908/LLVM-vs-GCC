; ModuleID = '1.c'
source_filename = "1.c"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.13.0"

; Function Attrs: norecurse nounwind ssp uwtable
define void @arymul([5 x i32]* nocapture readonly, [3 x i32]* nocapture readonly, [3 x i32]* nocapture) local_unnamed_addr #0 {
  br label %4

; <label>:4:                                      ; preds = %23, %3
  %5 = phi i64 [ 0, %3 ], [ %24, %23 ]
  br label %6

; <label>:6:                                      ; preds = %19, %4
  %7 = phi i64 [ 0, %4 ], [ %21, %19 ]
  br label %8

; <label>:8:                                      ; preds = %8, %6
  %9 = phi i64 [ 0, %6 ], [ %17, %8 ]
  %10 = phi i32 [ 0, %6 ], [ %16, %8 ]
  %11 = getelementptr inbounds [5 x i32], [5 x i32]* %0, i64 %5, i64 %9
  %12 = load i32, i32* %11, align 4, !tbaa !3
  %13 = getelementptr inbounds [3 x i32], [3 x i32]* %1, i64 %9, i64 %7
  %14 = load i32, i32* %13, align 4, !tbaa !3
  %15 = mul nsw i32 %14, %12
  %16 = add nsw i32 %15, %10
  %17 = add nuw nsw i64 %9, 1
  %18 = icmp eq i64 %17, 5
  br i1 %18, label %19, label %8

; <label>:19:                                     ; preds = %8
  %20 = getelementptr inbounds [3 x i32], [3 x i32]* %2, i64 %5, i64 %7
  store i32 %16, i32* %20, align 4, !tbaa !3
  %21 = add nuw nsw i64 %7, 1
  %22 = icmp eq i64 %21, 3
  br i1 %22, label %23, label %6

; <label>:23:                                     ; preds = %19
  %24 = add nuw nsw i64 %5, 1
  %25 = icmp eq i64 %24, 4
  br i1 %25, label %26, label %4

; <label>:26:                                     ; preds = %23
  ret void
}

attributes #0 = { norecurse nounwind ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{!"Apple LLVM version 9.1.0 (clang-902.0.39.2)"}
!3 = !{!4, !4, i64 0}
!4 = !{!"int", !5, i64 0}
!5 = !{!"omnipotent char", !6, i64 0}
!6 = !{!"Simple C/C++ TBAA"}
