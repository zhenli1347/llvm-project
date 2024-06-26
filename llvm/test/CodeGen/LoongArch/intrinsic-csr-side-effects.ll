; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py
; RUN: llc --mtriple=loongarch32 -mattr=+d < %s | FileCheck %s
; RUN: llc --mtriple=loongarch64 -mattr=+d < %s | FileCheck %s

declare i32 @llvm.loongarch.csrrd.w(i32 immarg) nounwind
declare i32 @llvm.loongarch.csrwr.w(i32, i32 immarg) nounwind
declare void @bug()

define dso_local void @foo(i32 noundef signext %flag) nounwind {
; CHECK-LABEL: foo:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    beqz $a0, .LBB0_2
; CHECK-NEXT:  # %bb.1: # %if.then
; CHECK-NEXT:    csrrd $a0, 2
; CHECK-NEXT:    ori $a0, $a0, 1
; CHECK-NEXT:    csrwr $a0, 2
; CHECK-NEXT:  .LBB0_2: # %if.end
; CHECK-NEXT:    csrrd $a0, 2
; CHECK-NEXT:    andi $a0, $a0, 1
; CHECK-NEXT:    bnez $a0, .LBB0_4
; CHECK-NEXT:  # %bb.3: # %if.then2
; CHECK-NEXT:    b %plt(bug)
; CHECK-NEXT:  .LBB0_4: # %if.end3
; CHECK-NEXT:    ret
entry:
  %tobool.not = icmp eq i32 %flag, 0
  br i1 %tobool.not, label %if.end, label %if.then

if.then:                                          ; preds = %entry
  %0 = tail call i32 @llvm.loongarch.csrrd.w(i32 2)
  %or = or i32 %0, 1
  %1 = tail call i32 @llvm.loongarch.csrwr.w(i32 %or, i32 2)
  br label %if.end

if.end:                                           ; preds = %if.then, %entry
  %2 = tail call i32 @llvm.loongarch.csrrd.w(i32 2)
  %and = and i32 %2, 1
  %tobool1.not = icmp eq i32 %and, 0
  br i1 %tobool1.not, label %if.then2, label %if.end3

if.then2:                                         ; preds = %if.end
  tail call void @bug()
  br label %if.end3

if.end3:                                          ; preds = %if.then2, %if.end
  ret void
}
