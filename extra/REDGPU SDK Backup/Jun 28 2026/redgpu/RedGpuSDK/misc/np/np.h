#pragma once

#define __NP_COUNTOF_CAT( a, b ) a b
#define __NP_COUNTOF_A( \
  a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74, a75, a76, a77, a78, a79, a80, a81, a82, a83, a84, a85, a86, a87, a88, a89, a90, a91, a92, a93, a94, a95, a96, a97, a98, a99, a100, a101, \
  n, ... ) n
#define __NP_COUNTOF(...) __NP_COUNTOF_CAT( __NP_COUNTOF_A, ( 0, ##__VA_ARGS__, \
  50, 0, 49, 0, 48, 0, 47, 0, 46, 0, 45, 0, 44, 0, 43, 0, 42, 0, 41, 0, 40, 0, 39, 0, 38, 0, 37, 0, 36, 0, 35, 0, 34, 0, 33, 0, 32, 0, 31, 0, 30, 0, 29, 0, 28, 0, 27, 0, 26, 0, 25, 0, 24, 0, 23, 0, 22, 0, 21, 0, 20, 0, 19, 0, 18, 0, 17, 0, 16, 0, 15, 0, 14, 0, 13, 0, 12, 0, 11, 0, 10, 0, 9, 0, 8, 0, 7, 0, 6, 0, 5, 0, 4, 0, 3, 0, 2, 0, 1, 0, 0, 0 ) )
#define __NP_VA_EXPAND(name, ...)        __NP_VA_EXPAND_JOIN(name, __NP_COUNTOF(__VA_ARGS__))(__VA_ARGS__)
#define __NP_VA_EXPAND_JOIN(name, count) __NP_VA_EXPAND_J0IN(name, count)
#define __NP_VA_EXPAND_J0IN(name, count) __NP_VA_EXPAND_J01N(name, count)
#define __NP_VA_EXPAND_J01N(name, count) name##count
#define np(...) __NP_VA_EXPAND(np, __VA_ARGS__)

#define __NPFP_COUNTOF_CAT( a, b ) a b
#define __NPFP_COUNTOF_A( \
  a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74, a75, a76, a77, a78, a79, a80, a81, a82, a83, a84, a85, a86, a87, a88, a89, a90, a91, a92, a93, a94, a95, a96, a97, a98, a99, a100, a101, a102, \
  n, ... ) n
#define __NPFP_COUNTOF(...) __NPFP_COUNTOF_CAT( __NPFP_COUNTOF_A, ( 0, ##__VA_ARGS__, \
  50, 0, 49, 0, 48, 0, 47, 0, 46, 0, 45, 0, 44, 0, 43, 0, 42, 0, 41, 0, 40, 0, 39, 0, 38, 0, 37, 0, 36, 0, 35, 0, 34, 0, 33, 0, 32, 0, 31, 0, 30, 0, 29, 0, 28, 0, 27, 0, 26, 0, 25, 0, 24, 0, 23, 0, 22, 0, 21, 0, 20, 0, 19, 0, 18, 0, 17, 0, 16, 0, 15, 0, 14, 0, 13, 0, 12, 0, 11, 0, 10, 0, 9, 0, 8, 0, 7, 0, 6, 0, 5, 0, 4, 0, 3, 0, 2, 0, 1, 0, 0, 0, 0 ) )
#define __NPFP_VA_EXPAND(name, ...)        __NPFP_VA_EXPAND_JOIN(name, __NPFP_COUNTOF(__VA_ARGS__))(__VA_ARGS__)
#define __NPFP_VA_EXPAND_JOIN(name, count) __NPFP_VA_EXPAND_J0IN(name, count)
#define __NPFP_VA_EXPAND_J0IN(name, count) __NPFP_VA_EXPAND_J01N(name, count)
#define __NPFP_VA_EXPAND_J01N(name, count) name##count
#define npfp(...) __NPFP_VA_EXPAND(npfp, __VA_ARGS__)

#ifdef REDGPU_DISABLE_NAMED_PARAMETERS

#define np0(ProcedureName) ProcedureName()

#define np1(ProcedureName, \
  p1_PassedName, p1 \
) \
  ProcedureName(p1)

#define np2(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2 \
) \
  ProcedureName(p1, p2)

#define np3(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3 \
) \
  ProcedureName(p1, p2, p3)

#define np4(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4 \
) \
  ProcedureName(p1, p2, p3, p4)

#define np5(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5 \
) \
  ProcedureName(p1, p2, p3, p4, p5)

#define np6(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6)

#define np7(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7)

#define np8(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8)

#define np9(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9)

#define np10(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define np11(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11)

#define np12(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12)

#define np13(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13)

#define np14(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14)

#define np15(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15)

#define np16(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16)

#define np17(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17)

#define np18(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18)

#define np19(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19)

#define np20(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20)

#define np21(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21)

#define np22(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22)

#define np23(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23)

#define np24(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24)

#define np25(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25)

#define np26(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26)

#define np27(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27)

#define np28(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28)

#define np29(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29)

#define np30(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30)

#define np31(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31)

#define np32(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32)

#define np33(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33)

#define np34(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34)

#define np35(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35)

#define np36(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36)

#define np37(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37)

#define np38(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38)

#define np39(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38, \
  p39_PassedName, p39 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39)

#define np40(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38, \
  p39_PassedName, p39, \
  p40_PassedName, p40 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40)

#define np41(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38, \
  p39_PassedName, p39, \
  p40_PassedName, p40, \
  p41_PassedName, p41 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41)

#define np42(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38, \
  p39_PassedName, p39, \
  p40_PassedName, p40, \
  p41_PassedName, p41, \
  p42_PassedName, p42 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42)

#define np43(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38, \
  p39_PassedName, p39, \
  p40_PassedName, p40, \
  p41_PassedName, p41, \
  p42_PassedName, p42, \
  p43_PassedName, p43 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43)

#define np44(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38, \
  p39_PassedName, p39, \
  p40_PassedName, p40, \
  p41_PassedName, p41, \
  p42_PassedName, p42, \
  p43_PassedName, p43, \
  p44_PassedName, p44 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44)

#define np45(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38, \
  p39_PassedName, p39, \
  p40_PassedName, p40, \
  p41_PassedName, p41, \
  p42_PassedName, p42, \
  p43_PassedName, p43, \
  p44_PassedName, p44, \
  p45_PassedName, p45 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45)

#define np46(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38, \
  p39_PassedName, p39, \
  p40_PassedName, p40, \
  p41_PassedName, p41, \
  p42_PassedName, p42, \
  p43_PassedName, p43, \
  p44_PassedName, p44, \
  p45_PassedName, p45, \
  p46_PassedName, p46 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46)

#define np47(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38, \
  p39_PassedName, p39, \
  p40_PassedName, p40, \
  p41_PassedName, p41, \
  p42_PassedName, p42, \
  p43_PassedName, p43, \
  p44_PassedName, p44, \
  p45_PassedName, p45, \
  p46_PassedName, p46, \
  p47_PassedName, p47 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47)

#define np48(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38, \
  p39_PassedName, p39, \
  p40_PassedName, p40, \
  p41_PassedName, p41, \
  p42_PassedName, p42, \
  p43_PassedName, p43, \
  p44_PassedName, p44, \
  p45_PassedName, p45, \
  p46_PassedName, p46, \
  p47_PassedName, p47, \
  p48_PassedName, p48 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48)

#define np49(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38, \
  p39_PassedName, p39, \
  p40_PassedName, p40, \
  p41_PassedName, p41, \
  p42_PassedName, p42, \
  p43_PassedName, p43, \
  p44_PassedName, p44, \
  p45_PassedName, p45, \
  p46_PassedName, p46, \
  p47_PassedName, p47, \
  p48_PassedName, p48, \
  p49_PassedName, p49 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49)

#define np50(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38, \
  p39_PassedName, p39, \
  p40_PassedName, p40, \
  p41_PassedName, p41, \
  p42_PassedName, p42, \
  p43_PassedName, p43, \
  p44_PassedName, p44, \
  p45_PassedName, p45, \
  p46_PassedName, p46, \
  p47_PassedName, p47, \
  p48_PassedName, p48, \
  p49_PassedName, p49, \
  p50_PassedName, p50 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50)

#define npfp0(ProcedureName, ProcedureCall) ProcedureCall()

#define npfp1(ProcedureName, ProcedureCall, \
  p1_PassedName, p1 \
) \
  ProcedureCall(p1)

#define npfp2(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2 \
) \
  ProcedureCall(p1, p2)

#define npfp3(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3 \
) \
  ProcedureCall(p1, p2, p3)

#define npfp4(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4 \
) \
  ProcedureCall(p1, p2, p3, p4)

#define npfp5(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5 \
) \
  ProcedureCall(p1, p2, p3, p4, p5)

#define npfp6(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6)

#define npfp7(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7)

#define npfp8(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8)

#define npfp9(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9)

#define npfp10(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define npfp11(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11)

#define npfp12(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12)

#define npfp13(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13)

#define npfp14(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14)

#define npfp15(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15)

#define npfp16(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16)

#define npfp17(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17)

#define npfp18(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18)

#define npfp19(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19)

#define npfp20(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20)

#define npfp21(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21)

#define npfp22(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22)

#define npfp23(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23)

#define npfp24(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24)

#define npfp25(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25)

#define npfp26(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26)

#define npfp27(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27)

#define npfp28(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28)

#define npfp29(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29)

#define npfp30(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30)

#define npfp31(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31)

#define npfp32(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32)

#define npfp33(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33)

#define npfp34(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34)

#define npfp35(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35)

#define npfp36(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36)

#define npfp37(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37)

#define npfp38(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38)

#define npfp39(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38, \
  p39_PassedName, p39 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39)

#define npfp40(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38, \
  p39_PassedName, p39, \
  p40_PassedName, p40 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40)

#define npfp41(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38, \
  p39_PassedName, p39, \
  p40_PassedName, p40, \
  p41_PassedName, p41 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41)

#define npfp42(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38, \
  p39_PassedName, p39, \
  p40_PassedName, p40, \
  p41_PassedName, p41, \
  p42_PassedName, p42 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42)

#define npfp43(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38, \
  p39_PassedName, p39, \
  p40_PassedName, p40, \
  p41_PassedName, p41, \
  p42_PassedName, p42, \
  p43_PassedName, p43 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43)

#define npfp44(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38, \
  p39_PassedName, p39, \
  p40_PassedName, p40, \
  p41_PassedName, p41, \
  p42_PassedName, p42, \
  p43_PassedName, p43, \
  p44_PassedName, p44 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44)

#define npfp45(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38, \
  p39_PassedName, p39, \
  p40_PassedName, p40, \
  p41_PassedName, p41, \
  p42_PassedName, p42, \
  p43_PassedName, p43, \
  p44_PassedName, p44, \
  p45_PassedName, p45 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45)

#define npfp46(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38, \
  p39_PassedName, p39, \
  p40_PassedName, p40, \
  p41_PassedName, p41, \
  p42_PassedName, p42, \
  p43_PassedName, p43, \
  p44_PassedName, p44, \
  p45_PassedName, p45, \
  p46_PassedName, p46 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46)

#define npfp47(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38, \
  p39_PassedName, p39, \
  p40_PassedName, p40, \
  p41_PassedName, p41, \
  p42_PassedName, p42, \
  p43_PassedName, p43, \
  p44_PassedName, p44, \
  p45_PassedName, p45, \
  p46_PassedName, p46, \
  p47_PassedName, p47 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47)

#define npfp48(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38, \
  p39_PassedName, p39, \
  p40_PassedName, p40, \
  p41_PassedName, p41, \
  p42_PassedName, p42, \
  p43_PassedName, p43, \
  p44_PassedName, p44, \
  p45_PassedName, p45, \
  p46_PassedName, p46, \
  p47_PassedName, p47, \
  p48_PassedName, p48 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48)

#define npfp49(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38, \
  p39_PassedName, p39, \
  p40_PassedName, p40, \
  p41_PassedName, p41, \
  p42_PassedName, p42, \
  p43_PassedName, p43, \
  p44_PassedName, p44, \
  p45_PassedName, p45, \
  p46_PassedName, p46, \
  p47_PassedName, p47, \
  p48_PassedName, p48, \
  p49_PassedName, p49 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49)

#define npfp50(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38, \
  p39_PassedName, p39, \
  p40_PassedName, p40, \
  p41_PassedName, p41, \
  p42_PassedName, p42, \
  p43_PassedName, p43, \
  p44_PassedName, p44, \
  p45_PassedName, p45, \
  p46_PassedName, p46, \
  p47_PassedName, p47, \
  p48_PassedName, p48, \
  p49_PassedName, p49, \
  p50_PassedName, p50 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50)

#else // #ifdef REDGPU_DISABLE_NAMED_PARAMETERS

#include <string_view> // NOTE(Constantine): std::string_view requires C++17.

#define NP_STRING(a) #a
#define NP_STRINGIFY(a) NP_STRING(a)

#define np0(ProcedureName) ProcedureName()

#define np1(ProcedureName, \
  p1_PassedName, p1 \
) \
  ProcedureName(p1); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)")

#define np2(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2 \
) \
  ProcedureName(p1, p2); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)")

#define np3(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3 \
) \
  ProcedureName(p1, p2, p3); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)")

#define np4(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4 \
) \
  ProcedureName(p1, p2, p3, p4); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)")

#define np5(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5 \
) \
  ProcedureName(p1, p2, p3, p4, p5); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)")

#define np6(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)")

#define np7(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)")

#define np8(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)")

#define np9(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)")

#define np10(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)")

#define np11(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)")

#define np12(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)")

#define np13(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)")

#define np14(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)")

#define np15(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)")

#define np16(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)")

#define np17(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)")

#define np18(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)")

#define np19(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)")

#define np20(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)")

#define np21(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)")

#define np22(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)")

#define np23(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)")

#define np24(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)")

#define np25(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)")

