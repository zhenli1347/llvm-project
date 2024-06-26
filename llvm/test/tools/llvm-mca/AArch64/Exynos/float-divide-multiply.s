# NOTE: Assertions have been autogenerated by utils/update_mca_test_checks.py
# RUN: llvm-mca -march=aarch64 -mcpu=exynos-m3 -resource-pressure=false -skip-unsupported-instructions=parse-failure < %s | FileCheck %s -check-prefixes=ALL,EM3
# RUN: llvm-mca -march=aarch64 -mcpu=exynos-m4 -resource-pressure=false < %s | FileCheck %s -check-prefixes=ALL,EM4
# RUN: llvm-mca -march=aarch64 -mcpu=exynos-m5 -resource-pressure=false < %s | FileCheck %s -check-prefixes=ALL,EM5

fdiv	h0, h1, h2
fdiv	s1, s2, s3
fdiv	d2, d3, d4

fmul	h3, h4, h5
fmul	s4, s5, s6
fmul	d5, d6, d7

fmadd	h6, h7, h8, h9
fmadd	s7, s8, s9, s10
fmadd	d8, d9, d10, d11

fsqrt	h9, h10
fsqrt	s10, s11
fsqrt	d11, d12

# ALL:      Iterations:        100

# EM3-NEXT: Instructions:      800
# EM3-NEXT: Total Cycles:      4503
# EM3-NEXT: Total uOps:        800

# EM4-NEXT: Instructions:      1200
# EM4-NEXT: Total Cycles:      572
# EM4-NEXT: Total uOps:        1200

# EM5-NEXT: Instructions:      1200
# EM5-NEXT: Total Cycles:      434
# EM5-NEXT: Total uOps:        1200

# ALL:      Dispatch Width:    6

# EM3-NEXT: uOps Per Cycle:    0.18
# EM3-NEXT: IPC:               0.18
# EM3-NEXT: Block RThroughput: 45.0

# EM4-NEXT: uOps Per Cycle:    2.10
# EM4-NEXT: IPC:               2.10
# EM4-NEXT: Block RThroughput: 4.0

# EM5-NEXT: uOps Per Cycle:    2.76
# EM5-NEXT: IPC:               2.76
# EM5-NEXT: Block RThroughput: 4.0

# ALL:      Instruction Info:
# ALL-NEXT: [1]: #uOps
# ALL-NEXT: [2]: Latency
# ALL-NEXT: [3]: RThroughput
# ALL-NEXT: [4]: MayLoad
# ALL-NEXT: [5]: MayStore
# ALL-NEXT: [6]: HasSideEffects (U)

# EM3:      [1]    [2]    [3]    [4]    [5]    [6]    Instructions:
# EM3-NEXT:  1      7     2.00                        fdiv	s1, s2, s3
# EM3-NEXT:  1      12    3.25                        fdiv	d2, d3, d4
# EM3-NEXT:  1      3     0.33                        fmul	s4, s5, s6
# EM3-NEXT:  1      3     0.33                        fmul	d5, d6, d7
# EM3-NEXT:  1      4     0.33                        fmadd	s7, s8, s9, s10
# EM3-NEXT:  1      4     0.33                        fmadd	d8, d9, d10, d11
# EM3-NEXT:  1      18    19.00                       fsqrt	s10, s11
# EM3-NEXT:  1      25    26.00                       fsqrt	d11, d12

# EM4:      [1]    [2]    [3]    [4]    [5]    [6]    Instructions:
# EM4-NEXT:  1      7     3.00                        fdiv	h0, h1, h2
# EM4-NEXT:  1      7     1.50                        fdiv	s1, s2, s3
# EM4-NEXT:  1      12    2.25                        fdiv	d2, d3, d4
# EM4-NEXT:  1      3     0.50                        fmul	h3, h4, h5
# EM4-NEXT:  1      3     0.33                        fmul	s4, s5, s6
# EM4-NEXT:  1      3     0.33                        fmul	d5, d6, d7
# EM4-NEXT:  1      4     0.50                        fmadd	h6, h7, h8, h9
# EM4-NEXT:  1      4     0.33                        fmadd	s7, s8, s9, s10
# EM4-NEXT:  1      4     0.33                        fmadd	d8, d9, d10, d11
# EM4-NEXT:  1      7     3.00                        fsqrt	h9, h10
# EM4-NEXT:  1      8     1.75                        fsqrt	s10, s11
# EM4-NEXT:  1      12    2.25                        fsqrt	d11, d12

# EM5:      [1]    [2]    [3]    [4]    [5]    [6]    Instructions:
# EM5-NEXT:  1      5     0.50                        fdiv	h0, h1, h2
# EM5-NEXT:  1      7     1.00                        fdiv	s1, s2, s3
# EM5-NEXT:  1      12    2.25                        fdiv	d2, d3, d4
# EM5-NEXT:  1      3     0.33                        fmul	h3, h4, h5
# EM5-NEXT:  1      3     0.33                        fmul	s4, s5, s6
# EM5-NEXT:  1      3     0.33                        fmul	d5, d6, d7
# EM5-NEXT:  1      4     0.33                        fmadd	h6, h7, h8, h9
# EM5-NEXT:  1      4     0.33                        fmadd	s7, s8, s9, s10
# EM5-NEXT:  1      4     0.33                        fmadd	d8, d9, d10, d11
# EM5-NEXT:  1      5     0.50                        fsqrt	h9, h10
# EM5-NEXT:  1      8     1.25                        fsqrt	s10, s11
# EM5-NEXT:  1      12    2.25                        fsqrt	d11, d12