#define np26(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)"); \
  static_assert(std::string_view(p26_PassedName) == NP_STRINGIFY(_np26_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np26_##ProcedureName) ": ...), got: " #ProcedureName "(" p26_PassedName ": ...)")

#define np27(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)"); \
  static_assert(std::string_view(p26_PassedName) == NP_STRINGIFY(_np26_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np26_##ProcedureName) ": ...), got: " #ProcedureName "(" p26_PassedName ": ...)"); \
  static_assert(std::string_view(p27_PassedName) == NP_STRINGIFY(_np27_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np27_##ProcedureName) ": ...), got: " #ProcedureName "(" p27_PassedName ": ...)")

#define np28(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)"); \
  static_assert(std::string_view(p26_PassedName) == NP_STRINGIFY(_np26_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np26_##ProcedureName) ": ...), got: " #ProcedureName "(" p26_PassedName ": ...)"); \
  static_assert(std::string_view(p27_PassedName) == NP_STRINGIFY(_np27_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np27_##ProcedureName) ": ...), got: " #ProcedureName "(" p27_PassedName ": ...)"); \
  static_assert(std::string_view(p28_PassedName) == NP_STRINGIFY(_np28_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np28_##ProcedureName) ": ...), got: " #ProcedureName "(" p28_PassedName ": ...)")

#define np29(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)"); \
  static_assert(std::string_view(p26_PassedName) == NP_STRINGIFY(_np26_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np26_##ProcedureName) ": ...), got: " #ProcedureName "(" p26_PassedName ": ...)"); \
  static_assert(std::string_view(p27_PassedName) == NP_STRINGIFY(_np27_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np27_##ProcedureName) ": ...), got: " #ProcedureName "(" p27_PassedName ": ...)"); \
  static_assert(std::string_view(p28_PassedName) == NP_STRINGIFY(_np28_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np28_##ProcedureName) ": ...), got: " #ProcedureName "(" p28_PassedName ": ...)"); \
  static_assert(std::string_view(p29_PassedName) == NP_STRINGIFY(_np29_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np29_##ProcedureName) ": ...), got: " #ProcedureName "(" p29_PassedName ": ...)")

#define np30(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)"); \
  static_assert(std::string_view(p26_PassedName) == NP_STRINGIFY(_np26_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np26_##ProcedureName) ": ...), got: " #ProcedureName "(" p26_PassedName ": ...)"); \
  static_assert(std::string_view(p27_PassedName) == NP_STRINGIFY(_np27_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np27_##ProcedureName) ": ...), got: " #ProcedureName "(" p27_PassedName ": ...)"); \
  static_assert(std::string_view(p28_PassedName) == NP_STRINGIFY(_np28_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np28_##ProcedureName) ": ...), got: " #ProcedureName "(" p28_PassedName ": ...)"); \
  static_assert(std::string_view(p29_PassedName) == NP_STRINGIFY(_np29_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np29_##ProcedureName) ": ...), got: " #ProcedureName "(" p29_PassedName ": ...)"); \
  static_assert(std::string_view(p30_PassedName) == NP_STRINGIFY(_np30_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np30_##ProcedureName) ": ...), got: " #ProcedureName "(" p30_PassedName ": ...)")

#define np31(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)"); \
  static_assert(std::string_view(p26_PassedName) == NP_STRINGIFY(_np26_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np26_##ProcedureName) ": ...), got: " #ProcedureName "(" p26_PassedName ": ...)"); \
  static_assert(std::string_view(p27_PassedName) == NP_STRINGIFY(_np27_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np27_##ProcedureName) ": ...), got: " #ProcedureName "(" p27_PassedName ": ...)"); \
  static_assert(std::string_view(p28_PassedName) == NP_STRINGIFY(_np28_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np28_##ProcedureName) ": ...), got: " #ProcedureName "(" p28_PassedName ": ...)"); \
  static_assert(std::string_view(p29_PassedName) == NP_STRINGIFY(_np29_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np29_##ProcedureName) ": ...), got: " #ProcedureName "(" p29_PassedName ": ...)"); \
  static_assert(std::string_view(p30_PassedName) == NP_STRINGIFY(_np30_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np30_##ProcedureName) ": ...), got: " #ProcedureName "(" p30_PassedName ": ...)"); \
  static_assert(std::string_view(p31_PassedName) == NP_STRINGIFY(_np31_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np31_##ProcedureName) ": ...), got: " #ProcedureName "(" p31_PassedName ": ...)")

#define np32(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)"); \
  static_assert(std::string_view(p26_PassedName) == NP_STRINGIFY(_np26_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np26_##ProcedureName) ": ...), got: " #ProcedureName "(" p26_PassedName ": ...)"); \
  static_assert(std::string_view(p27_PassedName) == NP_STRINGIFY(_np27_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np27_##ProcedureName) ": ...), got: " #ProcedureName "(" p27_PassedName ": ...)"); \
  static_assert(std::string_view(p28_PassedName) == NP_STRINGIFY(_np28_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np28_##ProcedureName) ": ...), got: " #ProcedureName "(" p28_PassedName ": ...)"); \
  static_assert(std::string_view(p29_PassedName) == NP_STRINGIFY(_np29_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np29_##ProcedureName) ": ...), got: " #ProcedureName "(" p29_PassedName ": ...)"); \
  static_assert(std::string_view(p30_PassedName) == NP_STRINGIFY(_np30_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np30_##ProcedureName) ": ...), got: " #ProcedureName "(" p30_PassedName ": ...)"); \
  static_assert(std::string_view(p31_PassedName) == NP_STRINGIFY(_np31_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np31_##ProcedureName) ": ...), got: " #ProcedureName "(" p31_PassedName ": ...)"); \
  static_assert(std::string_view(p32_PassedName) == NP_STRINGIFY(_np32_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np32_##ProcedureName) ": ...), got: " #ProcedureName "(" p32_PassedName ": ...)")

#define np33(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)"); \
  static_assert(std::string_view(p26_PassedName) == NP_STRINGIFY(_np26_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np26_##ProcedureName) ": ...), got: " #ProcedureName "(" p26_PassedName ": ...)"); \
  static_assert(std::string_view(p27_PassedName) == NP_STRINGIFY(_np27_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np27_##ProcedureName) ": ...), got: " #ProcedureName "(" p27_PassedName ": ...)"); \
  static_assert(std::string_view(p28_PassedName) == NP_STRINGIFY(_np28_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np28_##ProcedureName) ": ...), got: " #ProcedureName "(" p28_PassedName ": ...)"); \
  static_assert(std::string_view(p29_PassedName) == NP_STRINGIFY(_np29_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np29_##ProcedureName) ": ...), got: " #ProcedureName "(" p29_PassedName ": ...)"); \
  static_assert(std::string_view(p30_PassedName) == NP_STRINGIFY(_np30_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np30_##ProcedureName) ": ...), got: " #ProcedureName "(" p30_PassedName ": ...)"); \
  static_assert(std::string_view(p31_PassedName) == NP_STRINGIFY(_np31_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np31_##ProcedureName) ": ...), got: " #ProcedureName "(" p31_PassedName ": ...)"); \
  static_assert(std::string_view(p32_PassedName) == NP_STRINGIFY(_np32_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np32_##ProcedureName) ": ...), got: " #ProcedureName "(" p32_PassedName ": ...)"); \
  static_assert(std::string_view(p33_PassedName) == NP_STRINGIFY(_np33_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np33_##ProcedureName) ": ...), got: " #ProcedureName "(" p33_PassedName ": ...)")

#define np34(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)"); \
  static_assert(std::string_view(p26_PassedName) == NP_STRINGIFY(_np26_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np26_##ProcedureName) ": ...), got: " #ProcedureName "(" p26_PassedName ": ...)"); \
  static_assert(std::string_view(p27_PassedName) == NP_STRINGIFY(_np27_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np27_##ProcedureName) ": ...), got: " #ProcedureName "(" p27_PassedName ": ...)"); \
  static_assert(std::string_view(p28_PassedName) == NP_STRINGIFY(_np28_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np28_##ProcedureName) ": ...), got: " #ProcedureName "(" p28_PassedName ": ...)"); \
  static_assert(std::string_view(p29_PassedName) == NP_STRINGIFY(_np29_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np29_##ProcedureName) ": ...), got: " #ProcedureName "(" p29_PassedName ": ...)"); \
  static_assert(std::string_view(p30_PassedName) == NP_STRINGIFY(_np30_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np30_##ProcedureName) ": ...), got: " #ProcedureName "(" p30_PassedName ": ...)"); \
  static_assert(std::string_view(p31_PassedName) == NP_STRINGIFY(_np31_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np31_##ProcedureName) ": ...), got: " #ProcedureName "(" p31_PassedName ": ...)"); \
  static_assert(std::string_view(p32_PassedName) == NP_STRINGIFY(_np32_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np32_##ProcedureName) ": ...), got: " #ProcedureName "(" p32_PassedName ": ...)"); \
  static_assert(std::string_view(p33_PassedName) == NP_STRINGIFY(_np33_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np33_##ProcedureName) ": ...), got: " #ProcedureName "(" p33_PassedName ": ...)"); \
  static_assert(std::string_view(p34_PassedName) == NP_STRINGIFY(_np34_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np34_##ProcedureName) ": ...), got: " #ProcedureName "(" p34_PassedName ": ...)")

#define np35(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)"); \
  static_assert(std::string_view(p26_PassedName) == NP_STRINGIFY(_np26_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np26_##ProcedureName) ": ...), got: " #ProcedureName "(" p26_PassedName ": ...)"); \
  static_assert(std::string_view(p27_PassedName) == NP_STRINGIFY(_np27_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np27_##ProcedureName) ": ...), got: " #ProcedureName "(" p27_PassedName ": ...)"); \
  static_assert(std::string_view(p28_PassedName) == NP_STRINGIFY(_np28_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np28_##ProcedureName) ": ...), got: " #ProcedureName "(" p28_PassedName ": ...)"); \
  static_assert(std::string_view(p29_PassedName) == NP_STRINGIFY(_np29_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np29_##ProcedureName) ": ...), got: " #ProcedureName "(" p29_PassedName ": ...)"); \
  static_assert(std::string_view(p30_PassedName) == NP_STRINGIFY(_np30_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np30_##ProcedureName) ": ...), got: " #ProcedureName "(" p30_PassedName ": ...)"); \
  static_assert(std::string_view(p31_PassedName) == NP_STRINGIFY(_np31_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np31_##ProcedureName) ": ...), got: " #ProcedureName "(" p31_PassedName ": ...)"); \
  static_assert(std::string_view(p32_PassedName) == NP_STRINGIFY(_np32_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np32_##ProcedureName) ": ...), got: " #ProcedureName "(" p32_PassedName ": ...)"); \
  static_assert(std::string_view(p33_PassedName) == NP_STRINGIFY(_np33_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np33_##ProcedureName) ": ...), got: " #ProcedureName "(" p33_PassedName ": ...)"); \
  static_assert(std::string_view(p34_PassedName) == NP_STRINGIFY(_np34_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np34_##ProcedureName) ": ...), got: " #ProcedureName "(" p34_PassedName ": ...)"); \
  static_assert(std::string_view(p35_PassedName) == NP_STRINGIFY(_np35_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np35_##ProcedureName) ": ...), got: " #ProcedureName "(" p35_PassedName ": ...)")

#define np36(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)"); \
  static_assert(std::string_view(p26_PassedName) == NP_STRINGIFY(_np26_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np26_##ProcedureName) ": ...), got: " #ProcedureName "(" p26_PassedName ": ...)"); \
  static_assert(std::string_view(p27_PassedName) == NP_STRINGIFY(_np27_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np27_##ProcedureName) ": ...), got: " #ProcedureName "(" p27_PassedName ": ...)"); \
  static_assert(std::string_view(p28_PassedName) == NP_STRINGIFY(_np28_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np28_##ProcedureName) ": ...), got: " #ProcedureName "(" p28_PassedName ": ...)"); \
  static_assert(std::string_view(p29_PassedName) == NP_STRINGIFY(_np29_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np29_##ProcedureName) ": ...), got: " #ProcedureName "(" p29_PassedName ": ...)"); \
  static_assert(std::string_view(p30_PassedName) == NP_STRINGIFY(_np30_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np30_##ProcedureName) ": ...), got: " #ProcedureName "(" p30_PassedName ": ...)"); \
  static_assert(std::string_view(p31_PassedName) == NP_STRINGIFY(_np31_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np31_##ProcedureName) ": ...), got: " #ProcedureName "(" p31_PassedName ": ...)"); \
  static_assert(std::string_view(p32_PassedName) == NP_STRINGIFY(_np32_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np32_##ProcedureName) ": ...), got: " #ProcedureName "(" p32_PassedName ": ...)"); \
  static_assert(std::string_view(p33_PassedName) == NP_STRINGIFY(_np33_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np33_##ProcedureName) ": ...), got: " #ProcedureName "(" p33_PassedName ": ...)"); \
  static_assert(std::string_view(p34_PassedName) == NP_STRINGIFY(_np34_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np34_##ProcedureName) ": ...), got: " #ProcedureName "(" p34_PassedName ": ...)"); \
  static_assert(std::string_view(p35_PassedName) == NP_STRINGIFY(_np35_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np35_##ProcedureName) ": ...), got: " #ProcedureName "(" p35_PassedName ": ...)"); \
  static_assert(std::string_view(p36_PassedName) == NP_STRINGIFY(_np36_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np36_##ProcedureName) ": ...), got: " #ProcedureName "(" p36_PassedName ": ...)")

#define np37(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)"); \
  static_assert(std::string_view(p26_PassedName) == NP_STRINGIFY(_np26_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np26_##ProcedureName) ": ...), got: " #ProcedureName "(" p26_PassedName ": ...)"); \
  static_assert(std::string_view(p27_PassedName) == NP_STRINGIFY(_np27_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np27_##ProcedureName) ": ...), got: " #ProcedureName "(" p27_PassedName ": ...)"); \
  static_assert(std::string_view(p28_PassedName) == NP_STRINGIFY(_np28_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np28_##ProcedureName) ": ...), got: " #ProcedureName "(" p28_PassedName ": ...)"); \
  static_assert(std::string_view(p29_PassedName) == NP_STRINGIFY(_np29_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np29_##ProcedureName) ": ...), got: " #ProcedureName "(" p29_PassedName ": ...)"); \
  static_assert(std::string_view(p30_PassedName) == NP_STRINGIFY(_np30_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np30_##ProcedureName) ": ...), got: " #ProcedureName "(" p30_PassedName ": ...)"); \
  static_assert(std::string_view(p31_PassedName) == NP_STRINGIFY(_np31_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np31_##ProcedureName) ": ...), got: " #ProcedureName "(" p31_PassedName ": ...)"); \
  static_assert(std::string_view(p32_PassedName) == NP_STRINGIFY(_np32_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np32_##ProcedureName) ": ...), got: " #ProcedureName "(" p32_PassedName ": ...)"); \
  static_assert(std::string_view(p33_PassedName) == NP_STRINGIFY(_np33_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np33_##ProcedureName) ": ...), got: " #ProcedureName "(" p33_PassedName ": ...)"); \
  static_assert(std::string_view(p34_PassedName) == NP_STRINGIFY(_np34_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np34_##ProcedureName) ": ...), got: " #ProcedureName "(" p34_PassedName ": ...)"); \
  static_assert(std::string_view(p35_PassedName) == NP_STRINGIFY(_np35_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np35_##ProcedureName) ": ...), got: " #ProcedureName "(" p35_PassedName ": ...)"); \
  static_assert(std::string_view(p36_PassedName) == NP_STRINGIFY(_np36_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np36_##ProcedureName) ": ...), got: " #ProcedureName "(" p36_PassedName ": ...)"); \
  static_assert(std::string_view(p37_PassedName) == NP_STRINGIFY(_np37_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np37_##ProcedureName) ": ...), got: " #ProcedureName "(" p37_PassedName ": ...)")

#define np38(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)"); \
  static_assert(std::string_view(p26_PassedName) == NP_STRINGIFY(_np26_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np26_##ProcedureName) ": ...), got: " #ProcedureName "(" p26_PassedName ": ...)"); \
  static_assert(std::string_view(p27_PassedName) == NP_STRINGIFY(_np27_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np27_##ProcedureName) ": ...), got: " #ProcedureName "(" p27_PassedName ": ...)"); \
  static_assert(std::string_view(p28_PassedName) == NP_STRINGIFY(_np28_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np28_##ProcedureName) ": ...), got: " #ProcedureName "(" p28_PassedName ": ...)"); \
  static_assert(std::string_view(p29_PassedName) == NP_STRINGIFY(_np29_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np29_##ProcedureName) ": ...), got: " #ProcedureName "(" p29_PassedName ": ...)"); \
  static_assert(std::string_view(p30_PassedName) == NP_STRINGIFY(_np30_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np30_##ProcedureName) ": ...), got: " #ProcedureName "(" p30_PassedName ": ...)"); \
  static_assert(std::string_view(p31_PassedName) == NP_STRINGIFY(_np31_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np31_##ProcedureName) ": ...), got: " #ProcedureName "(" p31_PassedName ": ...)"); \
  static_assert(std::string_view(p32_PassedName) == NP_STRINGIFY(_np32_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np32_##ProcedureName) ": ...), got: " #ProcedureName "(" p32_PassedName ": ...)"); \
  static_assert(std::string_view(p33_PassedName) == NP_STRINGIFY(_np33_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np33_##ProcedureName) ": ...), got: " #ProcedureName "(" p33_PassedName ": ...)"); \
  static_assert(std::string_view(p34_PassedName) == NP_STRINGIFY(_np34_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np34_##ProcedureName) ": ...), got: " #ProcedureName "(" p34_PassedName ": ...)"); \
  static_assert(std::string_view(p35_PassedName) == NP_STRINGIFY(_np35_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np35_##ProcedureName) ": ...), got: " #ProcedureName "(" p35_PassedName ": ...)"); \
  static_assert(std::string_view(p36_PassedName) == NP_STRINGIFY(_np36_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np36_##ProcedureName) ": ...), got: " #ProcedureName "(" p36_PassedName ": ...)"); \
  static_assert(std::string_view(p37_PassedName) == NP_STRINGIFY(_np37_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np37_##ProcedureName) ": ...), got: " #ProcedureName "(" p37_PassedName ": ...)"); \
  static_assert(std::string_view(p38_PassedName) == NP_STRINGIFY(_np38_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np38_##ProcedureName) ": ...), got: " #ProcedureName "(" p38_PassedName ": ...)")

#define np39(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38, \
  p39_PassedName, p39 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)"); \
  static_assert(std::string_view(p26_PassedName) == NP_STRINGIFY(_np26_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np26_##ProcedureName) ": ...), got: " #ProcedureName "(" p26_PassedName ": ...)"); \
  static_assert(std::string_view(p27_PassedName) == NP_STRINGIFY(_np27_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np27_##ProcedureName) ": ...), got: " #ProcedureName "(" p27_PassedName ": ...)"); \
  static_assert(std::string_view(p28_PassedName) == NP_STRINGIFY(_np28_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np28_##ProcedureName) ": ...), got: " #ProcedureName "(" p28_PassedName ": ...)"); \
  static_assert(std::string_view(p29_PassedName) == NP_STRINGIFY(_np29_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np29_##ProcedureName) ": ...), got: " #ProcedureName "(" p29_PassedName ": ...)"); \
  static_assert(std::string_view(p30_PassedName) == NP_STRINGIFY(_np30_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np30_##ProcedureName) ": ...), got: " #ProcedureName "(" p30_PassedName ": ...)"); \
  static_assert(std::string_view(p31_PassedName) == NP_STRINGIFY(_np31_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np31_##ProcedureName) ": ...), got: " #ProcedureName "(" p31_PassedName ": ...)"); \
  static_assert(std::string_view(p32_PassedName) == NP_STRINGIFY(_np32_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np32_##ProcedureName) ": ...), got: " #ProcedureName "(" p32_PassedName ": ...)"); \
  static_assert(std::string_view(p33_PassedName) == NP_STRINGIFY(_np33_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np33_##ProcedureName) ": ...), got: " #ProcedureName "(" p33_PassedName ": ...)"); \
  static_assert(std::string_view(p34_PassedName) == NP_STRINGIFY(_np34_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np34_##ProcedureName) ": ...), got: " #ProcedureName "(" p34_PassedName ": ...)"); \
  static_assert(std::string_view(p35_PassedName) == NP_STRINGIFY(_np35_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np35_##ProcedureName) ": ...), got: " #ProcedureName "(" p35_PassedName ": ...)"); \
  static_assert(std::string_view(p36_PassedName) == NP_STRINGIFY(_np36_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np36_##ProcedureName) ": ...), got: " #ProcedureName "(" p36_PassedName ": ...)"); \
  static_assert(std::string_view(p37_PassedName) == NP_STRINGIFY(_np37_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np37_##ProcedureName) ": ...), got: " #ProcedureName "(" p37_PassedName ": ...)"); \
  static_assert(std::string_view(p38_PassedName) == NP_STRINGIFY(_np38_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np38_##ProcedureName) ": ...), got: " #ProcedureName "(" p38_PassedName ": ...)"); \
  static_assert(std::string_view(p39_PassedName) == NP_STRINGIFY(_np39_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np39_##ProcedureName) ": ...), got: " #ProcedureName "(" p39_PassedName ": ...)")

#define np40(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38, \
  p39_PassedName, p39, \
  p40_PassedName, p40 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)"); \
  static_assert(std::string_view(p26_PassedName) == NP_STRINGIFY(_np26_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np26_##ProcedureName) ": ...), got: " #ProcedureName "(" p26_PassedName ": ...)"); \
  static_assert(std::string_view(p27_PassedName) == NP_STRINGIFY(_np27_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np27_##ProcedureName) ": ...), got: " #ProcedureName "(" p27_PassedName ": ...)"); \
  static_assert(std::string_view(p28_PassedName) == NP_STRINGIFY(_np28_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np28_##ProcedureName) ": ...), got: " #ProcedureName "(" p28_PassedName ": ...)"); \
  static_assert(std::string_view(p29_PassedName) == NP_STRINGIFY(_np29_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np29_##ProcedureName) ": ...), got: " #ProcedureName "(" p29_PassedName ": ...)"); \
  static_assert(std::string_view(p30_PassedName) == NP_STRINGIFY(_np30_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np30_##ProcedureName) ": ...), got: " #ProcedureName "(" p30_PassedName ": ...)"); \
  static_assert(std::string_view(p31_PassedName) == NP_STRINGIFY(_np31_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np31_##ProcedureName) ": ...), got: " #ProcedureName "(" p31_PassedName ": ...)"); \
  static_assert(std::string_view(p32_PassedName) == NP_STRINGIFY(_np32_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np32_##ProcedureName) ": ...), got: " #ProcedureName "(" p32_PassedName ": ...)"); \
  static_assert(std::string_view(p33_PassedName) == NP_STRINGIFY(_np33_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np33_##ProcedureName) ": ...), got: " #ProcedureName "(" p33_PassedName ": ...)"); \
  static_assert(std::string_view(p34_PassedName) == NP_STRINGIFY(_np34_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np34_##ProcedureName) ": ...), got: " #ProcedureName "(" p34_PassedName ": ...)"); \
  static_assert(std::string_view(p35_PassedName) == NP_STRINGIFY(_np35_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np35_##ProcedureName) ": ...), got: " #ProcedureName "(" p35_PassedName ": ...)"); \
  static_assert(std::string_view(p36_PassedName) == NP_STRINGIFY(_np36_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np36_##ProcedureName) ": ...), got: " #ProcedureName "(" p36_PassedName ": ...)"); \
  static_assert(std::string_view(p37_PassedName) == NP_STRINGIFY(_np37_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np37_##ProcedureName) ": ...), got: " #ProcedureName "(" p37_PassedName ": ...)"); \
  static_assert(std::string_view(p38_PassedName) == NP_STRINGIFY(_np38_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np38_##ProcedureName) ": ...), got: " #ProcedureName "(" p38_PassedName ": ...)"); \
  static_assert(std::string_view(p39_PassedName) == NP_STRINGIFY(_np39_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np39_##ProcedureName) ": ...), got: " #ProcedureName "(" p39_PassedName ": ...)"); \
  static_assert(std::string_view(p40_PassedName) == NP_STRINGIFY(_np40_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np40_##ProcedureName) ": ...), got: " #ProcedureName "(" p40_PassedName ": ...)")

#define np41(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38, \
  p39_PassedName, p39, \
  p40_PassedName, p40, \
  p41_PassedName, p41 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)"); \
  static_assert(std::string_view(p26_PassedName) == NP_STRINGIFY(_np26_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np26_##ProcedureName) ": ...), got: " #ProcedureName "(" p26_PassedName ": ...)"); \
  static_assert(std::string_view(p27_PassedName) == NP_STRINGIFY(_np27_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np27_##ProcedureName) ": ...), got: " #ProcedureName "(" p27_PassedName ": ...)"); \
  static_assert(std::string_view(p28_PassedName) == NP_STRINGIFY(_np28_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np28_##ProcedureName) ": ...), got: " #ProcedureName "(" p28_PassedName ": ...)"); \
  static_assert(std::string_view(p29_PassedName) == NP_STRINGIFY(_np29_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np29_##ProcedureName) ": ...), got: " #ProcedureName "(" p29_PassedName ": ...)"); \
  static_assert(std::string_view(p30_PassedName) == NP_STRINGIFY(_np30_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np30_##ProcedureName) ": ...), got: " #ProcedureName "(" p30_PassedName ": ...)"); \
  static_assert(std::string_view(p31_PassedName) == NP_STRINGIFY(_np31_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np31_##ProcedureName) ": ...), got: " #ProcedureName "(" p31_PassedName ": ...)"); \
  static_assert(std::string_view(p32_PassedName) == NP_STRINGIFY(_np32_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np32_##ProcedureName) ": ...), got: " #ProcedureName "(" p32_PassedName ": ...)"); \
  static_assert(std::string_view(p33_PassedName) == NP_STRINGIFY(_np33_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np33_##ProcedureName) ": ...), got: " #ProcedureName "(" p33_PassedName ": ...)"); \
  static_assert(std::string_view(p34_PassedName) == NP_STRINGIFY(_np34_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np34_##ProcedureName) ": ...), got: " #ProcedureName "(" p34_PassedName ": ...)"); \
  static_assert(std::string_view(p35_PassedName) == NP_STRINGIFY(_np35_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np35_##ProcedureName) ": ...), got: " #ProcedureName "(" p35_PassedName ": ...)"); \
  static_assert(std::string_view(p36_PassedName) == NP_STRINGIFY(_np36_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np36_##ProcedureName) ": ...), got: " #ProcedureName "(" p36_PassedName ": ...)"); \
  static_assert(std::string_view(p37_PassedName) == NP_STRINGIFY(_np37_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np37_##ProcedureName) ": ...), got: " #ProcedureName "(" p37_PassedName ": ...)"); \
  static_assert(std::string_view(p38_PassedName) == NP_STRINGIFY(_np38_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np38_##ProcedureName) ": ...), got: " #ProcedureName "(" p38_PassedName ": ...)"); \
  static_assert(std::string_view(p39_PassedName) == NP_STRINGIFY(_np39_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np39_##ProcedureName) ": ...), got: " #ProcedureName "(" p39_PassedName ": ...)"); \
  static_assert(std::string_view(p40_PassedName) == NP_STRINGIFY(_np40_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np40_##ProcedureName) ": ...), got: " #ProcedureName "(" p40_PassedName ": ...)"); \
  static_assert(std::string_view(p41_PassedName) == NP_STRINGIFY(_np41_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np41_##ProcedureName) ": ...), got: " #ProcedureName "(" p41_PassedName ": ...)")

#define np42(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38, \
  p39_PassedName, p39, \
  p40_PassedName, p40, \
  p41_PassedName, p41, \
  p42_PassedName, p42 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)"); \
  static_assert(std::string_view(p26_PassedName) == NP_STRINGIFY(_np26_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np26_##ProcedureName) ": ...), got: " #ProcedureName "(" p26_PassedName ": ...)"); \
  static_assert(std::string_view(p27_PassedName) == NP_STRINGIFY(_np27_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np27_##ProcedureName) ": ...), got: " #ProcedureName "(" p27_PassedName ": ...)"); \
  static_assert(std::string_view(p28_PassedName) == NP_STRINGIFY(_np28_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np28_##ProcedureName) ": ...), got: " #ProcedureName "(" p28_PassedName ": ...)"); \
  static_assert(std::string_view(p29_PassedName) == NP_STRINGIFY(_np29_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np29_##ProcedureName) ": ...), got: " #ProcedureName "(" p29_PassedName ": ...)"); \
  static_assert(std::string_view(p30_PassedName) == NP_STRINGIFY(_np30_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np30_##ProcedureName) ": ...), got: " #ProcedureName "(" p30_PassedName ": ...)"); \
  static_assert(std::string_view(p31_PassedName) == NP_STRINGIFY(_np31_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np31_##ProcedureName) ": ...), got: " #ProcedureName "(" p31_PassedName ": ...)"); \
  static_assert(std::string_view(p32_PassedName) == NP_STRINGIFY(_np32_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np32_##ProcedureName) ": ...), got: " #ProcedureName "(" p32_PassedName ": ...)"); \
  static_assert(std::string_view(p33_PassedName) == NP_STRINGIFY(_np33_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np33_##ProcedureName) ": ...), got: " #ProcedureName "(" p33_PassedName ": ...)"); \
  static_assert(std::string_view(p34_PassedName) == NP_STRINGIFY(_np34_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np34_##ProcedureName) ": ...), got: " #ProcedureName "(" p34_PassedName ": ...)"); \
  static_assert(std::string_view(p35_PassedName) == NP_STRINGIFY(_np35_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np35_##ProcedureName) ": ...), got: " #ProcedureName "(" p35_PassedName ": ...)"); \
  static_assert(std::string_view(p36_PassedName) == NP_STRINGIFY(_np36_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np36_##ProcedureName) ": ...), got: " #ProcedureName "(" p36_PassedName ": ...)"); \
  static_assert(std::string_view(p37_PassedName) == NP_STRINGIFY(_np37_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np37_##ProcedureName) ": ...), got: " #ProcedureName "(" p37_PassedName ": ...)"); \
  static_assert(std::string_view(p38_PassedName) == NP_STRINGIFY(_np38_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np38_##ProcedureName) ": ...), got: " #ProcedureName "(" p38_PassedName ": ...)"); \
  static_assert(std::string_view(p39_PassedName) == NP_STRINGIFY(_np39_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np39_##ProcedureName) ": ...), got: " #ProcedureName "(" p39_PassedName ": ...)"); \
  static_assert(std::string_view(p40_PassedName) == NP_STRINGIFY(_np40_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np40_##ProcedureName) ": ...), got: " #ProcedureName "(" p40_PassedName ": ...)"); \
  static_assert(std::string_view(p41_PassedName) == NP_STRINGIFY(_np41_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np41_##ProcedureName) ": ...), got: " #ProcedureName "(" p41_PassedName ": ...)"); \
  static_assert(std::string_view(p42_PassedName) == NP_STRINGIFY(_np42_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np42_##ProcedureName) ": ...), got: " #ProcedureName "(" p42_PassedName ": ...)")

#define np43(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38, \
  p39_PassedName, p39, \
  p40_PassedName, p40, \
  p41_PassedName, p41, \
  p42_PassedName, p42, \
  p43_PassedName, p43 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)"); \
  static_assert(std::string_view(p26_PassedName) == NP_STRINGIFY(_np26_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np26_##ProcedureName) ": ...), got: " #ProcedureName "(" p26_PassedName ": ...)"); \
  static_assert(std::string_view(p27_PassedName) == NP_STRINGIFY(_np27_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np27_##ProcedureName) ": ...), got: " #ProcedureName "(" p27_PassedName ": ...)"); \
  static_assert(std::string_view(p28_PassedName) == NP_STRINGIFY(_np28_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np28_##ProcedureName) ": ...), got: " #ProcedureName "(" p28_PassedName ": ...)"); \
  static_assert(std::string_view(p29_PassedName) == NP_STRINGIFY(_np29_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np29_##ProcedureName) ": ...), got: " #ProcedureName "(" p29_PassedName ": ...)"); \
  static_assert(std::string_view(p30_PassedName) == NP_STRINGIFY(_np30_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np30_##ProcedureName) ": ...), got: " #ProcedureName "(" p30_PassedName ": ...)"); \
  static_assert(std::string_view(p31_PassedName) == NP_STRINGIFY(_np31_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np31_##ProcedureName) ": ...), got: " #ProcedureName "(" p31_PassedName ": ...)"); \
  static_assert(std::string_view(p32_PassedName) == NP_STRINGIFY(_np32_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np32_##ProcedureName) ": ...), got: " #ProcedureName "(" p32_PassedName ": ...)"); \
  static_assert(std::string_view(p33_PassedName) == NP_STRINGIFY(_np33_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np33_##ProcedureName) ": ...), got: " #ProcedureName "(" p33_PassedName ": ...)"); \
  static_assert(std::string_view(p34_PassedName) == NP_STRINGIFY(_np34_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np34_##ProcedureName) ": ...), got: " #ProcedureName "(" p34_PassedName ": ...)"); \
  static_assert(std::string_view(p35_PassedName) == NP_STRINGIFY(_np35_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np35_##ProcedureName) ": ...), got: " #ProcedureName "(" p35_PassedName ": ...)"); \
  static_assert(std::string_view(p36_PassedName) == NP_STRINGIFY(_np36_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np36_##ProcedureName) ": ...), got: " #ProcedureName "(" p36_PassedName ": ...)"); \
  static_assert(std::string_view(p37_PassedName) == NP_STRINGIFY(_np37_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np37_##ProcedureName) ": ...), got: " #ProcedureName "(" p37_PassedName ": ...)"); \
  static_assert(std::string_view(p38_PassedName) == NP_STRINGIFY(_np38_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np38_##ProcedureName) ": ...), got: " #ProcedureName "(" p38_PassedName ": ...)"); \
  static_assert(std::string_view(p39_PassedName) == NP_STRINGIFY(_np39_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np39_##ProcedureName) ": ...), got: " #ProcedureName "(" p39_PassedName ": ...)"); \
  static_assert(std::string_view(p40_PassedName) == NP_STRINGIFY(_np40_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np40_##ProcedureName) ": ...), got: " #ProcedureName "(" p40_PassedName ": ...)"); \
  static_assert(std::string_view(p41_PassedName) == NP_STRINGIFY(_np41_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np41_##ProcedureName) ": ...), got: " #ProcedureName "(" p41_PassedName ": ...)"); \
  static_assert(std::string_view(p42_PassedName) == NP_STRINGIFY(_np42_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np42_##ProcedureName) ": ...), got: " #ProcedureName "(" p42_PassedName ": ...)"); \
  static_assert(std::string_view(p43_PassedName) == NP_STRINGIFY(_np43_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np43_##ProcedureName) ": ...), got: " #ProcedureName "(" p43_PassedName ": ...)")

#define np44(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38, \
  p39_PassedName, p39, \
  p40_PassedName, p40, \
  p41_PassedName, p41, \
  p42_PassedName, p42, \
  p43_PassedName, p43, \
  p44_PassedName, p44 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)"); \
  static_assert(std::string_view(p26_PassedName) == NP_STRINGIFY(_np26_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np26_##ProcedureName) ": ...), got: " #ProcedureName "(" p26_PassedName ": ...)"); \
  static_assert(std::string_view(p27_PassedName) == NP_STRINGIFY(_np27_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np27_##ProcedureName) ": ...), got: " #ProcedureName "(" p27_PassedName ": ...)"); \
  static_assert(std::string_view(p28_PassedName) == NP_STRINGIFY(_np28_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np28_##ProcedureName) ": ...), got: " #ProcedureName "(" p28_PassedName ": ...)"); \
  static_assert(std::string_view(p29_PassedName) == NP_STRINGIFY(_np29_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np29_##ProcedureName) ": ...), got: " #ProcedureName "(" p29_PassedName ": ...)"); \
  static_assert(std::string_view(p30_PassedName) == NP_STRINGIFY(_np30_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np30_##ProcedureName) ": ...), got: " #ProcedureName "(" p30_PassedName ": ...)"); \
  static_assert(std::string_view(p31_PassedName) == NP_STRINGIFY(_np31_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np31_##ProcedureName) ": ...), got: " #ProcedureName "(" p31_PassedName ": ...)"); \
  static_assert(std::string_view(p32_PassedName) == NP_STRINGIFY(_np32_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np32_##ProcedureName) ": ...), got: " #ProcedureName "(" p32_PassedName ": ...)"); \
  static_assert(std::string_view(p33_PassedName) == NP_STRINGIFY(_np33_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np33_##ProcedureName) ": ...), got: " #ProcedureName "(" p33_PassedName ": ...)"); \
  static_assert(std::string_view(p34_PassedName) == NP_STRINGIFY(_np34_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np34_##ProcedureName) ": ...), got: " #ProcedureName "(" p34_PassedName ": ...)"); \
  static_assert(std::string_view(p35_PassedName) == NP_STRINGIFY(_np35_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np35_##ProcedureName) ": ...), got: " #ProcedureName "(" p35_PassedName ": ...)"); \
  static_assert(std::string_view(p36_PassedName) == NP_STRINGIFY(_np36_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np36_##ProcedureName) ": ...), got: " #ProcedureName "(" p36_PassedName ": ...)"); \
  static_assert(std::string_view(p37_PassedName) == NP_STRINGIFY(_np37_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np37_##ProcedureName) ": ...), got: " #ProcedureName "(" p37_PassedName ": ...)"); \
  static_assert(std::string_view(p38_PassedName) == NP_STRINGIFY(_np38_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np38_##ProcedureName) ": ...), got: " #ProcedureName "(" p38_PassedName ": ...)"); \
  static_assert(std::string_view(p39_PassedName) == NP_STRINGIFY(_np39_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np39_##ProcedureName) ": ...), got: " #ProcedureName "(" p39_PassedName ": ...)"); \
  static_assert(std::string_view(p40_PassedName) == NP_STRINGIFY(_np40_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np40_##ProcedureName) ": ...), got: " #ProcedureName "(" p40_PassedName ": ...)"); \
  static_assert(std::string_view(p41_PassedName) == NP_STRINGIFY(_np41_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np41_##ProcedureName) ": ...), got: " #ProcedureName "(" p41_PassedName ": ...)"); \
  static_assert(std::string_view(p42_PassedName) == NP_STRINGIFY(_np42_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np42_##ProcedureName) ": ...), got: " #ProcedureName "(" p42_PassedName ": ...)"); \
  static_assert(std::string_view(p43_PassedName) == NP_STRINGIFY(_np43_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np43_##ProcedureName) ": ...), got: " #ProcedureName "(" p43_PassedName ": ...)"); \
  static_assert(std::string_view(p44_PassedName) == NP_STRINGIFY(_np44_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np44_##ProcedureName) ": ...), got: " #ProcedureName "(" p44_PassedName ": ...)")

#define np45(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38, \
  p39_PassedName, p39, \
  p40_PassedName, p40, \
  p41_PassedName, p41, \
  p42_PassedName, p42, \
  p43_PassedName, p43, \
  p44_PassedName, p44, \
  p45_PassedName, p45 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)"); \
  static_assert(std::string_view(p26_PassedName) == NP_STRINGIFY(_np26_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np26_##ProcedureName) ": ...), got: " #ProcedureName "(" p26_PassedName ": ...)"); \
  static_assert(std::string_view(p27_PassedName) == NP_STRINGIFY(_np27_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np27_##ProcedureName) ": ...), got: " #ProcedureName "(" p27_PassedName ": ...)"); \
  static_assert(std::string_view(p28_PassedName) == NP_STRINGIFY(_np28_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np28_##ProcedureName) ": ...), got: " #ProcedureName "(" p28_PassedName ": ...)"); \
  static_assert(std::string_view(p29_PassedName) == NP_STRINGIFY(_np29_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np29_##ProcedureName) ": ...), got: " #ProcedureName "(" p29_PassedName ": ...)"); \
  static_assert(std::string_view(p30_PassedName) == NP_STRINGIFY(_np30_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np30_##ProcedureName) ": ...), got: " #ProcedureName "(" p30_PassedName ": ...)"); \
  static_assert(std::string_view(p31_PassedName) == NP_STRINGIFY(_np31_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np31_##ProcedureName) ": ...), got: " #ProcedureName "(" p31_PassedName ": ...)"); \
  static_assert(std::string_view(p32_PassedName) == NP_STRINGIFY(_np32_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np32_##ProcedureName) ": ...), got: " #ProcedureName "(" p32_PassedName ": ...)"); \
  static_assert(std::string_view(p33_PassedName) == NP_STRINGIFY(_np33_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np33_##ProcedureName) ": ...), got: " #ProcedureName "(" p33_PassedName ": ...)"); \
  static_assert(std::string_view(p34_PassedName) == NP_STRINGIFY(_np34_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np34_##ProcedureName) ": ...), got: " #ProcedureName "(" p34_PassedName ": ...)"); \
  static_assert(std::string_view(p35_PassedName) == NP_STRINGIFY(_np35_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np35_##ProcedureName) ": ...), got: " #ProcedureName "(" p35_PassedName ": ...)"); \
  static_assert(std::string_view(p36_PassedName) == NP_STRINGIFY(_np36_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np36_##ProcedureName) ": ...), got: " #ProcedureName "(" p36_PassedName ": ...)"); \
  static_assert(std::string_view(p37_PassedName) == NP_STRINGIFY(_np37_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np37_##ProcedureName) ": ...), got: " #ProcedureName "(" p37_PassedName ": ...)"); \
  static_assert(std::string_view(p38_PassedName) == NP_STRINGIFY(_np38_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np38_##ProcedureName) ": ...), got: " #ProcedureName "(" p38_PassedName ": ...)"); \
  static_assert(std::string_view(p39_PassedName) == NP_STRINGIFY(_np39_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np39_##ProcedureName) ": ...), got: " #ProcedureName "(" p39_PassedName ": ...)"); \
  static_assert(std::string_view(p40_PassedName) == NP_STRINGIFY(_np40_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np40_##ProcedureName) ": ...), got: " #ProcedureName "(" p40_PassedName ": ...)"); \
  static_assert(std::string_view(p41_PassedName) == NP_STRINGIFY(_np41_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np41_##ProcedureName) ": ...), got: " #ProcedureName "(" p41_PassedName ": ...)"); \
  static_assert(std::string_view(p42_PassedName) == NP_STRINGIFY(_np42_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np42_##ProcedureName) ": ...), got: " #ProcedureName "(" p42_PassedName ": ...)"); \
  static_assert(std::string_view(p43_PassedName) == NP_STRINGIFY(_np43_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np43_##ProcedureName) ": ...), got: " #ProcedureName "(" p43_PassedName ": ...)"); \
  static_assert(std::string_view(p44_PassedName) == NP_STRINGIFY(_np44_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np44_##ProcedureName) ": ...), got: " #ProcedureName "(" p44_PassedName ": ...)"); \
  static_assert(std::string_view(p45_PassedName) == NP_STRINGIFY(_np45_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np45_##ProcedureName) ": ...), got: " #ProcedureName "(" p45_PassedName ": ...)")

#define np46(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38, \
  p39_PassedName, p39, \
  p40_PassedName, p40, \
  p41_PassedName, p41, \
  p42_PassedName, p42, \
  p43_PassedName, p43, \
  p44_PassedName, p44, \
  p45_PassedName, p45, \
  p46_PassedName, p46 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)"); \
  static_assert(std::string_view(p26_PassedName) == NP_STRINGIFY(_np26_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np26_##ProcedureName) ": ...), got: " #ProcedureName "(" p26_PassedName ": ...)"); \
  static_assert(std::string_view(p27_PassedName) == NP_STRINGIFY(_np27_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np27_##ProcedureName) ": ...), got: " #ProcedureName "(" p27_PassedName ": ...)"); \
  static_assert(std::string_view(p28_PassedName) == NP_STRINGIFY(_np28_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np28_##ProcedureName) ": ...), got: " #ProcedureName "(" p28_PassedName ": ...)"); \
  static_assert(std::string_view(p29_PassedName) == NP_STRINGIFY(_np29_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np29_##ProcedureName) ": ...), got: " #ProcedureName "(" p29_PassedName ": ...)"); \
  static_assert(std::string_view(p30_PassedName) == NP_STRINGIFY(_np30_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np30_##ProcedureName) ": ...), got: " #ProcedureName "(" p30_PassedName ": ...)"); \
  static_assert(std::string_view(p31_PassedName) == NP_STRINGIFY(_np31_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np31_##ProcedureName) ": ...), got: " #ProcedureName "(" p31_PassedName ": ...)"); \
  static_assert(std::string_view(p32_PassedName) == NP_STRINGIFY(_np32_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np32_##ProcedureName) ": ...), got: " #ProcedureName "(" p32_PassedName ": ...)"); \
  static_assert(std::string_view(p33_PassedName) == NP_STRINGIFY(_np33_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np33_##ProcedureName) ": ...), got: " #ProcedureName "(" p33_PassedName ": ...)"); \
  static_assert(std::string_view(p34_PassedName) == NP_STRINGIFY(_np34_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np34_##ProcedureName) ": ...), got: " #ProcedureName "(" p34_PassedName ": ...)"); \
  static_assert(std::string_view(p35_PassedName) == NP_STRINGIFY(_np35_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np35_##ProcedureName) ": ...), got: " #ProcedureName "(" p35_PassedName ": ...)"); \
  static_assert(std::string_view(p36_PassedName) == NP_STRINGIFY(_np36_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np36_##ProcedureName) ": ...), got: " #ProcedureName "(" p36_PassedName ": ...)"); \
  static_assert(std::string_view(p37_PassedName) == NP_STRINGIFY(_np37_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np37_##ProcedureName) ": ...), got: " #ProcedureName "(" p37_PassedName ": ...)"); \
  static_assert(std::string_view(p38_PassedName) == NP_STRINGIFY(_np38_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np38_##ProcedureName) ": ...), got: " #ProcedureName "(" p38_PassedName ": ...)"); \
  static_assert(std::string_view(p39_PassedName) == NP_STRINGIFY(_np39_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np39_##ProcedureName) ": ...), got: " #ProcedureName "(" p39_PassedName ": ...)"); \
  static_assert(std::string_view(p40_PassedName) == NP_STRINGIFY(_np40_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np40_##ProcedureName) ": ...), got: " #ProcedureName "(" p40_PassedName ": ...)"); \
  static_assert(std::string_view(p41_PassedName) == NP_STRINGIFY(_np41_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np41_##ProcedureName) ": ...), got: " #ProcedureName "(" p41_PassedName ": ...)"); \
  static_assert(std::string_view(p42_PassedName) == NP_STRINGIFY(_np42_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np42_##ProcedureName) ": ...), got: " #ProcedureName "(" p42_PassedName ": ...)"); \
  static_assert(std::string_view(p43_PassedName) == NP_STRINGIFY(_np43_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np43_##ProcedureName) ": ...), got: " #ProcedureName "(" p43_PassedName ": ...)"); \
  static_assert(std::string_view(p44_PassedName) == NP_STRINGIFY(_np44_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np44_##ProcedureName) ": ...), got: " #ProcedureName "(" p44_PassedName ": ...)"); \
  static_assert(std::string_view(p45_PassedName) == NP_STRINGIFY(_np45_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np45_##ProcedureName) ": ...), got: " #ProcedureName "(" p45_PassedName ": ...)"); \
  static_assert(std::string_view(p46_PassedName) == NP_STRINGIFY(_np46_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np46_##ProcedureName) ": ...), got: " #ProcedureName "(" p46_PassedName ": ...)")

#define np47(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38, \
  p39_PassedName, p39, \
  p40_PassedName, p40, \
  p41_PassedName, p41, \
  p42_PassedName, p42, \
  p43_PassedName, p43, \
  p44_PassedName, p44, \
  p45_PassedName, p45, \
  p46_PassedName, p46, \
  p47_PassedName, p47 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)"); \
  static_assert(std::string_view(p26_PassedName) == NP_STRINGIFY(_np26_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np26_##ProcedureName) ": ...), got: " #ProcedureName "(" p26_PassedName ": ...)"); \
  static_assert(std::string_view(p27_PassedName) == NP_STRINGIFY(_np27_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np27_##ProcedureName) ": ...), got: " #ProcedureName "(" p27_PassedName ": ...)"); \
  static_assert(std::string_view(p28_PassedName) == NP_STRINGIFY(_np28_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np28_##ProcedureName) ": ...), got: " #ProcedureName "(" p28_PassedName ": ...)"); \
  static_assert(std::string_view(p29_PassedName) == NP_STRINGIFY(_np29_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np29_##ProcedureName) ": ...), got: " #ProcedureName "(" p29_PassedName ": ...)"); \
  static_assert(std::string_view(p30_PassedName) == NP_STRINGIFY(_np30_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np30_##ProcedureName) ": ...), got: " #ProcedureName "(" p30_PassedName ": ...)"); \
  static_assert(std::string_view(p31_PassedName) == NP_STRINGIFY(_np31_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np31_##ProcedureName) ": ...), got: " #ProcedureName "(" p31_PassedName ": ...)"); \
  static_assert(std::string_view(p32_PassedName) == NP_STRINGIFY(_np32_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np32_##ProcedureName) ": ...), got: " #ProcedureName "(" p32_PassedName ": ...)"); \
  static_assert(std::string_view(p33_PassedName) == NP_STRINGIFY(_np33_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np33_##ProcedureName) ": ...), got: " #ProcedureName "(" p33_PassedName ": ...)"); \
  static_assert(std::string_view(p34_PassedName) == NP_STRINGIFY(_np34_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np34_##ProcedureName) ": ...), got: " #ProcedureName "(" p34_PassedName ": ...)"); \
  static_assert(std::string_view(p35_PassedName) == NP_STRINGIFY(_np35_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np35_##ProcedureName) ": ...), got: " #ProcedureName "(" p35_PassedName ": ...)"); \
  static_assert(std::string_view(p36_PassedName) == NP_STRINGIFY(_np36_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np36_##ProcedureName) ": ...), got: " #ProcedureName "(" p36_PassedName ": ...)"); \
  static_assert(std::string_view(p37_PassedName) == NP_STRINGIFY(_np37_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np37_##ProcedureName) ": ...), got: " #ProcedureName "(" p37_PassedName ": ...)"); \
  static_assert(std::string_view(p38_PassedName) == NP_STRINGIFY(_np38_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np38_##ProcedureName) ": ...), got: " #ProcedureName "(" p38_PassedName ": ...)"); \
  static_assert(std::string_view(p39_PassedName) == NP_STRINGIFY(_np39_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np39_##ProcedureName) ": ...), got: " #ProcedureName "(" p39_PassedName ": ...)"); \
  static_assert(std::string_view(p40_PassedName) == NP_STRINGIFY(_np40_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np40_##ProcedureName) ": ...), got: " #ProcedureName "(" p40_PassedName ": ...)"); \
  static_assert(std::string_view(p41_PassedName) == NP_STRINGIFY(_np41_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np41_##ProcedureName) ": ...), got: " #ProcedureName "(" p41_PassedName ": ...)"); \
  static_assert(std::string_view(p42_PassedName) == NP_STRINGIFY(_np42_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np42_##ProcedureName) ": ...), got: " #ProcedureName "(" p42_PassedName ": ...)"); \
  static_assert(std::string_view(p43_PassedName) == NP_STRINGIFY(_np43_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np43_##ProcedureName) ": ...), got: " #ProcedureName "(" p43_PassedName ": ...)"); \
  static_assert(std::string_view(p44_PassedName) == NP_STRINGIFY(_np44_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np44_##ProcedureName) ": ...), got: " #ProcedureName "(" p44_PassedName ": ...)"); \
  static_assert(std::string_view(p45_PassedName) == NP_STRINGIFY(_np45_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np45_##ProcedureName) ": ...), got: " #ProcedureName "(" p45_PassedName ": ...)"); \
  static_assert(std::string_view(p46_PassedName) == NP_STRINGIFY(_np46_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np46_##ProcedureName) ": ...), got: " #ProcedureName "(" p46_PassedName ": ...)"); \
  static_assert(std::string_view(p47_PassedName) == NP_STRINGIFY(_np47_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np47_##ProcedureName) ": ...), got: " #ProcedureName "(" p47_PassedName ": ...)")

#define np48(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38, \
  p39_PassedName, p39, \
  p40_PassedName, p40, \
  p41_PassedName, p41, \
  p42_PassedName, p42, \
  p43_PassedName, p43, \
  p44_PassedName, p44, \
  p45_PassedName, p45, \
  p46_PassedName, p46, \
  p47_PassedName, p47, \
  p48_PassedName, p48 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)"); \
  static_assert(std::string_view(p26_PassedName) == NP_STRINGIFY(_np26_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np26_##ProcedureName) ": ...), got: " #ProcedureName "(" p26_PassedName ": ...)"); \
  static_assert(std::string_view(p27_PassedName) == NP_STRINGIFY(_np27_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np27_##ProcedureName) ": ...), got: " #ProcedureName "(" p27_PassedName ": ...)"); \
  static_assert(std::string_view(p28_PassedName) == NP_STRINGIFY(_np28_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np28_##ProcedureName) ": ...), got: " #ProcedureName "(" p28_PassedName ": ...)"); \
  static_assert(std::string_view(p29_PassedName) == NP_STRINGIFY(_np29_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np29_##ProcedureName) ": ...), got: " #ProcedureName "(" p29_PassedName ": ...)"); \
  static_assert(std::string_view(p30_PassedName) == NP_STRINGIFY(_np30_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np30_##ProcedureName) ": ...), got: " #ProcedureName "(" p30_PassedName ": ...)"); \
  static_assert(std::string_view(p31_PassedName) == NP_STRINGIFY(_np31_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np31_##ProcedureName) ": ...), got: " #ProcedureName "(" p31_PassedName ": ...)"); \
  static_assert(std::string_view(p32_PassedName) == NP_STRINGIFY(_np32_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np32_##ProcedureName) ": ...), got: " #ProcedureName "(" p32_PassedName ": ...)"); \
  static_assert(std::string_view(p33_PassedName) == NP_STRINGIFY(_np33_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np33_##ProcedureName) ": ...), got: " #ProcedureName "(" p33_PassedName ": ...)"); \
  static_assert(std::string_view(p34_PassedName) == NP_STRINGIFY(_np34_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np34_##ProcedureName) ": ...), got: " #ProcedureName "(" p34_PassedName ": ...)"); \
  static_assert(std::string_view(p35_PassedName) == NP_STRINGIFY(_np35_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np35_##ProcedureName) ": ...), got: " #ProcedureName "(" p35_PassedName ": ...)"); \
  static_assert(std::string_view(p36_PassedName) == NP_STRINGIFY(_np36_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np36_##ProcedureName) ": ...), got: " #ProcedureName "(" p36_PassedName ": ...)"); \
  static_assert(std::string_view(p37_PassedName) == NP_STRINGIFY(_np37_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np37_##ProcedureName) ": ...), got: " #ProcedureName "(" p37_PassedName ": ...)"); \
  static_assert(std::string_view(p38_PassedName) == NP_STRINGIFY(_np38_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np38_##ProcedureName) ": ...), got: " #ProcedureName "(" p38_PassedName ": ...)"); \
  static_assert(std::string_view(p39_PassedName) == NP_STRINGIFY(_np39_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np39_##ProcedureName) ": ...), got: " #ProcedureName "(" p39_PassedName ": ...)"); \
  static_assert(std::string_view(p40_PassedName) == NP_STRINGIFY(_np40_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np40_##ProcedureName) ": ...), got: " #ProcedureName "(" p40_PassedName ": ...)"); \
  static_assert(std::string_view(p41_PassedName) == NP_STRINGIFY(_np41_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np41_##ProcedureName) ": ...), got: " #ProcedureName "(" p41_PassedName ": ...)"); \
  static_assert(std::string_view(p42_PassedName) == NP_STRINGIFY(_np42_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np42_##ProcedureName) ": ...), got: " #ProcedureName "(" p42_PassedName ": ...)"); \
  static_assert(std::string_view(p43_PassedName) == NP_STRINGIFY(_np43_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np43_##ProcedureName) ": ...), got: " #ProcedureName "(" p43_PassedName ": ...)"); \
  static_assert(std::string_view(p44_PassedName) == NP_STRINGIFY(_np44_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np44_##ProcedureName) ": ...), got: " #ProcedureName "(" p44_PassedName ": ...)"); \
  static_assert(std::string_view(p45_PassedName) == NP_STRINGIFY(_np45_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np45_##ProcedureName) ": ...), got: " #ProcedureName "(" p45_PassedName ": ...)"); \
  static_assert(std::string_view(p46_PassedName) == NP_STRINGIFY(_np46_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np46_##ProcedureName) ": ...), got: " #ProcedureName "(" p46_PassedName ": ...)"); \
  static_assert(std::string_view(p47_PassedName) == NP_STRINGIFY(_np47_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np47_##ProcedureName) ": ...), got: " #ProcedureName "(" p47_PassedName ": ...)"); \
  static_assert(std::string_view(p48_PassedName) == NP_STRINGIFY(_np48_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np48_##ProcedureName) ": ...), got: " #ProcedureName "(" p48_PassedName ": ...)")

#define np49(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38, \
  p39_PassedName, p39, \
  p40_PassedName, p40, \
  p41_PassedName, p41, \
  p42_PassedName, p42, \
  p43_PassedName, p43, \
  p44_PassedName, p44, \
  p45_PassedName, p45, \
  p46_PassedName, p46, \
  p47_PassedName, p47, \
  p48_PassedName, p48, \
  p49_PassedName, p49 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)"); \
  static_assert(std::string_view(p26_PassedName) == NP_STRINGIFY(_np26_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np26_##ProcedureName) ": ...), got: " #ProcedureName "(" p26_PassedName ": ...)"); \
  static_assert(std::string_view(p27_PassedName) == NP_STRINGIFY(_np27_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np27_##ProcedureName) ": ...), got: " #ProcedureName "(" p27_PassedName ": ...)"); \
  static_assert(std::string_view(p28_PassedName) == NP_STRINGIFY(_np28_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np28_##ProcedureName) ": ...), got: " #ProcedureName "(" p28_PassedName ": ...)"); \
  static_assert(std::string_view(p29_PassedName) == NP_STRINGIFY(_np29_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np29_##ProcedureName) ": ...), got: " #ProcedureName "(" p29_PassedName ": ...)"); \
  static_assert(std::string_view(p30_PassedName) == NP_STRINGIFY(_np30_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np30_##ProcedureName) ": ...), got: " #ProcedureName "(" p30_PassedName ": ...)"); \
  static_assert(std::string_view(p31_PassedName) == NP_STRINGIFY(_np31_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np31_##ProcedureName) ": ...), got: " #ProcedureName "(" p31_PassedName ": ...)"); \
  static_assert(std::string_view(p32_PassedName) == NP_STRINGIFY(_np32_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np32_##ProcedureName) ": ...), got: " #ProcedureName "(" p32_PassedName ": ...)"); \
  static_assert(std::string_view(p33_PassedName) == NP_STRINGIFY(_np33_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np33_##ProcedureName) ": ...), got: " #ProcedureName "(" p33_PassedName ": ...)"); \
  static_assert(std::string_view(p34_PassedName) == NP_STRINGIFY(_np34_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np34_##ProcedureName) ": ...), got: " #ProcedureName "(" p34_PassedName ": ...)"); \
  static_assert(std::string_view(p35_PassedName) == NP_STRINGIFY(_np35_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np35_##ProcedureName) ": ...), got: " #ProcedureName "(" p35_PassedName ": ...)"); \
  static_assert(std::string_view(p36_PassedName) == NP_STRINGIFY(_np36_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np36_##ProcedureName) ": ...), got: " #ProcedureName "(" p36_PassedName ": ...)"); \
  static_assert(std::string_view(p37_PassedName) == NP_STRINGIFY(_np37_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np37_##ProcedureName) ": ...), got: " #ProcedureName "(" p37_PassedName ": ...)"); \
  static_assert(std::string_view(p38_PassedName) == NP_STRINGIFY(_np38_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np38_##ProcedureName) ": ...), got: " #ProcedureName "(" p38_PassedName ": ...)"); \
  static_assert(std::string_view(p39_PassedName) == NP_STRINGIFY(_np39_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np39_##ProcedureName) ": ...), got: " #ProcedureName "(" p39_PassedName ": ...)"); \
  static_assert(std::string_view(p40_PassedName) == NP_STRINGIFY(_np40_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np40_##ProcedureName) ": ...), got: " #ProcedureName "(" p40_PassedName ": ...)"); \
  static_assert(std::string_view(p41_PassedName) == NP_STRINGIFY(_np41_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np41_##ProcedureName) ": ...), got: " #ProcedureName "(" p41_PassedName ": ...)"); \
  static_assert(std::string_view(p42_PassedName) == NP_STRINGIFY(_np42_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np42_##ProcedureName) ": ...), got: " #ProcedureName "(" p42_PassedName ": ...)"); \
  static_assert(std::string_view(p43_PassedName) == NP_STRINGIFY(_np43_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np43_##ProcedureName) ": ...), got: " #ProcedureName "(" p43_PassedName ": ...)"); \
  static_assert(std::string_view(p44_PassedName) == NP_STRINGIFY(_np44_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np44_##ProcedureName) ": ...), got: " #ProcedureName "(" p44_PassedName ": ...)"); \
  static_assert(std::string_view(p45_PassedName) == NP_STRINGIFY(_np45_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np45_##ProcedureName) ": ...), got: " #ProcedureName "(" p45_PassedName ": ...)"); \
  static_assert(std::string_view(p46_PassedName) == NP_STRINGIFY(_np46_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np46_##ProcedureName) ": ...), got: " #ProcedureName "(" p46_PassedName ": ...)"); \
  static_assert(std::string_view(p47_PassedName) == NP_STRINGIFY(_np47_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np47_##ProcedureName) ": ...), got: " #ProcedureName "(" p47_PassedName ": ...)"); \
  static_assert(std::string_view(p48_PassedName) == NP_STRINGIFY(_np48_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np48_##ProcedureName) ": ...), got: " #ProcedureName "(" p48_PassedName ": ...)"); \
  static_assert(std::string_view(p49_PassedName) == NP_STRINGIFY(_np49_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np49_##ProcedureName) ": ...), got: " #ProcedureName "(" p49_PassedName ": ...)")

#define np50(ProcedureName, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38, \
  p39_PassedName, p39, \
  p40_PassedName, p40, \
  p41_PassedName, p41, \
  p42_PassedName, p42, \
  p43_PassedName, p43, \
  p44_PassedName, p44, \
  p45_PassedName, p45, \
  p46_PassedName, p46, \
  p47_PassedName, p47, \
  p48_PassedName, p48, \
  p49_PassedName, p49, \
  p50_PassedName, p50 \
) \
  ProcedureName(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)"); \
  static_assert(std::string_view(p26_PassedName) == NP_STRINGIFY(_np26_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np26_##ProcedureName) ": ...), got: " #ProcedureName "(" p26_PassedName ": ...)"); \
  static_assert(std::string_view(p27_PassedName) == NP_STRINGIFY(_np27_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np27_##ProcedureName) ": ...), got: " #ProcedureName "(" p27_PassedName ": ...)"); \
  static_assert(std::string_view(p28_PassedName) == NP_STRINGIFY(_np28_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np28_##ProcedureName) ": ...), got: " #ProcedureName "(" p28_PassedName ": ...)"); \
  static_assert(std::string_view(p29_PassedName) == NP_STRINGIFY(_np29_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np29_##ProcedureName) ": ...), got: " #ProcedureName "(" p29_PassedName ": ...)"); \
  static_assert(std::string_view(p30_PassedName) == NP_STRINGIFY(_np30_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np30_##ProcedureName) ": ...), got: " #ProcedureName "(" p30_PassedName ": ...)"); \
  static_assert(std::string_view(p31_PassedName) == NP_STRINGIFY(_np31_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np31_##ProcedureName) ": ...), got: " #ProcedureName "(" p31_PassedName ": ...)"); \
  static_assert(std::string_view(p32_PassedName) == NP_STRINGIFY(_np32_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np32_##ProcedureName) ": ...), got: " #ProcedureName "(" p32_PassedName ": ...)"); \
  static_assert(std::string_view(p33_PassedName) == NP_STRINGIFY(_np33_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np33_##ProcedureName) ": ...), got: " #ProcedureName "(" p33_PassedName ": ...)"); \
  static_assert(std::string_view(p34_PassedName) == NP_STRINGIFY(_np34_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np34_##ProcedureName) ": ...), got: " #ProcedureName "(" p34_PassedName ": ...)"); \
  static_assert(std::string_view(p35_PassedName) == NP_STRINGIFY(_np35_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np35_##ProcedureName) ": ...), got: " #ProcedureName "(" p35_PassedName ": ...)"); \
  static_assert(std::string_view(p36_PassedName) == NP_STRINGIFY(_np36_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np36_##ProcedureName) ": ...), got: " #ProcedureName "(" p36_PassedName ": ...)"); \
  static_assert(std::string_view(p37_PassedName) == NP_STRINGIFY(_np37_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np37_##ProcedureName) ": ...), got: " #ProcedureName "(" p37_PassedName ": ...)"); \
  static_assert(std::string_view(p38_PassedName) == NP_STRINGIFY(_np38_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np38_##ProcedureName) ": ...), got: " #ProcedureName "(" p38_PassedName ": ...)"); \
  static_assert(std::string_view(p39_PassedName) == NP_STRINGIFY(_np39_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np39_##ProcedureName) ": ...), got: " #ProcedureName "(" p39_PassedName ": ...)"); \
  static_assert(std::string_view(p40_PassedName) == NP_STRINGIFY(_np40_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np40_##ProcedureName) ": ...), got: " #ProcedureName "(" p40_PassedName ": ...)"); \
  static_assert(std::string_view(p41_PassedName) == NP_STRINGIFY(_np41_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np41_##ProcedureName) ": ...), got: " #ProcedureName "(" p41_PassedName ": ...)"); \
  static_assert(std::string_view(p42_PassedName) == NP_STRINGIFY(_np42_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np42_##ProcedureName) ": ...), got: " #ProcedureName "(" p42_PassedName ": ...)"); \
  static_assert(std::string_view(p43_PassedName) == NP_STRINGIFY(_np43_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np43_##ProcedureName) ": ...), got: " #ProcedureName "(" p43_PassedName ": ...)"); \
  static_assert(std::string_view(p44_PassedName) == NP_STRINGIFY(_np44_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np44_##ProcedureName) ": ...), got: " #ProcedureName "(" p44_PassedName ": ...)"); \
  static_assert(std::string_view(p45_PassedName) == NP_STRINGIFY(_np45_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np45_##ProcedureName) ": ...), got: " #ProcedureName "(" p45_PassedName ": ...)"); \
  static_assert(std::string_view(p46_PassedName) == NP_STRINGIFY(_np46_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np46_##ProcedureName) ": ...), got: " #ProcedureName "(" p46_PassedName ": ...)"); \
  static_assert(std::string_view(p47_PassedName) == NP_STRINGIFY(_np47_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np47_##ProcedureName) ": ...), got: " #ProcedureName "(" p47_PassedName ": ...)"); \
  static_assert(std::string_view(p48_PassedName) == NP_STRINGIFY(_np48_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np48_##ProcedureName) ": ...), got: " #ProcedureName "(" p48_PassedName ": ...)"); \
  static_assert(std::string_view(p49_PassedName) == NP_STRINGIFY(_np49_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np49_##ProcedureName) ": ...), got: " #ProcedureName "(" p49_PassedName ": ...)"); \
  static_assert(std::string_view(p50_PassedName) == NP_STRINGIFY(_np50_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np50_##ProcedureName) ": ...), got: " #ProcedureName "(" p50_PassedName ": ...)")

#define npfp0(ProcedureName, ProcedureCall) ProcedureCall()

#define npfp1(ProcedureName, ProcedureCall, \
  p1_PassedName, p1 \
) \
  ProcedureCall(p1); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)")

#define npfp2(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2 \
) \
  ProcedureCall(p1, p2); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)")

#define npfp3(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3 \
) \
  ProcedureCall(p1, p2, p3); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)")

#define npfp4(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4 \
) \
  ProcedureCall(p1, p2, p3, p4); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)")

#define npfp5(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5 \
) \
  ProcedureCall(p1, p2, p3, p4, p5); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)")

#define npfp6(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)")

#define npfp7(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)")

#define npfp8(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)")

#define npfp9(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)")

#define npfp10(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)")

#define npfp11(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)")

#define npfp12(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)")

#define npfp13(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)")

#define npfp14(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)")

#define npfp15(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)")

#define npfp16(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)")

#define npfp17(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)")

#define npfp18(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)")

#define npfp19(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)")

#define npfp20(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)")

#define npfp21(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)")

#define npfp22(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)")

#define npfp23(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)")

#define npfp24(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)")

#define npfp25(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)")

#define npfp26(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)"); \
  static_assert(std::string_view(p26_PassedName) == NP_STRINGIFY(_np26_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np26_##ProcedureName) ": ...), got: " #ProcedureName "(" p26_PassedName ": ...)")

#define npfp27(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)"); \
  static_assert(std::string_view(p26_PassedName) == NP_STRINGIFY(_np26_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np26_##ProcedureName) ": ...), got: " #ProcedureName "(" p26_PassedName ": ...)"); \
  static_assert(std::string_view(p27_PassedName) == NP_STRINGIFY(_np27_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np27_##ProcedureName) ": ...), got: " #ProcedureName "(" p27_PassedName ": ...)")

#define npfp28(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)"); \
  static_assert(std::string_view(p26_PassedName) == NP_STRINGIFY(_np26_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np26_##ProcedureName) ": ...), got: " #ProcedureName "(" p26_PassedName ": ...)"); \
  static_assert(std::string_view(p27_PassedName) == NP_STRINGIFY(_np27_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np27_##ProcedureName) ": ...), got: " #ProcedureName "(" p27_PassedName ": ...)"); \
  static_assert(std::string_view(p28_PassedName) == NP_STRINGIFY(_np28_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np28_##ProcedureName) ": ...), got: " #ProcedureName "(" p28_PassedName ": ...)")

#define npfp29(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)"); \
  static_assert(std::string_view(p26_PassedName) == NP_STRINGIFY(_np26_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np26_##ProcedureName) ": ...), got: " #ProcedureName "(" p26_PassedName ": ...)"); \
  static_assert(std::string_view(p27_PassedName) == NP_STRINGIFY(_np27_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np27_##ProcedureName) ": ...), got: " #ProcedureName "(" p27_PassedName ": ...)"); \
  static_assert(std::string_view(p28_PassedName) == NP_STRINGIFY(_np28_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np28_##ProcedureName) ": ...), got: " #ProcedureName "(" p28_PassedName ": ...)"); \
  static_assert(std::string_view(p29_PassedName) == NP_STRINGIFY(_np29_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np29_##ProcedureName) ": ...), got: " #ProcedureName "(" p29_PassedName ": ...)")

#define npfp30(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)"); \
  static_assert(std::string_view(p26_PassedName) == NP_STRINGIFY(_np26_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np26_##ProcedureName) ": ...), got: " #ProcedureName "(" p26_PassedName ": ...)"); \
  static_assert(std::string_view(p27_PassedName) == NP_STRINGIFY(_np27_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np27_##ProcedureName) ": ...), got: " #ProcedureName "(" p27_PassedName ": ...)"); \
  static_assert(std::string_view(p28_PassedName) == NP_STRINGIFY(_np28_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np28_##ProcedureName) ": ...), got: " #ProcedureName "(" p28_PassedName ": ...)"); \
  static_assert(std::string_view(p29_PassedName) == NP_STRINGIFY(_np29_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np29_##ProcedureName) ": ...), got: " #ProcedureName "(" p29_PassedName ": ...)"); \
  static_assert(std::string_view(p30_PassedName) == NP_STRINGIFY(_np30_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np30_##ProcedureName) ": ...), got: " #ProcedureName "(" p30_PassedName ": ...)")

#define npfp31(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)"); \
  static_assert(std::string_view(p26_PassedName) == NP_STRINGIFY(_np26_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np26_##ProcedureName) ": ...), got: " #ProcedureName "(" p26_PassedName ": ...)"); \
  static_assert(std::string_view(p27_PassedName) == NP_STRINGIFY(_np27_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np27_##ProcedureName) ": ...), got: " #ProcedureName "(" p27_PassedName ": ...)"); \
  static_assert(std::string_view(p28_PassedName) == NP_STRINGIFY(_np28_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np28_##ProcedureName) ": ...), got: " #ProcedureName "(" p28_PassedName ": ...)"); \
  static_assert(std::string_view(p29_PassedName) == NP_STRINGIFY(_np29_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np29_##ProcedureName) ": ...), got: " #ProcedureName "(" p29_PassedName ": ...)"); \
  static_assert(std::string_view(p30_PassedName) == NP_STRINGIFY(_np30_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np30_##ProcedureName) ": ...), got: " #ProcedureName "(" p30_PassedName ": ...)"); \
  static_assert(std::string_view(p31_PassedName) == NP_STRINGIFY(_np31_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np31_##ProcedureName) ": ...), got: " #ProcedureName "(" p31_PassedName ": ...)")

#define npfp32(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)"); \
  static_assert(std::string_view(p26_PassedName) == NP_STRINGIFY(_np26_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np26_##ProcedureName) ": ...), got: " #ProcedureName "(" p26_PassedName ": ...)"); \
  static_assert(std::string_view(p27_PassedName) == NP_STRINGIFY(_np27_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np27_##ProcedureName) ": ...), got: " #ProcedureName "(" p27_PassedName ": ...)"); \
  static_assert(std::string_view(p28_PassedName) == NP_STRINGIFY(_np28_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np28_##ProcedureName) ": ...), got: " #ProcedureName "(" p28_PassedName ": ...)"); \
  static_assert(std::string_view(p29_PassedName) == NP_STRINGIFY(_np29_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np29_##ProcedureName) ": ...), got: " #ProcedureName "(" p29_PassedName ": ...)"); \
  static_assert(std::string_view(p30_PassedName) == NP_STRINGIFY(_np30_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np30_##ProcedureName) ": ...), got: " #ProcedureName "(" p30_PassedName ": ...)"); \
  static_assert(std::string_view(p31_PassedName) == NP_STRINGIFY(_np31_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np31_##ProcedureName) ": ...), got: " #ProcedureName "(" p31_PassedName ": ...)"); \
  static_assert(std::string_view(p32_PassedName) == NP_STRINGIFY(_np32_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np32_##ProcedureName) ": ...), got: " #ProcedureName "(" p32_PassedName ": ...)")

#define npfp33(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)"); \
  static_assert(std::string_view(p26_PassedName) == NP_STRINGIFY(_np26_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np26_##ProcedureName) ": ...), got: " #ProcedureName "(" p26_PassedName ": ...)"); \
  static_assert(std::string_view(p27_PassedName) == NP_STRINGIFY(_np27_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np27_##ProcedureName) ": ...), got: " #ProcedureName "(" p27_PassedName ": ...)"); \
  static_assert(std::string_view(p28_PassedName) == NP_STRINGIFY(_np28_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np28_##ProcedureName) ": ...), got: " #ProcedureName "(" p28_PassedName ": ...)"); \
  static_assert(std::string_view(p29_PassedName) == NP_STRINGIFY(_np29_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np29_##ProcedureName) ": ...), got: " #ProcedureName "(" p29_PassedName ": ...)"); \
  static_assert(std::string_view(p30_PassedName) == NP_STRINGIFY(_np30_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np30_##ProcedureName) ": ...), got: " #ProcedureName "(" p30_PassedName ": ...)"); \
  static_assert(std::string_view(p31_PassedName) == NP_STRINGIFY(_np31_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np31_##ProcedureName) ": ...), got: " #ProcedureName "(" p31_PassedName ": ...)"); \
  static_assert(std::string_view(p32_PassedName) == NP_STRINGIFY(_np32_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np32_##ProcedureName) ": ...), got: " #ProcedureName "(" p32_PassedName ": ...)"); \
  static_assert(std::string_view(p33_PassedName) == NP_STRINGIFY(_np33_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np33_##ProcedureName) ": ...), got: " #ProcedureName "(" p33_PassedName ": ...)")

#define npfp34(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)"); \
  static_assert(std::string_view(p26_PassedName) == NP_STRINGIFY(_np26_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np26_##ProcedureName) ": ...), got: " #ProcedureName "(" p26_PassedName ": ...)"); \
  static_assert(std::string_view(p27_PassedName) == NP_STRINGIFY(_np27_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np27_##ProcedureName) ": ...), got: " #ProcedureName "(" p27_PassedName ": ...)"); \
  static_assert(std::string_view(p28_PassedName) == NP_STRINGIFY(_np28_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np28_##ProcedureName) ": ...), got: " #ProcedureName "(" p28_PassedName ": ...)"); \
  static_assert(std::string_view(p29_PassedName) == NP_STRINGIFY(_np29_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np29_##ProcedureName) ": ...), got: " #ProcedureName "(" p29_PassedName ": ...)"); \
  static_assert(std::string_view(p30_PassedName) == NP_STRINGIFY(_np30_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np30_##ProcedureName) ": ...), got: " #ProcedureName "(" p30_PassedName ": ...)"); \
  static_assert(std::string_view(p31_PassedName) == NP_STRINGIFY(_np31_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np31_##ProcedureName) ": ...), got: " #ProcedureName "(" p31_PassedName ": ...)"); \
  static_assert(std::string_view(p32_PassedName) == NP_STRINGIFY(_np32_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np32_##ProcedureName) ": ...), got: " #ProcedureName "(" p32_PassedName ": ...)"); \
  static_assert(std::string_view(p33_PassedName) == NP_STRINGIFY(_np33_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np33_##ProcedureName) ": ...), got: " #ProcedureName "(" p33_PassedName ": ...)"); \
  static_assert(std::string_view(p34_PassedName) == NP_STRINGIFY(_np34_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np34_##ProcedureName) ": ...), got: " #ProcedureName "(" p34_PassedName ": ...)")

#define npfp35(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)"); \
  static_assert(std::string_view(p26_PassedName) == NP_STRINGIFY(_np26_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np26_##ProcedureName) ": ...), got: " #ProcedureName "(" p26_PassedName ": ...)"); \
  static_assert(std::string_view(p27_PassedName) == NP_STRINGIFY(_np27_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np27_##ProcedureName) ": ...), got: " #ProcedureName "(" p27_PassedName ": ...)"); \
  static_assert(std::string_view(p28_PassedName) == NP_STRINGIFY(_np28_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np28_##ProcedureName) ": ...), got: " #ProcedureName "(" p28_PassedName ": ...)"); \
  static_assert(std::string_view(p29_PassedName) == NP_STRINGIFY(_np29_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np29_##ProcedureName) ": ...), got: " #ProcedureName "(" p29_PassedName ": ...)"); \
  static_assert(std::string_view(p30_PassedName) == NP_STRINGIFY(_np30_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np30_##ProcedureName) ": ...), got: " #ProcedureName "(" p30_PassedName ": ...)"); \
  static_assert(std::string_view(p31_PassedName) == NP_STRINGIFY(_np31_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np31_##ProcedureName) ": ...), got: " #ProcedureName "(" p31_PassedName ": ...)"); \
  static_assert(std::string_view(p32_PassedName) == NP_STRINGIFY(_np32_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np32_##ProcedureName) ": ...), got: " #ProcedureName "(" p32_PassedName ": ...)"); \
  static_assert(std::string_view(p33_PassedName) == NP_STRINGIFY(_np33_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np33_##ProcedureName) ": ...), got: " #ProcedureName "(" p33_PassedName ": ...)"); \
  static_assert(std::string_view(p34_PassedName) == NP_STRINGIFY(_np34_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np34_##ProcedureName) ": ...), got: " #ProcedureName "(" p34_PassedName ": ...)"); \
  static_assert(std::string_view(p35_PassedName) == NP_STRINGIFY(_np35_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np35_##ProcedureName) ": ...), got: " #ProcedureName "(" p35_PassedName ": ...)")

#define npfp36(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)"); \
  static_assert(std::string_view(p26_PassedName) == NP_STRINGIFY(_np26_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np26_##ProcedureName) ": ...), got: " #ProcedureName "(" p26_PassedName ": ...)"); \
  static_assert(std::string_view(p27_PassedName) == NP_STRINGIFY(_np27_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np27_##ProcedureName) ": ...), got: " #ProcedureName "(" p27_PassedName ": ...)"); \
  static_assert(std::string_view(p28_PassedName) == NP_STRINGIFY(_np28_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np28_##ProcedureName) ": ...), got: " #ProcedureName "(" p28_PassedName ": ...)"); \
  static_assert(std::string_view(p29_PassedName) == NP_STRINGIFY(_np29_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np29_##ProcedureName) ": ...), got: " #ProcedureName "(" p29_PassedName ": ...)"); \
  static_assert(std::string_view(p30_PassedName) == NP_STRINGIFY(_np30_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np30_##ProcedureName) ": ...), got: " #ProcedureName "(" p30_PassedName ": ...)"); \
  static_assert(std::string_view(p31_PassedName) == NP_STRINGIFY(_np31_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np31_##ProcedureName) ": ...), got: " #ProcedureName "(" p31_PassedName ": ...)"); \
  static_assert(std::string_view(p32_PassedName) == NP_STRINGIFY(_np32_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np32_##ProcedureName) ": ...), got: " #ProcedureName "(" p32_PassedName ": ...)"); \
  static_assert(std::string_view(p33_PassedName) == NP_STRINGIFY(_np33_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np33_##ProcedureName) ": ...), got: " #ProcedureName "(" p33_PassedName ": ...)"); \
  static_assert(std::string_view(p34_PassedName) == NP_STRINGIFY(_np34_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np34_##ProcedureName) ": ...), got: " #ProcedureName "(" p34_PassedName ": ...)"); \
  static_assert(std::string_view(p35_PassedName) == NP_STRINGIFY(_np35_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np35_##ProcedureName) ": ...), got: " #ProcedureName "(" p35_PassedName ": ...)"); \
  static_assert(std::string_view(p36_PassedName) == NP_STRINGIFY(_np36_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np36_##ProcedureName) ": ...), got: " #ProcedureName "(" p36_PassedName ": ...)")

#define npfp37(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)"); \
  static_assert(std::string_view(p26_PassedName) == NP_STRINGIFY(_np26_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np26_##ProcedureName) ": ...), got: " #ProcedureName "(" p26_PassedName ": ...)"); \
  static_assert(std::string_view(p27_PassedName) == NP_STRINGIFY(_np27_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np27_##ProcedureName) ": ...), got: " #ProcedureName "(" p27_PassedName ": ...)"); \
  static_assert(std::string_view(p28_PassedName) == NP_STRINGIFY(_np28_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np28_##ProcedureName) ": ...), got: " #ProcedureName "(" p28_PassedName ": ...)"); \
  static_assert(std::string_view(p29_PassedName) == NP_STRINGIFY(_np29_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np29_##ProcedureName) ": ...), got: " #ProcedureName "(" p29_PassedName ": ...)"); \
  static_assert(std::string_view(p30_PassedName) == NP_STRINGIFY(_np30_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np30_##ProcedureName) ": ...), got: " #ProcedureName "(" p30_PassedName ": ...)"); \
  static_assert(std::string_view(p31_PassedName) == NP_STRINGIFY(_np31_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np31_##ProcedureName) ": ...), got: " #ProcedureName "(" p31_PassedName ": ...)"); \
  static_assert(std::string_view(p32_PassedName) == NP_STRINGIFY(_np32_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np32_##ProcedureName) ": ...), got: " #ProcedureName "(" p32_PassedName ": ...)"); \
  static_assert(std::string_view(p33_PassedName) == NP_STRINGIFY(_np33_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np33_##ProcedureName) ": ...), got: " #ProcedureName "(" p33_PassedName ": ...)"); \
  static_assert(std::string_view(p34_PassedName) == NP_STRINGIFY(_np34_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np34_##ProcedureName) ": ...), got: " #ProcedureName "(" p34_PassedName ": ...)"); \
  static_assert(std::string_view(p35_PassedName) == NP_STRINGIFY(_np35_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np35_##ProcedureName) ": ...), got: " #ProcedureName "(" p35_PassedName ": ...)"); \
  static_assert(std::string_view(p36_PassedName) == NP_STRINGIFY(_np36_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np36_##ProcedureName) ": ...), got: " #ProcedureName "(" p36_PassedName ": ...)"); \
  static_assert(std::string_view(p37_PassedName) == NP_STRINGIFY(_np37_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np37_##ProcedureName) ": ...), got: " #ProcedureName "(" p37_PassedName ": ...)")

#define npfp38(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)"); \
  static_assert(std::string_view(p26_PassedName) == NP_STRINGIFY(_np26_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np26_##ProcedureName) ": ...), got: " #ProcedureName "(" p26_PassedName ": ...)"); \
  static_assert(std::string_view(p27_PassedName) == NP_STRINGIFY(_np27_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np27_##ProcedureName) ": ...), got: " #ProcedureName "(" p27_PassedName ": ...)"); \
  static_assert(std::string_view(p28_PassedName) == NP_STRINGIFY(_np28_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np28_##ProcedureName) ": ...), got: " #ProcedureName "(" p28_PassedName ": ...)"); \
  static_assert(std::string_view(p29_PassedName) == NP_STRINGIFY(_np29_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np29_##ProcedureName) ": ...), got: " #ProcedureName "(" p29_PassedName ": ...)"); \
  static_assert(std::string_view(p30_PassedName) == NP_STRINGIFY(_np30_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np30_##ProcedureName) ": ...), got: " #ProcedureName "(" p30_PassedName ": ...)"); \
  static_assert(std::string_view(p31_PassedName) == NP_STRINGIFY(_np31_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np31_##ProcedureName) ": ...), got: " #ProcedureName "(" p31_PassedName ": ...)"); \
  static_assert(std::string_view(p32_PassedName) == NP_STRINGIFY(_np32_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np32_##ProcedureName) ": ...), got: " #ProcedureName "(" p32_PassedName ": ...)"); \
  static_assert(std::string_view(p33_PassedName) == NP_STRINGIFY(_np33_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np33_##ProcedureName) ": ...), got: " #ProcedureName "(" p33_PassedName ": ...)"); \
  static_assert(std::string_view(p34_PassedName) == NP_STRINGIFY(_np34_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np34_##ProcedureName) ": ...), got: " #ProcedureName "(" p34_PassedName ": ...)"); \
  static_assert(std::string_view(p35_PassedName) == NP_STRINGIFY(_np35_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np35_##ProcedureName) ": ...), got: " #ProcedureName "(" p35_PassedName ": ...)"); \
  static_assert(std::string_view(p36_PassedName) == NP_STRINGIFY(_np36_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np36_##ProcedureName) ": ...), got: " #ProcedureName "(" p36_PassedName ": ...)"); \
  static_assert(std::string_view(p37_PassedName) == NP_STRINGIFY(_np37_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np37_##ProcedureName) ": ...), got: " #ProcedureName "(" p37_PassedName ": ...)"); \
  static_assert(std::string_view(p38_PassedName) == NP_STRINGIFY(_np38_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np38_##ProcedureName) ": ...), got: " #ProcedureName "(" p38_PassedName ": ...)")

#define npfp39(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38, \
  p39_PassedName, p39 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)"); \
  static_assert(std::string_view(p26_PassedName) == NP_STRINGIFY(_np26_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np26_##ProcedureName) ": ...), got: " #ProcedureName "(" p26_PassedName ": ...)"); \
  static_assert(std::string_view(p27_PassedName) == NP_STRINGIFY(_np27_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np27_##ProcedureName) ": ...), got: " #ProcedureName "(" p27_PassedName ": ...)"); \
  static_assert(std::string_view(p28_PassedName) == NP_STRINGIFY(_np28_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np28_##ProcedureName) ": ...), got: " #ProcedureName "(" p28_PassedName ": ...)"); \
  static_assert(std::string_view(p29_PassedName) == NP_STRINGIFY(_np29_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np29_##ProcedureName) ": ...), got: " #ProcedureName "(" p29_PassedName ": ...)"); \
  static_assert(std::string_view(p30_PassedName) == NP_STRINGIFY(_np30_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np30_##ProcedureName) ": ...), got: " #ProcedureName "(" p30_PassedName ": ...)"); \
  static_assert(std::string_view(p31_PassedName) == NP_STRINGIFY(_np31_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np31_##ProcedureName) ": ...), got: " #ProcedureName "(" p31_PassedName ": ...)"); \
  static_assert(std::string_view(p32_PassedName) == NP_STRINGIFY(_np32_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np32_##ProcedureName) ": ...), got: " #ProcedureName "(" p32_PassedName ": ...)"); \
  static_assert(std::string_view(p33_PassedName) == NP_STRINGIFY(_np33_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np33_##ProcedureName) ": ...), got: " #ProcedureName "(" p33_PassedName ": ...)"); \
  static_assert(std::string_view(p34_PassedName) == NP_STRINGIFY(_np34_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np34_##ProcedureName) ": ...), got: " #ProcedureName "(" p34_PassedName ": ...)"); \
  static_assert(std::string_view(p35_PassedName) == NP_STRINGIFY(_np35_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np35_##ProcedureName) ": ...), got: " #ProcedureName "(" p35_PassedName ": ...)"); \
  static_assert(std::string_view(p36_PassedName) == NP_STRINGIFY(_np36_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np36_##ProcedureName) ": ...), got: " #ProcedureName "(" p36_PassedName ": ...)"); \
  static_assert(std::string_view(p37_PassedName) == NP_STRINGIFY(_np37_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np37_##ProcedureName) ": ...), got: " #ProcedureName "(" p37_PassedName ": ...)"); \
  static_assert(std::string_view(p38_PassedName) == NP_STRINGIFY(_np38_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np38_##ProcedureName) ": ...), got: " #ProcedureName "(" p38_PassedName ": ...)"); \
  static_assert(std::string_view(p39_PassedName) == NP_STRINGIFY(_np39_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np39_##ProcedureName) ": ...), got: " #ProcedureName "(" p39_PassedName ": ...)")

#define npfp40(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38, \
  p39_PassedName, p39, \
  p40_PassedName, p40 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)"); \
  static_assert(std::string_view(p26_PassedName) == NP_STRINGIFY(_np26_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np26_##ProcedureName) ": ...), got: " #ProcedureName "(" p26_PassedName ": ...)"); \
  static_assert(std::string_view(p27_PassedName) == NP_STRINGIFY(_np27_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np27_##ProcedureName) ": ...), got: " #ProcedureName "(" p27_PassedName ": ...)"); \
  static_assert(std::string_view(p28_PassedName) == NP_STRINGIFY(_np28_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np28_##ProcedureName) ": ...), got: " #ProcedureName "(" p28_PassedName ": ...)"); \
  static_assert(std::string_view(p29_PassedName) == NP_STRINGIFY(_np29_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np29_##ProcedureName) ": ...), got: " #ProcedureName "(" p29_PassedName ": ...)"); \
  static_assert(std::string_view(p30_PassedName) == NP_STRINGIFY(_np30_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np30_##ProcedureName) ": ...), got: " #ProcedureName "(" p30_PassedName ": ...)"); \
  static_assert(std::string_view(p31_PassedName) == NP_STRINGIFY(_np31_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np31_##ProcedureName) ": ...), got: " #ProcedureName "(" p31_PassedName ": ...)"); \
  static_assert(std::string_view(p32_PassedName) == NP_STRINGIFY(_np32_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np32_##ProcedureName) ": ...), got: " #ProcedureName "(" p32_PassedName ": ...)"); \
  static_assert(std::string_view(p33_PassedName) == NP_STRINGIFY(_np33_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np33_##ProcedureName) ": ...), got: " #ProcedureName "(" p33_PassedName ": ...)"); \
  static_assert(std::string_view(p34_PassedName) == NP_STRINGIFY(_np34_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np34_##ProcedureName) ": ...), got: " #ProcedureName "(" p34_PassedName ": ...)"); \
  static_assert(std::string_view(p35_PassedName) == NP_STRINGIFY(_np35_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np35_##ProcedureName) ": ...), got: " #ProcedureName "(" p35_PassedName ": ...)"); \
  static_assert(std::string_view(p36_PassedName) == NP_STRINGIFY(_np36_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np36_##ProcedureName) ": ...), got: " #ProcedureName "(" p36_PassedName ": ...)"); \
  static_assert(std::string_view(p37_PassedName) == NP_STRINGIFY(_np37_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np37_##ProcedureName) ": ...), got: " #ProcedureName "(" p37_PassedName ": ...)"); \
  static_assert(std::string_view(p38_PassedName) == NP_STRINGIFY(_np38_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np38_##ProcedureName) ": ...), got: " #ProcedureName "(" p38_PassedName ": ...)"); \
  static_assert(std::string_view(p39_PassedName) == NP_STRINGIFY(_np39_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np39_##ProcedureName) ": ...), got: " #ProcedureName "(" p39_PassedName ": ...)"); \
  static_assert(std::string_view(p40_PassedName) == NP_STRINGIFY(_np40_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np40_##ProcedureName) ": ...), got: " #ProcedureName "(" p40_PassedName ": ...)")

#define npfp41(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38, \
  p39_PassedName, p39, \
  p40_PassedName, p40, \
  p41_PassedName, p41 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)"); \
  static_assert(std::string_view(p26_PassedName) == NP_STRINGIFY(_np26_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np26_##ProcedureName) ": ...), got: " #ProcedureName "(" p26_PassedName ": ...)"); \
  static_assert(std::string_view(p27_PassedName) == NP_STRINGIFY(_np27_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np27_##ProcedureName) ": ...), got: " #ProcedureName "(" p27_PassedName ": ...)"); \
  static_assert(std::string_view(p28_PassedName) == NP_STRINGIFY(_np28_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np28_##ProcedureName) ": ...), got: " #ProcedureName "(" p28_PassedName ": ...)"); \
  static_assert(std::string_view(p29_PassedName) == NP_STRINGIFY(_np29_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np29_##ProcedureName) ": ...), got: " #ProcedureName "(" p29_PassedName ": ...)"); \
  static_assert(std::string_view(p30_PassedName) == NP_STRINGIFY(_np30_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np30_##ProcedureName) ": ...), got: " #ProcedureName "(" p30_PassedName ": ...)"); \
  static_assert(std::string_view(p31_PassedName) == NP_STRINGIFY(_np31_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np31_##ProcedureName) ": ...), got: " #ProcedureName "(" p31_PassedName ": ...)"); \
  static_assert(std::string_view(p32_PassedName) == NP_STRINGIFY(_np32_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np32_##ProcedureName) ": ...), got: " #ProcedureName "(" p32_PassedName ": ...)"); \
  static_assert(std::string_view(p33_PassedName) == NP_STRINGIFY(_np33_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np33_##ProcedureName) ": ...), got: " #ProcedureName "(" p33_PassedName ": ...)"); \
  static_assert(std::string_view(p34_PassedName) == NP_STRINGIFY(_np34_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np34_##ProcedureName) ": ...), got: " #ProcedureName "(" p34_PassedName ": ...)"); \
  static_assert(std::string_view(p35_PassedName) == NP_STRINGIFY(_np35_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np35_##ProcedureName) ": ...), got: " #ProcedureName "(" p35_PassedName ": ...)"); \
  static_assert(std::string_view(p36_PassedName) == NP_STRINGIFY(_np36_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np36_##ProcedureName) ": ...), got: " #ProcedureName "(" p36_PassedName ": ...)"); \
  static_assert(std::string_view(p37_PassedName) == NP_STRINGIFY(_np37_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np37_##ProcedureName) ": ...), got: " #ProcedureName "(" p37_PassedName ": ...)"); \
  static_assert(std::string_view(p38_PassedName) == NP_STRINGIFY(_np38_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np38_##ProcedureName) ": ...), got: " #ProcedureName "(" p38_PassedName ": ...)"); \
  static_assert(std::string_view(p39_PassedName) == NP_STRINGIFY(_np39_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np39_##ProcedureName) ": ...), got: " #ProcedureName "(" p39_PassedName ": ...)"); \
  static_assert(std::string_view(p40_PassedName) == NP_STRINGIFY(_np40_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np40_##ProcedureName) ": ...), got: " #ProcedureName "(" p40_PassedName ": ...)"); \
  static_assert(std::string_view(p41_PassedName) == NP_STRINGIFY(_np41_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np41_##ProcedureName) ": ...), got: " #ProcedureName "(" p41_PassedName ": ...)")

#define npfp42(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38, \
  p39_PassedName, p39, \
  p40_PassedName, p40, \
  p41_PassedName, p41, \
  p42_PassedName, p42 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)"); \
  static_assert(std::string_view(p26_PassedName) == NP_STRINGIFY(_np26_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np26_##ProcedureName) ": ...), got: " #ProcedureName "(" p26_PassedName ": ...)"); \
  static_assert(std::string_view(p27_PassedName) == NP_STRINGIFY(_np27_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np27_##ProcedureName) ": ...), got: " #ProcedureName "(" p27_PassedName ": ...)"); \
  static_assert(std::string_view(p28_PassedName) == NP_STRINGIFY(_np28_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np28_##ProcedureName) ": ...), got: " #ProcedureName "(" p28_PassedName ": ...)"); \
  static_assert(std::string_view(p29_PassedName) == NP_STRINGIFY(_np29_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np29_##ProcedureName) ": ...), got: " #ProcedureName "(" p29_PassedName ": ...)"); \
  static_assert(std::string_view(p30_PassedName) == NP_STRINGIFY(_np30_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np30_##ProcedureName) ": ...), got: " #ProcedureName "(" p30_PassedName ": ...)"); \
  static_assert(std::string_view(p31_PassedName) == NP_STRINGIFY(_np31_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np31_##ProcedureName) ": ...), got: " #ProcedureName "(" p31_PassedName ": ...)"); \
  static_assert(std::string_view(p32_PassedName) == NP_STRINGIFY(_np32_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np32_##ProcedureName) ": ...), got: " #ProcedureName "(" p32_PassedName ": ...)"); \
  static_assert(std::string_view(p33_PassedName) == NP_STRINGIFY(_np33_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np33_##ProcedureName) ": ...), got: " #ProcedureName "(" p33_PassedName ": ...)"); \
  static_assert(std::string_view(p34_PassedName) == NP_STRINGIFY(_np34_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np34_##ProcedureName) ": ...), got: " #ProcedureName "(" p34_PassedName ": ...)"); \
  static_assert(std::string_view(p35_PassedName) == NP_STRINGIFY(_np35_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np35_##ProcedureName) ": ...), got: " #ProcedureName "(" p35_PassedName ": ...)"); \
  static_assert(std::string_view(p36_PassedName) == NP_STRINGIFY(_np36_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np36_##ProcedureName) ": ...), got: " #ProcedureName "(" p36_PassedName ": ...)"); \
  static_assert(std::string_view(p37_PassedName) == NP_STRINGIFY(_np37_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np37_##ProcedureName) ": ...), got: " #ProcedureName "(" p37_PassedName ": ...)"); \
  static_assert(std::string_view(p38_PassedName) == NP_STRINGIFY(_np38_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np38_##ProcedureName) ": ...), got: " #ProcedureName "(" p38_PassedName ": ...)"); \
  static_assert(std::string_view(p39_PassedName) == NP_STRINGIFY(_np39_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np39_##ProcedureName) ": ...), got: " #ProcedureName "(" p39_PassedName ": ...)"); \
  static_assert(std::string_view(p40_PassedName) == NP_STRINGIFY(_np40_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np40_##ProcedureName) ": ...), got: " #ProcedureName "(" p40_PassedName ": ...)"); \
  static_assert(std::string_view(p41_PassedName) == NP_STRINGIFY(_np41_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np41_##ProcedureName) ": ...), got: " #ProcedureName "(" p41_PassedName ": ...)"); \
  static_assert(std::string_view(p42_PassedName) == NP_STRINGIFY(_np42_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np42_##ProcedureName) ": ...), got: " #ProcedureName "(" p42_PassedName ": ...)")

#define npfp43(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38, \
  p39_PassedName, p39, \
  p40_PassedName, p40, \
  p41_PassedName, p41, \
  p42_PassedName, p42, \
  p43_PassedName, p43 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)"); \
  static_assert(std::string_view(p26_PassedName) == NP_STRINGIFY(_np26_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np26_##ProcedureName) ": ...), got: " #ProcedureName "(" p26_PassedName ": ...)"); \
  static_assert(std::string_view(p27_PassedName) == NP_STRINGIFY(_np27_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np27_##ProcedureName) ": ...), got: " #ProcedureName "(" p27_PassedName ": ...)"); \
  static_assert(std::string_view(p28_PassedName) == NP_STRINGIFY(_np28_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np28_##ProcedureName) ": ...), got: " #ProcedureName "(" p28_PassedName ": ...)"); \
  static_assert(std::string_view(p29_PassedName) == NP_STRINGIFY(_np29_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np29_##ProcedureName) ": ...), got: " #ProcedureName "(" p29_PassedName ": ...)"); \
  static_assert(std::string_view(p30_PassedName) == NP_STRINGIFY(_np30_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np30_##ProcedureName) ": ...), got: " #ProcedureName "(" p30_PassedName ": ...)"); \
  static_assert(std::string_view(p31_PassedName) == NP_STRINGIFY(_np31_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np31_##ProcedureName) ": ...), got: " #ProcedureName "(" p31_PassedName ": ...)"); \
  static_assert(std::string_view(p32_PassedName) == NP_STRINGIFY(_np32_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np32_##ProcedureName) ": ...), got: " #ProcedureName "(" p32_PassedName ": ...)"); \
  static_assert(std::string_view(p33_PassedName) == NP_STRINGIFY(_np33_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np33_##ProcedureName) ": ...), got: " #ProcedureName "(" p33_PassedName ": ...)"); \
  static_assert(std::string_view(p34_PassedName) == NP_STRINGIFY(_np34_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np34_##ProcedureName) ": ...), got: " #ProcedureName "(" p34_PassedName ": ...)"); \
  static_assert(std::string_view(p35_PassedName) == NP_STRINGIFY(_np35_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np35_##ProcedureName) ": ...), got: " #ProcedureName "(" p35_PassedName ": ...)"); \
  static_assert(std::string_view(p36_PassedName) == NP_STRINGIFY(_np36_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np36_##ProcedureName) ": ...), got: " #ProcedureName "(" p36_PassedName ": ...)"); \
  static_assert(std::string_view(p37_PassedName) == NP_STRINGIFY(_np37_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np37_##ProcedureName) ": ...), got: " #ProcedureName "(" p37_PassedName ": ...)"); \
  static_assert(std::string_view(p38_PassedName) == NP_STRINGIFY(_np38_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np38_##ProcedureName) ": ...), got: " #ProcedureName "(" p38_PassedName ": ...)"); \
  static_assert(std::string_view(p39_PassedName) == NP_STRINGIFY(_np39_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np39_##ProcedureName) ": ...), got: " #ProcedureName "(" p39_PassedName ": ...)"); \
  static_assert(std::string_view(p40_PassedName) == NP_STRINGIFY(_np40_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np40_##ProcedureName) ": ...), got: " #ProcedureName "(" p40_PassedName ": ...)"); \
  static_assert(std::string_view(p41_PassedName) == NP_STRINGIFY(_np41_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np41_##ProcedureName) ": ...), got: " #ProcedureName "(" p41_PassedName ": ...)"); \
  static_assert(std::string_view(p42_PassedName) == NP_STRINGIFY(_np42_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np42_##ProcedureName) ": ...), got: " #ProcedureName "(" p42_PassedName ": ...)"); \
  static_assert(std::string_view(p43_PassedName) == NP_STRINGIFY(_np43_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np43_##ProcedureName) ": ...), got: " #ProcedureName "(" p43_PassedName ": ...)")

#define npfp44(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38, \
  p39_PassedName, p39, \
  p40_PassedName, p40, \
  p41_PassedName, p41, \
  p42_PassedName, p42, \
  p43_PassedName, p43, \
  p44_PassedName, p44 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)"); \
  static_assert(std::string_view(p26_PassedName) == NP_STRINGIFY(_np26_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np26_##ProcedureName) ": ...), got: " #ProcedureName "(" p26_PassedName ": ...)"); \
  static_assert(std::string_view(p27_PassedName) == NP_STRINGIFY(_np27_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np27_##ProcedureName) ": ...), got: " #ProcedureName "(" p27_PassedName ": ...)"); \
  static_assert(std::string_view(p28_PassedName) == NP_STRINGIFY(_np28_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np28_##ProcedureName) ": ...), got: " #ProcedureName "(" p28_PassedName ": ...)"); \
  static_assert(std::string_view(p29_PassedName) == NP_STRINGIFY(_np29_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np29_##ProcedureName) ": ...), got: " #ProcedureName "(" p29_PassedName ": ...)"); \
  static_assert(std::string_view(p30_PassedName) == NP_STRINGIFY(_np30_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np30_##ProcedureName) ": ...), got: " #ProcedureName "(" p30_PassedName ": ...)"); \
  static_assert(std::string_view(p31_PassedName) == NP_STRINGIFY(_np31_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np31_##ProcedureName) ": ...), got: " #ProcedureName "(" p31_PassedName ": ...)"); \
  static_assert(std::string_view(p32_PassedName) == NP_STRINGIFY(_np32_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np32_##ProcedureName) ": ...), got: " #ProcedureName "(" p32_PassedName ": ...)"); \
  static_assert(std::string_view(p33_PassedName) == NP_STRINGIFY(_np33_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np33_##ProcedureName) ": ...), got: " #ProcedureName "(" p33_PassedName ": ...)"); \
  static_assert(std::string_view(p34_PassedName) == NP_STRINGIFY(_np34_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np34_##ProcedureName) ": ...), got: " #ProcedureName "(" p34_PassedName ": ...)"); \
  static_assert(std::string_view(p35_PassedName) == NP_STRINGIFY(_np35_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np35_##ProcedureName) ": ...), got: " #ProcedureName "(" p35_PassedName ": ...)"); \
  static_assert(std::string_view(p36_PassedName) == NP_STRINGIFY(_np36_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np36_##ProcedureName) ": ...), got: " #ProcedureName "(" p36_PassedName ": ...)"); \
  static_assert(std::string_view(p37_PassedName) == NP_STRINGIFY(_np37_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np37_##ProcedureName) ": ...), got: " #ProcedureName "(" p37_PassedName ": ...)"); \
  static_assert(std::string_view(p38_PassedName) == NP_STRINGIFY(_np38_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np38_##ProcedureName) ": ...), got: " #ProcedureName "(" p38_PassedName ": ...)"); \
  static_assert(std::string_view(p39_PassedName) == NP_STRINGIFY(_np39_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np39_##ProcedureName) ": ...), got: " #ProcedureName "(" p39_PassedName ": ...)"); \
  static_assert(std::string_view(p40_PassedName) == NP_STRINGIFY(_np40_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np40_##ProcedureName) ": ...), got: " #ProcedureName "(" p40_PassedName ": ...)"); \
  static_assert(std::string_view(p41_PassedName) == NP_STRINGIFY(_np41_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np41_##ProcedureName) ": ...), got: " #ProcedureName "(" p41_PassedName ": ...)"); \
  static_assert(std::string_view(p42_PassedName) == NP_STRINGIFY(_np42_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np42_##ProcedureName) ": ...), got: " #ProcedureName "(" p42_PassedName ": ...)"); \
  static_assert(std::string_view(p43_PassedName) == NP_STRINGIFY(_np43_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np43_##ProcedureName) ": ...), got: " #ProcedureName "(" p43_PassedName ": ...)"); \
  static_assert(std::string_view(p44_PassedName) == NP_STRINGIFY(_np44_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np44_##ProcedureName) ": ...), got: " #ProcedureName "(" p44_PassedName ": ...)")

#define npfp45(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38, \
  p39_PassedName, p39, \
  p40_PassedName, p40, \
  p41_PassedName, p41, \
  p42_PassedName, p42, \
  p43_PassedName, p43, \
  p44_PassedName, p44, \
  p45_PassedName, p45 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)"); \
  static_assert(std::string_view(p26_PassedName) == NP_STRINGIFY(_np26_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np26_##ProcedureName) ": ...), got: " #ProcedureName "(" p26_PassedName ": ...)"); \
  static_assert(std::string_view(p27_PassedName) == NP_STRINGIFY(_np27_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np27_##ProcedureName) ": ...), got: " #ProcedureName "(" p27_PassedName ": ...)"); \
  static_assert(std::string_view(p28_PassedName) == NP_STRINGIFY(_np28_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np28_##ProcedureName) ": ...), got: " #ProcedureName "(" p28_PassedName ": ...)"); \
  static_assert(std::string_view(p29_PassedName) == NP_STRINGIFY(_np29_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np29_##ProcedureName) ": ...), got: " #ProcedureName "(" p29_PassedName ": ...)"); \
  static_assert(std::string_view(p30_PassedName) == NP_STRINGIFY(_np30_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np30_##ProcedureName) ": ...), got: " #ProcedureName "(" p30_PassedName ": ...)"); \
  static_assert(std::string_view(p31_PassedName) == NP_STRINGIFY(_np31_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np31_##ProcedureName) ": ...), got: " #ProcedureName "(" p31_PassedName ": ...)"); \
  static_assert(std::string_view(p32_PassedName) == NP_STRINGIFY(_np32_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np32_##ProcedureName) ": ...), got: " #ProcedureName "(" p32_PassedName ": ...)"); \
  static_assert(std::string_view(p33_PassedName) == NP_STRINGIFY(_np33_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np33_##ProcedureName) ": ...), got: " #ProcedureName "(" p33_PassedName ": ...)"); \
  static_assert(std::string_view(p34_PassedName) == NP_STRINGIFY(_np34_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np34_##ProcedureName) ": ...), got: " #ProcedureName "(" p34_PassedName ": ...)"); \
  static_assert(std::string_view(p35_PassedName) == NP_STRINGIFY(_np35_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np35_##ProcedureName) ": ...), got: " #ProcedureName "(" p35_PassedName ": ...)"); \
  static_assert(std::string_view(p36_PassedName) == NP_STRINGIFY(_np36_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np36_##ProcedureName) ": ...), got: " #ProcedureName "(" p36_PassedName ": ...)"); \
  static_assert(std::string_view(p37_PassedName) == NP_STRINGIFY(_np37_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np37_##ProcedureName) ": ...), got: " #ProcedureName "(" p37_PassedName ": ...)"); \
  static_assert(std::string_view(p38_PassedName) == NP_STRINGIFY(_np38_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np38_##ProcedureName) ": ...), got: " #ProcedureName "(" p38_PassedName ": ...)"); \
  static_assert(std::string_view(p39_PassedName) == NP_STRINGIFY(_np39_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np39_##ProcedureName) ": ...), got: " #ProcedureName "(" p39_PassedName ": ...)"); \
  static_assert(std::string_view(p40_PassedName) == NP_STRINGIFY(_np40_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np40_##ProcedureName) ": ...), got: " #ProcedureName "(" p40_PassedName ": ...)"); \
  static_assert(std::string_view(p41_PassedName) == NP_STRINGIFY(_np41_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np41_##ProcedureName) ": ...), got: " #ProcedureName "(" p41_PassedName ": ...)"); \
  static_assert(std::string_view(p42_PassedName) == NP_STRINGIFY(_np42_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np42_##ProcedureName) ": ...), got: " #ProcedureName "(" p42_PassedName ": ...)"); \
  static_assert(std::string_view(p43_PassedName) == NP_STRINGIFY(_np43_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np43_##ProcedureName) ": ...), got: " #ProcedureName "(" p43_PassedName ": ...)"); \
  static_assert(std::string_view(p44_PassedName) == NP_STRINGIFY(_np44_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np44_##ProcedureName) ": ...), got: " #ProcedureName "(" p44_PassedName ": ...)"); \
  static_assert(std::string_view(p45_PassedName) == NP_STRINGIFY(_np45_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np45_##ProcedureName) ": ...), got: " #ProcedureName "(" p45_PassedName ": ...)")

#define npfp46(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38, \
  p39_PassedName, p39, \
  p40_PassedName, p40, \
  p41_PassedName, p41, \
  p42_PassedName, p42, \
  p43_PassedName, p43, \
  p44_PassedName, p44, \
  p45_PassedName, p45, \
  p46_PassedName, p46 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)"); \
  static_assert(std::string_view(p26_PassedName) == NP_STRINGIFY(_np26_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np26_##ProcedureName) ": ...), got: " #ProcedureName "(" p26_PassedName ": ...)"); \
  static_assert(std::string_view(p27_PassedName) == NP_STRINGIFY(_np27_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np27_##ProcedureName) ": ...), got: " #ProcedureName "(" p27_PassedName ": ...)"); \
  static_assert(std::string_view(p28_PassedName) == NP_STRINGIFY(_np28_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np28_##ProcedureName) ": ...), got: " #ProcedureName "(" p28_PassedName ": ...)"); \
  static_assert(std::string_view(p29_PassedName) == NP_STRINGIFY(_np29_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np29_##ProcedureName) ": ...), got: " #ProcedureName "(" p29_PassedName ": ...)"); \
  static_assert(std::string_view(p30_PassedName) == NP_STRINGIFY(_np30_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np30_##ProcedureName) ": ...), got: " #ProcedureName "(" p30_PassedName ": ...)"); \
  static_assert(std::string_view(p31_PassedName) == NP_STRINGIFY(_np31_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np31_##ProcedureName) ": ...), got: " #ProcedureName "(" p31_PassedName ": ...)"); \
  static_assert(std::string_view(p32_PassedName) == NP_STRINGIFY(_np32_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np32_##ProcedureName) ": ...), got: " #ProcedureName "(" p32_PassedName ": ...)"); \
  static_assert(std::string_view(p33_PassedName) == NP_STRINGIFY(_np33_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np33_##ProcedureName) ": ...), got: " #ProcedureName "(" p33_PassedName ": ...)"); \
  static_assert(std::string_view(p34_PassedName) == NP_STRINGIFY(_np34_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np34_##ProcedureName) ": ...), got: " #ProcedureName "(" p34_PassedName ": ...)"); \
  static_assert(std::string_view(p35_PassedName) == NP_STRINGIFY(_np35_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np35_##ProcedureName) ": ...), got: " #ProcedureName "(" p35_PassedName ": ...)"); \
  static_assert(std::string_view(p36_PassedName) == NP_STRINGIFY(_np36_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np36_##ProcedureName) ": ...), got: " #ProcedureName "(" p36_PassedName ": ...)"); \
  static_assert(std::string_view(p37_PassedName) == NP_STRINGIFY(_np37_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np37_##ProcedureName) ": ...), got: " #ProcedureName "(" p37_PassedName ": ...)"); \
  static_assert(std::string_view(p38_PassedName) == NP_STRINGIFY(_np38_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np38_##ProcedureName) ": ...), got: " #ProcedureName "(" p38_PassedName ": ...)"); \
  static_assert(std::string_view(p39_PassedName) == NP_STRINGIFY(_np39_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np39_##ProcedureName) ": ...), got: " #ProcedureName "(" p39_PassedName ": ...)"); \
  static_assert(std::string_view(p40_PassedName) == NP_STRINGIFY(_np40_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np40_##ProcedureName) ": ...), got: " #ProcedureName "(" p40_PassedName ": ...)"); \
  static_assert(std::string_view(p41_PassedName) == NP_STRINGIFY(_np41_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np41_##ProcedureName) ": ...), got: " #ProcedureName "(" p41_PassedName ": ...)"); \
  static_assert(std::string_view(p42_PassedName) == NP_STRINGIFY(_np42_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np42_##ProcedureName) ": ...), got: " #ProcedureName "(" p42_PassedName ": ...)"); \
  static_assert(std::string_view(p43_PassedName) == NP_STRINGIFY(_np43_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np43_##ProcedureName) ": ...), got: " #ProcedureName "(" p43_PassedName ": ...)"); \
  static_assert(std::string_view(p44_PassedName) == NP_STRINGIFY(_np44_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np44_##ProcedureName) ": ...), got: " #ProcedureName "(" p44_PassedName ": ...)"); \
  static_assert(std::string_view(p45_PassedName) == NP_STRINGIFY(_np45_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np45_##ProcedureName) ": ...), got: " #ProcedureName "(" p45_PassedName ": ...)"); \
  static_assert(std::string_view(p46_PassedName) == NP_STRINGIFY(_np46_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np46_##ProcedureName) ": ...), got: " #ProcedureName "(" p46_PassedName ": ...)")

#define npfp47(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38, \
  p39_PassedName, p39, \
  p40_PassedName, p40, \
  p41_PassedName, p41, \
  p42_PassedName, p42, \
  p43_PassedName, p43, \
  p44_PassedName, p44, \
  p45_PassedName, p45, \
  p46_PassedName, p46, \
  p47_PassedName, p47 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)"); \
  static_assert(std::string_view(p26_PassedName) == NP_STRINGIFY(_np26_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np26_##ProcedureName) ": ...), got: " #ProcedureName "(" p26_PassedName ": ...)"); \
  static_assert(std::string_view(p27_PassedName) == NP_STRINGIFY(_np27_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np27_##ProcedureName) ": ...), got: " #ProcedureName "(" p27_PassedName ": ...)"); \
  static_assert(std::string_view(p28_PassedName) == NP_STRINGIFY(_np28_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np28_##ProcedureName) ": ...), got: " #ProcedureName "(" p28_PassedName ": ...)"); \
  static_assert(std::string_view(p29_PassedName) == NP_STRINGIFY(_np29_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np29_##ProcedureName) ": ...), got: " #ProcedureName "(" p29_PassedName ": ...)"); \
  static_assert(std::string_view(p30_PassedName) == NP_STRINGIFY(_np30_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np30_##ProcedureName) ": ...), got: " #ProcedureName "(" p30_PassedName ": ...)"); \
  static_assert(std::string_view(p31_PassedName) == NP_STRINGIFY(_np31_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np31_##ProcedureName) ": ...), got: " #ProcedureName "(" p31_PassedName ": ...)"); \
  static_assert(std::string_view(p32_PassedName) == NP_STRINGIFY(_np32_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np32_##ProcedureName) ": ...), got: " #ProcedureName "(" p32_PassedName ": ...)"); \
  static_assert(std::string_view(p33_PassedName) == NP_STRINGIFY(_np33_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np33_##ProcedureName) ": ...), got: " #ProcedureName "(" p33_PassedName ": ...)"); \
  static_assert(std::string_view(p34_PassedName) == NP_STRINGIFY(_np34_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np34_##ProcedureName) ": ...), got: " #ProcedureName "(" p34_PassedName ": ...)"); \
  static_assert(std::string_view(p35_PassedName) == NP_STRINGIFY(_np35_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np35_##ProcedureName) ": ...), got: " #ProcedureName "(" p35_PassedName ": ...)"); \
  static_assert(std::string_view(p36_PassedName) == NP_STRINGIFY(_np36_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np36_##ProcedureName) ": ...), got: " #ProcedureName "(" p36_PassedName ": ...)"); \
  static_assert(std::string_view(p37_PassedName) == NP_STRINGIFY(_np37_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np37_##ProcedureName) ": ...), got: " #ProcedureName "(" p37_PassedName ": ...)"); \
  static_assert(std::string_view(p38_PassedName) == NP_STRINGIFY(_np38_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np38_##ProcedureName) ": ...), got: " #ProcedureName "(" p38_PassedName ": ...)"); \
  static_assert(std::string_view(p39_PassedName) == NP_STRINGIFY(_np39_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np39_##ProcedureName) ": ...), got: " #ProcedureName "(" p39_PassedName ": ...)"); \
  static_assert(std::string_view(p40_PassedName) == NP_STRINGIFY(_np40_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np40_##ProcedureName) ": ...), got: " #ProcedureName "(" p40_PassedName ": ...)"); \
  static_assert(std::string_view(p41_PassedName) == NP_STRINGIFY(_np41_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np41_##ProcedureName) ": ...), got: " #ProcedureName "(" p41_PassedName ": ...)"); \
  static_assert(std::string_view(p42_PassedName) == NP_STRINGIFY(_np42_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np42_##ProcedureName) ": ...), got: " #ProcedureName "(" p42_PassedName ": ...)"); \
  static_assert(std::string_view(p43_PassedName) == NP_STRINGIFY(_np43_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np43_##ProcedureName) ": ...), got: " #ProcedureName "(" p43_PassedName ": ...)"); \
  static_assert(std::string_view(p44_PassedName) == NP_STRINGIFY(_np44_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np44_##ProcedureName) ": ...), got: " #ProcedureName "(" p44_PassedName ": ...)"); \
  static_assert(std::string_view(p45_PassedName) == NP_STRINGIFY(_np45_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np45_##ProcedureName) ": ...), got: " #ProcedureName "(" p45_PassedName ": ...)"); \
  static_assert(std::string_view(p46_PassedName) == NP_STRINGIFY(_np46_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np46_##ProcedureName) ": ...), got: " #ProcedureName "(" p46_PassedName ": ...)"); \
  static_assert(std::string_view(p47_PassedName) == NP_STRINGIFY(_np47_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np47_##ProcedureName) ": ...), got: " #ProcedureName "(" p47_PassedName ": ...)")

#define npfp48(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38, \
  p39_PassedName, p39, \
  p40_PassedName, p40, \
  p41_PassedName, p41, \
  p42_PassedName, p42, \
  p43_PassedName, p43, \
  p44_PassedName, p44, \
  p45_PassedName, p45, \
  p46_PassedName, p46, \
  p47_PassedName, p47, \
  p48_PassedName, p48 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)"); \
  static_assert(std::string_view(p26_PassedName) == NP_STRINGIFY(_np26_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np26_##ProcedureName) ": ...), got: " #ProcedureName "(" p26_PassedName ": ...)"); \
  static_assert(std::string_view(p27_PassedName) == NP_STRINGIFY(_np27_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np27_##ProcedureName) ": ...), got: " #ProcedureName "(" p27_PassedName ": ...)"); \
  static_assert(std::string_view(p28_PassedName) == NP_STRINGIFY(_np28_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np28_##ProcedureName) ": ...), got: " #ProcedureName "(" p28_PassedName ": ...)"); \
  static_assert(std::string_view(p29_PassedName) == NP_STRINGIFY(_np29_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np29_##ProcedureName) ": ...), got: " #ProcedureName "(" p29_PassedName ": ...)"); \
  static_assert(std::string_view(p30_PassedName) == NP_STRINGIFY(_np30_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np30_##ProcedureName) ": ...), got: " #ProcedureName "(" p30_PassedName ": ...)"); \
  static_assert(std::string_view(p31_PassedName) == NP_STRINGIFY(_np31_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np31_##ProcedureName) ": ...), got: " #ProcedureName "(" p31_PassedName ": ...)"); \
  static_assert(std::string_view(p32_PassedName) == NP_STRINGIFY(_np32_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np32_##ProcedureName) ": ...), got: " #ProcedureName "(" p32_PassedName ": ...)"); \
  static_assert(std::string_view(p33_PassedName) == NP_STRINGIFY(_np33_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np33_##ProcedureName) ": ...), got: " #ProcedureName "(" p33_PassedName ": ...)"); \
  static_assert(std::string_view(p34_PassedName) == NP_STRINGIFY(_np34_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np34_##ProcedureName) ": ...), got: " #ProcedureName "(" p34_PassedName ": ...)"); \
  static_assert(std::string_view(p35_PassedName) == NP_STRINGIFY(_np35_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np35_##ProcedureName) ": ...), got: " #ProcedureName "(" p35_PassedName ": ...)"); \
  static_assert(std::string_view(p36_PassedName) == NP_STRINGIFY(_np36_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np36_##ProcedureName) ": ...), got: " #ProcedureName "(" p36_PassedName ": ...)"); \
  static_assert(std::string_view(p37_PassedName) == NP_STRINGIFY(_np37_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np37_##ProcedureName) ": ...), got: " #ProcedureName "(" p37_PassedName ": ...)"); \
  static_assert(std::string_view(p38_PassedName) == NP_STRINGIFY(_np38_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np38_##ProcedureName) ": ...), got: " #ProcedureName "(" p38_PassedName ": ...)"); \
  static_assert(std::string_view(p39_PassedName) == NP_STRINGIFY(_np39_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np39_##ProcedureName) ": ...), got: " #ProcedureName "(" p39_PassedName ": ...)"); \
  static_assert(std::string_view(p40_PassedName) == NP_STRINGIFY(_np40_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np40_##ProcedureName) ": ...), got: " #ProcedureName "(" p40_PassedName ": ...)"); \
  static_assert(std::string_view(p41_PassedName) == NP_STRINGIFY(_np41_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np41_##ProcedureName) ": ...), got: " #ProcedureName "(" p41_PassedName ": ...)"); \
  static_assert(std::string_view(p42_PassedName) == NP_STRINGIFY(_np42_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np42_##ProcedureName) ": ...), got: " #ProcedureName "(" p42_PassedName ": ...)"); \
  static_assert(std::string_view(p43_PassedName) == NP_STRINGIFY(_np43_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np43_##ProcedureName) ": ...), got: " #ProcedureName "(" p43_PassedName ": ...)"); \
  static_assert(std::string_view(p44_PassedName) == NP_STRINGIFY(_np44_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np44_##ProcedureName) ": ...), got: " #ProcedureName "(" p44_PassedName ": ...)"); \
  static_assert(std::string_view(p45_PassedName) == NP_STRINGIFY(_np45_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np45_##ProcedureName) ": ...), got: " #ProcedureName "(" p45_PassedName ": ...)"); \
  static_assert(std::string_view(p46_PassedName) == NP_STRINGIFY(_np46_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np46_##ProcedureName) ": ...), got: " #ProcedureName "(" p46_PassedName ": ...)"); \
  static_assert(std::string_view(p47_PassedName) == NP_STRINGIFY(_np47_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np47_##ProcedureName) ": ...), got: " #ProcedureName "(" p47_PassedName ": ...)"); \
  static_assert(std::string_view(p48_PassedName) == NP_STRINGIFY(_np48_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np48_##ProcedureName) ": ...), got: " #ProcedureName "(" p48_PassedName ": ...)")

#define npfp49(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38, \
  p39_PassedName, p39, \
  p40_PassedName, p40, \
  p41_PassedName, p41, \
  p42_PassedName, p42, \
  p43_PassedName, p43, \
  p44_PassedName, p44, \
  p45_PassedName, p45, \
  p46_PassedName, p46, \
  p47_PassedName, p47, \
  p48_PassedName, p48, \
  p49_PassedName, p49 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)"); \
  static_assert(std::string_view(p26_PassedName) == NP_STRINGIFY(_np26_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np26_##ProcedureName) ": ...), got: " #ProcedureName "(" p26_PassedName ": ...)"); \
  static_assert(std::string_view(p27_PassedName) == NP_STRINGIFY(_np27_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np27_##ProcedureName) ": ...), got: " #ProcedureName "(" p27_PassedName ": ...)"); \
  static_assert(std::string_view(p28_PassedName) == NP_STRINGIFY(_np28_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np28_##ProcedureName) ": ...), got: " #ProcedureName "(" p28_PassedName ": ...)"); \
  static_assert(std::string_view(p29_PassedName) == NP_STRINGIFY(_np29_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np29_##ProcedureName) ": ...), got: " #ProcedureName "(" p29_PassedName ": ...)"); \
  static_assert(std::string_view(p30_PassedName) == NP_STRINGIFY(_np30_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np30_##ProcedureName) ": ...), got: " #ProcedureName "(" p30_PassedName ": ...)"); \
  static_assert(std::string_view(p31_PassedName) == NP_STRINGIFY(_np31_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np31_##ProcedureName) ": ...), got: " #ProcedureName "(" p31_PassedName ": ...)"); \
  static_assert(std::string_view(p32_PassedName) == NP_STRINGIFY(_np32_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np32_##ProcedureName) ": ...), got: " #ProcedureName "(" p32_PassedName ": ...)"); \
  static_assert(std::string_view(p33_PassedName) == NP_STRINGIFY(_np33_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np33_##ProcedureName) ": ...), got: " #ProcedureName "(" p33_PassedName ": ...)"); \
  static_assert(std::string_view(p34_PassedName) == NP_STRINGIFY(_np34_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np34_##ProcedureName) ": ...), got: " #ProcedureName "(" p34_PassedName ": ...)"); \
  static_assert(std::string_view(p35_PassedName) == NP_STRINGIFY(_np35_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np35_##ProcedureName) ": ...), got: " #ProcedureName "(" p35_PassedName ": ...)"); \
  static_assert(std::string_view(p36_PassedName) == NP_STRINGIFY(_np36_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np36_##ProcedureName) ": ...), got: " #ProcedureName "(" p36_PassedName ": ...)"); \
  static_assert(std::string_view(p37_PassedName) == NP_STRINGIFY(_np37_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np37_##ProcedureName) ": ...), got: " #ProcedureName "(" p37_PassedName ": ...)"); \
  static_assert(std::string_view(p38_PassedName) == NP_STRINGIFY(_np38_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np38_##ProcedureName) ": ...), got: " #ProcedureName "(" p38_PassedName ": ...)"); \
  static_assert(std::string_view(p39_PassedName) == NP_STRINGIFY(_np39_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np39_##ProcedureName) ": ...), got: " #ProcedureName "(" p39_PassedName ": ...)"); \
  static_assert(std::string_view(p40_PassedName) == NP_STRINGIFY(_np40_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np40_##ProcedureName) ": ...), got: " #ProcedureName "(" p40_PassedName ": ...)"); \
  static_assert(std::string_view(p41_PassedName) == NP_STRINGIFY(_np41_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np41_##ProcedureName) ": ...), got: " #ProcedureName "(" p41_PassedName ": ...)"); \
  static_assert(std::string_view(p42_PassedName) == NP_STRINGIFY(_np42_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np42_##ProcedureName) ": ...), got: " #ProcedureName "(" p42_PassedName ": ...)"); \
  static_assert(std::string_view(p43_PassedName) == NP_STRINGIFY(_np43_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np43_##ProcedureName) ": ...), got: " #ProcedureName "(" p43_PassedName ": ...)"); \
  static_assert(std::string_view(p44_PassedName) == NP_STRINGIFY(_np44_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np44_##ProcedureName) ": ...), got: " #ProcedureName "(" p44_PassedName ": ...)"); \
  static_assert(std::string_view(p45_PassedName) == NP_STRINGIFY(_np45_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np45_##ProcedureName) ": ...), got: " #ProcedureName "(" p45_PassedName ": ...)"); \
  static_assert(std::string_view(p46_PassedName) == NP_STRINGIFY(_np46_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np46_##ProcedureName) ": ...), got: " #ProcedureName "(" p46_PassedName ": ...)"); \
  static_assert(std::string_view(p47_PassedName) == NP_STRINGIFY(_np47_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np47_##ProcedureName) ": ...), got: " #ProcedureName "(" p47_PassedName ": ...)"); \
  static_assert(std::string_view(p48_PassedName) == NP_STRINGIFY(_np48_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np48_##ProcedureName) ": ...), got: " #ProcedureName "(" p48_PassedName ": ...)"); \
  static_assert(std::string_view(p49_PassedName) == NP_STRINGIFY(_np49_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np49_##ProcedureName) ": ...), got: " #ProcedureName "(" p49_PassedName ": ...)")

#define npfp50(ProcedureName, ProcedureCall, \
  p1_PassedName, p1, \
  p2_PassedName, p2, \
  p3_PassedName, p3, \
  p4_PassedName, p4, \
  p5_PassedName, p5, \
  p6_PassedName, p6, \
  p7_PassedName, p7, \
  p8_PassedName, p8, \
  p9_PassedName, p9, \
  p10_PassedName, p10, \
  p11_PassedName, p11, \
  p12_PassedName, p12, \
  p13_PassedName, p13, \
  p14_PassedName, p14, \
  p15_PassedName, p15, \
  p16_PassedName, p16, \
  p17_PassedName, p17, \
  p18_PassedName, p18, \
  p19_PassedName, p19, \
  p20_PassedName, p20, \
  p21_PassedName, p21, \
  p22_PassedName, p22, \
  p23_PassedName, p23, \
  p24_PassedName, p24, \
  p25_PassedName, p25, \
  p26_PassedName, p26, \
  p27_PassedName, p27, \
  p28_PassedName, p28, \
  p29_PassedName, p29, \
  p30_PassedName, p30, \
  p31_PassedName, p31, \
  p32_PassedName, p32, \
  p33_PassedName, p33, \
  p34_PassedName, p34, \
  p35_PassedName, p35, \
  p36_PassedName, p36, \
  p37_PassedName, p37, \
  p38_PassedName, p38, \
  p39_PassedName, p39, \
  p40_PassedName, p40, \
  p41_PassedName, p41, \
  p42_PassedName, p42, \
  p43_PassedName, p43, \
  p44_PassedName, p44, \
  p45_PassedName, p45, \
  p46_PassedName, p46, \
  p47_PassedName, p47, \
  p48_PassedName, p48, \
  p49_PassedName, p49, \
  p50_PassedName, p50 \
) \
  ProcedureCall(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, p42, p43, p44, p45, p46, p47, p48, p49, p50); \
  static_assert(std::string_view(p1_PassedName) == NP_STRINGIFY(_np1_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np1_##ProcedureName) ": ...), got: " #ProcedureName "(" p1_PassedName ": ...)"); \
  static_assert(std::string_view(p2_PassedName) == NP_STRINGIFY(_np2_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np2_##ProcedureName) ": ...), got: " #ProcedureName "(" p2_PassedName ": ...)"); \
  static_assert(std::string_view(p3_PassedName) == NP_STRINGIFY(_np3_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np3_##ProcedureName) ": ...), got: " #ProcedureName "(" p3_PassedName ": ...)"); \
  static_assert(std::string_view(p4_PassedName) == NP_STRINGIFY(_np4_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np4_##ProcedureName) ": ...), got: " #ProcedureName "(" p4_PassedName ": ...)"); \
  static_assert(std::string_view(p5_PassedName) == NP_STRINGIFY(_np5_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np5_##ProcedureName) ": ...), got: " #ProcedureName "(" p5_PassedName ": ...)"); \
  static_assert(std::string_view(p6_PassedName) == NP_STRINGIFY(_np6_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np6_##ProcedureName) ": ...), got: " #ProcedureName "(" p6_PassedName ": ...)"); \
  static_assert(std::string_view(p7_PassedName) == NP_STRINGIFY(_np7_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np7_##ProcedureName) ": ...), got: " #ProcedureName "(" p7_PassedName ": ...)"); \
  static_assert(std::string_view(p8_PassedName) == NP_STRINGIFY(_np8_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np8_##ProcedureName) ": ...), got: " #ProcedureName "(" p8_PassedName ": ...)"); \
  static_assert(std::string_view(p9_PassedName) == NP_STRINGIFY(_np9_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np9_##ProcedureName) ": ...), got: " #ProcedureName "(" p9_PassedName ": ...)"); \
  static_assert(std::string_view(p10_PassedName) == NP_STRINGIFY(_np10_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np10_##ProcedureName) ": ...), got: " #ProcedureName "(" p10_PassedName ": ...)"); \
  static_assert(std::string_view(p11_PassedName) == NP_STRINGIFY(_np11_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np11_##ProcedureName) ": ...), got: " #ProcedureName "(" p11_PassedName ": ...)"); \
  static_assert(std::string_view(p12_PassedName) == NP_STRINGIFY(_np12_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np12_##ProcedureName) ": ...), got: " #ProcedureName "(" p12_PassedName ": ...)"); \
  static_assert(std::string_view(p13_PassedName) == NP_STRINGIFY(_np13_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np13_##ProcedureName) ": ...), got: " #ProcedureName "(" p13_PassedName ": ...)"); \
  static_assert(std::string_view(p14_PassedName) == NP_STRINGIFY(_np14_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np14_##ProcedureName) ": ...), got: " #ProcedureName "(" p14_PassedName ": ...)"); \
  static_assert(std::string_view(p15_PassedName) == NP_STRINGIFY(_np15_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np15_##ProcedureName) ": ...), got: " #ProcedureName "(" p15_PassedName ": ...)"); \
  static_assert(std::string_view(p16_PassedName) == NP_STRINGIFY(_np16_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np16_##ProcedureName) ": ...), got: " #ProcedureName "(" p16_PassedName ": ...)"); \
  static_assert(std::string_view(p17_PassedName) == NP_STRINGIFY(_np17_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np17_##ProcedureName) ": ...), got: " #ProcedureName "(" p17_PassedName ": ...)"); \
  static_assert(std::string_view(p18_PassedName) == NP_STRINGIFY(_np18_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np18_##ProcedureName) ": ...), got: " #ProcedureName "(" p18_PassedName ": ...)"); \
  static_assert(std::string_view(p19_PassedName) == NP_STRINGIFY(_np19_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np19_##ProcedureName) ": ...), got: " #ProcedureName "(" p19_PassedName ": ...)"); \
  static_assert(std::string_view(p20_PassedName) == NP_STRINGIFY(_np20_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np20_##ProcedureName) ": ...), got: " #ProcedureName "(" p20_PassedName ": ...)"); \
  static_assert(std::string_view(p21_PassedName) == NP_STRINGIFY(_np21_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np21_##ProcedureName) ": ...), got: " #ProcedureName "(" p21_PassedName ": ...)"); \
  static_assert(std::string_view(p22_PassedName) == NP_STRINGIFY(_np22_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np22_##ProcedureName) ": ...), got: " #ProcedureName "(" p22_PassedName ": ...)"); \
  static_assert(std::string_view(p23_PassedName) == NP_STRINGIFY(_np23_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np23_##ProcedureName) ": ...), got: " #ProcedureName "(" p23_PassedName ": ...)"); \
  static_assert(std::string_view(p24_PassedName) == NP_STRINGIFY(_np24_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np24_##ProcedureName) ": ...), got: " #ProcedureName "(" p24_PassedName ": ...)"); \
  static_assert(std::string_view(p25_PassedName) == NP_STRINGIFY(_np25_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np25_##ProcedureName) ": ...), got: " #ProcedureName "(" p25_PassedName ": ...)"); \
  static_assert(std::string_view(p26_PassedName) == NP_STRINGIFY(_np26_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np26_##ProcedureName) ": ...), got: " #ProcedureName "(" p26_PassedName ": ...)"); \
  static_assert(std::string_view(p27_PassedName) == NP_STRINGIFY(_np27_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np27_##ProcedureName) ": ...), got: " #ProcedureName "(" p27_PassedName ": ...)"); \
  static_assert(std::string_view(p28_PassedName) == NP_STRINGIFY(_np28_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np28_##ProcedureName) ": ...), got: " #ProcedureName "(" p28_PassedName ": ...)"); \
  static_assert(std::string_view(p29_PassedName) == NP_STRINGIFY(_np29_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np29_##ProcedureName) ": ...), got: " #ProcedureName "(" p29_PassedName ": ...)"); \
  static_assert(std::string_view(p30_PassedName) == NP_STRINGIFY(_np30_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np30_##ProcedureName) ": ...), got: " #ProcedureName "(" p30_PassedName ": ...)"); \
  static_assert(std::string_view(p31_PassedName) == NP_STRINGIFY(_np31_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np31_##ProcedureName) ": ...), got: " #ProcedureName "(" p31_PassedName ": ...)"); \
  static_assert(std::string_view(p32_PassedName) == NP_STRINGIFY(_np32_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np32_##ProcedureName) ": ...), got: " #ProcedureName "(" p32_PassedName ": ...)"); \
  static_assert(std::string_view(p33_PassedName) == NP_STRINGIFY(_np33_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np33_##ProcedureName) ": ...), got: " #ProcedureName "(" p33_PassedName ": ...)"); \
  static_assert(std::string_view(p34_PassedName) == NP_STRINGIFY(_np34_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np34_##ProcedureName) ": ...), got: " #ProcedureName "(" p34_PassedName ": ...)"); \
  static_assert(std::string_view(p35_PassedName) == NP_STRINGIFY(_np35_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np35_##ProcedureName) ": ...), got: " #ProcedureName "(" p35_PassedName ": ...)"); \
  static_assert(std::string_view(p36_PassedName) == NP_STRINGIFY(_np36_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np36_##ProcedureName) ": ...), got: " #ProcedureName "(" p36_PassedName ": ...)"); \
  static_assert(std::string_view(p37_PassedName) == NP_STRINGIFY(_np37_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np37_##ProcedureName) ": ...), got: " #ProcedureName "(" p37_PassedName ": ...)"); \
  static_assert(std::string_view(p38_PassedName) == NP_STRINGIFY(_np38_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np38_##ProcedureName) ": ...), got: " #ProcedureName "(" p38_PassedName ": ...)"); \
  static_assert(std::string_view(p39_PassedName) == NP_STRINGIFY(_np39_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np39_##ProcedureName) ": ...), got: " #ProcedureName "(" p39_PassedName ": ...)"); \
  static_assert(std::string_view(p40_PassedName) == NP_STRINGIFY(_np40_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np40_##ProcedureName) ": ...), got: " #ProcedureName "(" p40_PassedName ": ...)"); \
  static_assert(std::string_view(p41_PassedName) == NP_STRINGIFY(_np41_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np41_##ProcedureName) ": ...), got: " #ProcedureName "(" p41_PassedName ": ...)"); \
  static_assert(std::string_view(p42_PassedName) == NP_STRINGIFY(_np42_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np42_##ProcedureName) ": ...), got: " #ProcedureName "(" p42_PassedName ": ...)"); \
  static_assert(std::string_view(p43_PassedName) == NP_STRINGIFY(_np43_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np43_##ProcedureName) ": ...), got: " #ProcedureName "(" p43_PassedName ": ...)"); \
  static_assert(std::string_view(p44_PassedName) == NP_STRINGIFY(_np44_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np44_##ProcedureName) ": ...), got: " #ProcedureName "(" p44_PassedName ": ...)"); \
  static_assert(std::string_view(p45_PassedName) == NP_STRINGIFY(_np45_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np45_##ProcedureName) ": ...), got: " #ProcedureName "(" p45_PassedName ": ...)"); \
  static_assert(std::string_view(p46_PassedName) == NP_STRINGIFY(_np46_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np46_##ProcedureName) ": ...), got: " #ProcedureName "(" p46_PassedName ": ...)"); \
  static_assert(std::string_view(p47_PassedName) == NP_STRINGIFY(_np47_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np47_##ProcedureName) ": ...), got: " #ProcedureName "(" p47_PassedName ": ...)"); \
  static_assert(std::string_view(p48_PassedName) == NP_STRINGIFY(_np48_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np48_##ProcedureName) ": ...), got: " #ProcedureName "(" p48_PassedName ": ...)"); \
  static_assert(std::string_view(p49_PassedName) == NP_STRINGIFY(_np49_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np49_##ProcedureName) ": ...), got: " #ProcedureName "(" p49_PassedName ": ...)"); \
  static_assert(std::string_view(p50_PassedName) == NP_STRINGIFY(_np50_##ProcedureName), "Expected parameter name: " #ProcedureName "(" NP_STRINGIFY(_np50_##ProcedureName) ": ...), got: " #ProcedureName "(" p50_PassedName ": ...)")

#endif // #ifdef REDGPU_DISABLE_NAMED_PARAMETERS

