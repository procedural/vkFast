#pragma once

#if !defined(__cplusplus) && !defined(static_assert)
  #define static_assert _Static_assert
#endif

#define __NPF_COUNTOF_CAT( a, b ) a b
#define __NPF_COUNTOF_A( \
  a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74, a75, a76, a77, a78, a79, a80, a81, a82, a83, a84, a85, a86, a87, a88, a89, a90, a91, a92, a93, a94, a95, a96, a97, a98, a99, a100, a101, a102, \
  n, ... ) n
#define __NPF_COUNTOF(...) __NPF_COUNTOF_CAT( __NPF_COUNTOF_A, ( 0, ##__VA_ARGS__, \
  50, 0, 49, 0, 48, 0, 47, 0, 46, 0, 45, 0, 44, 0, 43, 0, 42, 0, 41, 0, 40, 0, 39, 0, 38, 0, 37, 0, 36, 0, 35, 0, 34, 0, 33, 0, 32, 0, 31, 0, 30, 0, 29, 0, 28, 0, 27, 0, 26, 0, 25, 0, 24, 0, 23, 0, 22, 0, 21, 0, 20, 0, 19, 0, 18, 0, 17, 0, 16, 0, 15, 0, 14, 0, 13, 0, 12, 0, 11, 0, 10, 0, 9, 0, 8, 0, 7, 0, 6, 0, 5, 0, 4, 0, 3, 0, 2, 0, 1, 0, 0, 0, 0 ) )
#define __NPF_VA_EXPAND(name, ...)        __NPF_VA_EXPAND_JOIN(name, __NPF_COUNTOF(__VA_ARGS__))(__VA_ARGS__)
#define __NPF_VA_EXPAND_JOIN(name, count) __NPF_VA_EXPAND_J0IN(name, count)
#define __NPF_VA_EXPAND_J0IN(name, count) __NPF_VA_EXPAND_J01N(name, count)
#define __NPF_VA_EXPAND_J01N(name, count) name##count

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

#define npf(...)  __NPF_VA_EXPAND(npf,  __VA_ARGS__)
#define np(...)    __NP_VA_EXPAND(np,   __VA_ARGS__)
#define npfp(...) __NPF_VA_EXPAND(npfp, __VA_ARGS__)

#define npf0(FNDECL, FNNAME) FNDECL FNNAME()
#define np0(FNNAME) FNNAME()
#define npfp0(FNNAME, FNPTR) FNPTR()

// npf

#if !defined(DISABLE_NAMED_PARAMETERS)

  #define npf1(FNDECL, FNNAME, PARTYPE1, PARNAME1) \
    enum { _named_parameter_##FNNAME##_##PARNAME1 = 1 }; \
    FNDECL FNNAME(PARTYPE1 PARNAME1)

  #define npf2(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2) \
    enum { _named_parameter_##FNNAME##_##PARNAME1 = 1 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME2 = 2 }; \
    FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2)

  #define npf3(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3) \
    enum { _named_parameter_##FNNAME##_##PARNAME1 = 1 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME2 = 2 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME3 = 3 }; \
    FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3)

  #define npf4(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4) \
    enum { _named_parameter_##FNNAME##_##PARNAME1 = 1 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME2 = 2 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME3 = 3 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME4 = 4 }; \
    FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4)

  #define npf5(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5) \
    enum { _named_parameter_##FNNAME##_##PARNAME1 = 1 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME2 = 2 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME3 = 3 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME4 = 4 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME5 = 5 }; \
    FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5)

  #define npf6(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6) \
    enum { _named_parameter_##FNNAME##_##PARNAME1 = 1 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME2 = 2 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME3 = 3 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME4 = 4 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME5 = 5 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME6 = 6 }; \
    FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6)

  #define npf7(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7) \
    enum { _named_parameter_##FNNAME##_##PARNAME1 = 1 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME2 = 2 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME3 = 3 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME4 = 4 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME5 = 5 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME6 = 6 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME7 = 7 }; \
    FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7)

  #define npf8(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8) \
    enum { _named_parameter_##FNNAME##_##PARNAME1 = 1 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME2 = 2 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME3 = 3 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME4 = 4 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME5 = 5 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME6 = 6 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME7 = 7 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME8 = 8 }; \
    FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8)

  #define npf9(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9) \
    enum { _named_parameter_##FNNAME##_##PARNAME1 = 1 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME2 = 2 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME3 = 3 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME4 = 4 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME5 = 5 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME6 = 6 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME7 = 7 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME8 = 8 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME9 = 9 }; \
    FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9)

  #define npf10(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10) \
    enum { _named_parameter_##FNNAME##_##PARNAME1 = 1 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME2 = 2 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME3 = 3 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME4 = 4 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME5 = 5 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME6 = 6 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME7 = 7 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME8 = 8 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME9 = 9 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME10 = 10 }; \
    FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10)

  #define npf11(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11) \
    enum { _named_parameter_##FNNAME##_##PARNAME1 = 1 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME2 = 2 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME3 = 3 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME4 = 4 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME5 = 5 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME6 = 6 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME7 = 7 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME8 = 8 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME9 = 9 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME10 = 10 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME11 = 11 }; \
    FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11)

  #define npf12(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12) \
    enum { _named_parameter_##FNNAME##_##PARNAME1 = 1 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME2 = 2 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME3 = 3 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME4 = 4 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME5 = 5 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME6 = 6 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME7 = 7 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME8 = 8 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME9 = 9 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME10 = 10 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME11 = 11 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME12 = 12 }; \
    FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12)

  #define npf13(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13) \
    enum { _named_parameter_##FNNAME##_##PARNAME1 = 1 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME2 = 2 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME3 = 3 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME4 = 4 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME5 = 5 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME6 = 6 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME7 = 7 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME8 = 8 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME9 = 9 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME10 = 10 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME11 = 11 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME12 = 12 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME13 = 13 }; \
    FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13)

  #define npf14(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14) \
    enum { _named_parameter_##FNNAME##_##PARNAME1 = 1 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME2 = 2 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME3 = 3 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME4 = 4 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME5 = 5 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME6 = 6 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME7 = 7 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME8 = 8 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME9 = 9 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME10 = 10 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME11 = 11 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME12 = 12 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME13 = 13 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME14 = 14 }; \
    FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14)

  #define npf15(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15) \
    enum { _named_parameter_##FNNAME##_##PARNAME1 = 1 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME2 = 2 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME3 = 3 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME4 = 4 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME5 = 5 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME6 = 6 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME7 = 7 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME8 = 8 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME9 = 9 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME10 = 10 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME11 = 11 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME12 = 12 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME13 = 13 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME14 = 14 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME15 = 15 }; \
    FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15)

  #define npf16(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16) \
    enum { _named_parameter_##FNNAME##_##PARNAME1 = 1 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME2 = 2 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME3 = 3 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME4 = 4 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME5 = 5 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME6 = 6 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME7 = 7 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME8 = 8 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME9 = 9 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME10 = 10 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME11 = 11 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME12 = 12 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME13 = 13 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME14 = 14 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME15 = 15 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME16 = 16 }; \
    FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16)

  #define npf17(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17) \
    enum { _named_parameter_##FNNAME##_##PARNAME1 = 1 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME2 = 2 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME3 = 3 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME4 = 4 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME5 = 5 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME6 = 6 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME7 = 7 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME8 = 8 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME9 = 9 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME10 = 10 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME11 = 11 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME12 = 12 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME13 = 13 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME14 = 14 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME15 = 15 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME16 = 16 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME17 = 17 }; \
    FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17)

  #define npf18(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18) \
    enum { _named_parameter_##FNNAME##_##PARNAME1 = 1 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME2 = 2 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME3 = 3 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME4 = 4 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME5 = 5 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME6 = 6 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME7 = 7 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME8 = 8 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME9 = 9 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME10 = 10 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME11 = 11 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME12 = 12 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME13 = 13 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME14 = 14 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME15 = 15 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME16 = 16 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME17 = 17 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME18 = 18 }; \
    FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18)

  #define npf19(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19) \
    enum { _named_parameter_##FNNAME##_##PARNAME1 = 1 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME2 = 2 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME3 = 3 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME4 = 4 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME5 = 5 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME6 = 6 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME7 = 7 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME8 = 8 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME9 = 9 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME10 = 10 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME11 = 11 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME12 = 12 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME13 = 13 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME14 = 14 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME15 = 15 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME16 = 16 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME17 = 17 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME18 = 18 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME19 = 19 }; \
    FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19)

  #define npf20(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20) \
    enum { _named_parameter_##FNNAME##_##PARNAME1 = 1 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME2 = 2 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME3 = 3 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME4 = 4 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME5 = 5 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME6 = 6 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME7 = 7 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME8 = 8 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME9 = 9 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME10 = 10 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME11 = 11 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME12 = 12 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME13 = 13 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME14 = 14 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME15 = 15 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME16 = 16 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME17 = 17 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME18 = 18 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME19 = 19 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME20 = 20 }; \
    FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20)

  #define npf21(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21) \
    enum { _named_parameter_##FNNAME##_##PARNAME1 = 1 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME2 = 2 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME3 = 3 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME4 = 4 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME5 = 5 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME6 = 6 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME7 = 7 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME8 = 8 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME9 = 9 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME10 = 10 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME11 = 11 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME12 = 12 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME13 = 13 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME14 = 14 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME15 = 15 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME16 = 16 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME17 = 17 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME18 = 18 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME19 = 19 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME20 = 20 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME21 = 21 }; \
    FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21)

  #define npf22(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22) \
    enum { _named_parameter_##FNNAME##_##PARNAME1 = 1 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME2 = 2 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME3 = 3 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME4 = 4 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME5 = 5 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME6 = 6 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME7 = 7 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME8 = 8 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME9 = 9 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME10 = 10 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME11 = 11 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME12 = 12 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME13 = 13 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME14 = 14 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME15 = 15 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME16 = 16 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME17 = 17 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME18 = 18 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME19 = 19 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME20 = 20 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME21 = 21 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME22 = 22 }; \
    FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22)

  #define npf23(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23) \
    enum { _named_parameter_##FNNAME##_##PARNAME1 = 1 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME2 = 2 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME3 = 3 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME4 = 4 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME5 = 5 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME6 = 6 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME7 = 7 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME8 = 8 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME9 = 9 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME10 = 10 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME11 = 11 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME12 = 12 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME13 = 13 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME14 = 14 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME15 = 15 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME16 = 16 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME17 = 17 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME18 = 18 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME19 = 19 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME20 = 20 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME21 = 21 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME22 = 22 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME23 = 23 }; \
    FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23)

  #define npf24(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24) \
    enum { _named_parameter_##FNNAME##_##PARNAME1 = 1 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME2 = 2 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME3 = 3 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME4 = 4 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME5 = 5 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME6 = 6 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME7 = 7 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME8 = 8 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME9 = 9 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME10 = 10 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME11 = 11 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME12 = 12 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME13 = 13 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME14 = 14 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME15 = 15 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME16 = 16 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME17 = 17 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME18 = 18 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME19 = 19 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME20 = 20 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME21 = 21 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME22 = 22 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME23 = 23 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME24 = 24 }; \
    FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24)

  #define npf25(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25) \
    enum { _named_parameter_##FNNAME##_##PARNAME1 = 1 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME2 = 2 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME3 = 3 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME4 = 4 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME5 = 5 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME6 = 6 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME7 = 7 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME8 = 8 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME9 = 9 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME10 = 10 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME11 = 11 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME12 = 12 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME13 = 13 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME14 = 14 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME15 = 15 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME16 = 16 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME17 = 17 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME18 = 18 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME19 = 19 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME20 = 20 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME21 = 21 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME22 = 22 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME23 = 23 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME24 = 24 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME25 = 25 }; \
    FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25)

  #define npf26(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25, PARTYPE26, PARNAME26) \
    enum { _named_parameter_##FNNAME##_##PARNAME1 = 1 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME2 = 2 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME3 = 3 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME4 = 4 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME5 = 5 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME6 = 6 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME7 = 7 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME8 = 8 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME9 = 9 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME10 = 10 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME11 = 11 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME12 = 12 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME13 = 13 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME14 = 14 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME15 = 15 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME16 = 16 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME17 = 17 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME18 = 18 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME19 = 19 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME20 = 20 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME21 = 21 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME22 = 22 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME23 = 23 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME24 = 24 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME25 = 25 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME26 = 26 }; \
    FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25, PARTYPE26 PARNAME26)

  #define npf27(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25, PARTYPE26, PARNAME26, PARTYPE27, PARNAME27) \
    enum { _named_parameter_##FNNAME##_##PARNAME1 = 1 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME2 = 2 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME3 = 3 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME4 = 4 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME5 = 5 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME6 = 6 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME7 = 7 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME8 = 8 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME9 = 9 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME10 = 10 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME11 = 11 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME12 = 12 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME13 = 13 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME14 = 14 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME15 = 15 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME16 = 16 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME17 = 17 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME18 = 18 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME19 = 19 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME20 = 20 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME21 = 21 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME22 = 22 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME23 = 23 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME24 = 24 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME25 = 25 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME26 = 26 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME27 = 27 }; \
    FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25, PARTYPE26 PARNAME26, PARTYPE27 PARNAME27)

  #define npf28(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25, PARTYPE26, PARNAME26, PARTYPE27, PARNAME27, PARTYPE28, PARNAME28) \
    enum { _named_parameter_##FNNAME##_##PARNAME1 = 1 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME2 = 2 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME3 = 3 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME4 = 4 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME5 = 5 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME6 = 6 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME7 = 7 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME8 = 8 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME9 = 9 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME10 = 10 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME11 = 11 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME12 = 12 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME13 = 13 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME14 = 14 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME15 = 15 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME16 = 16 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME17 = 17 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME18 = 18 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME19 = 19 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME20 = 20 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME21 = 21 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME22 = 22 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME23 = 23 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME24 = 24 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME25 = 25 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME26 = 26 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME27 = 27 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME28 = 28 }; \
    FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25, PARTYPE26 PARNAME26, PARTYPE27 PARNAME27, PARTYPE28 PARNAME28)

  #define npf29(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25, PARTYPE26, PARNAME26, PARTYPE27, PARNAME27, PARTYPE28, PARNAME28, PARTYPE29, PARNAME29) \
    enum { _named_parameter_##FNNAME##_##PARNAME1 = 1 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME2 = 2 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME3 = 3 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME4 = 4 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME5 = 5 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME6 = 6 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME7 = 7 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME8 = 8 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME9 = 9 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME10 = 10 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME11 = 11 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME12 = 12 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME13 = 13 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME14 = 14 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME15 = 15 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME16 = 16 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME17 = 17 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME18 = 18 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME19 = 19 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME20 = 20 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME21 = 21 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME22 = 22 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME23 = 23 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME24 = 24 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME25 = 25 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME26 = 26 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME27 = 27 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME28 = 28 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME29 = 29 }; \
    FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25, PARTYPE26 PARNAME26, PARTYPE27 PARNAME27, PARTYPE28 PARNAME28, PARTYPE29 PARNAME29)

  #define npf30(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25, PARTYPE26, PARNAME26, PARTYPE27, PARNAME27, PARTYPE28, PARNAME28, PARTYPE29, PARNAME29, PARTYPE30, PARNAME30) \
    enum { _named_parameter_##FNNAME##_##PARNAME1 = 1 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME2 = 2 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME3 = 3 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME4 = 4 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME5 = 5 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME6 = 6 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME7 = 7 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME8 = 8 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME9 = 9 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME10 = 10 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME11 = 11 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME12 = 12 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME13 = 13 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME14 = 14 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME15 = 15 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME16 = 16 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME17 = 17 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME18 = 18 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME19 = 19 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME20 = 20 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME21 = 21 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME22 = 22 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME23 = 23 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME24 = 24 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME25 = 25 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME26 = 26 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME27 = 27 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME28 = 28 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME29 = 29 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME30 = 30 }; \
    FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25, PARTYPE26 PARNAME26, PARTYPE27 PARNAME27, PARTYPE28 PARNAME28, PARTYPE29 PARNAME29, PARTYPE30 PARNAME30)

  #define npf31(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25, PARTYPE26, PARNAME26, PARTYPE27, PARNAME27, PARTYPE28, PARNAME28, PARTYPE29, PARNAME29, PARTYPE30, PARNAME30, PARTYPE31, PARNAME31) \
    enum { _named_parameter_##FNNAME##_##PARNAME1 = 1 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME2 = 2 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME3 = 3 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME4 = 4 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME5 = 5 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME6 = 6 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME7 = 7 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME8 = 8 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME9 = 9 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME10 = 10 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME11 = 11 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME12 = 12 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME13 = 13 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME14 = 14 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME15 = 15 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME16 = 16 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME17 = 17 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME18 = 18 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME19 = 19 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME20 = 20 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME21 = 21 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME22 = 22 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME23 = 23 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME24 = 24 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME25 = 25 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME26 = 26 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME27 = 27 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME28 = 28 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME29 = 29 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME30 = 30 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME31 = 31 }; \
    FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25, PARTYPE26 PARNAME26, PARTYPE27 PARNAME27, PARTYPE28 PARNAME28, PARTYPE29 PARNAME29, PARTYPE30 PARNAME30, PARTYPE31 PARNAME31)

  #define npf32(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25, PARTYPE26, PARNAME26, PARTYPE27, PARNAME27, PARTYPE28, PARNAME28, PARTYPE29, PARNAME29, PARTYPE30, PARNAME30, PARTYPE31, PARNAME31, PARTYPE32, PARNAME32) \
    enum { _named_parameter_##FNNAME##_##PARNAME1 = 1 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME2 = 2 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME3 = 3 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME4 = 4 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME5 = 5 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME6 = 6 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME7 = 7 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME8 = 8 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME9 = 9 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME10 = 10 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME11 = 11 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME12 = 12 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME13 = 13 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME14 = 14 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME15 = 15 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME16 = 16 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME17 = 17 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME18 = 18 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME19 = 19 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME20 = 20 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME21 = 21 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME22 = 22 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME23 = 23 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME24 = 24 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME25 = 25 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME26 = 26 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME27 = 27 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME28 = 28 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME29 = 29 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME30 = 30 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME31 = 31 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME32 = 32 }; \
    FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25, PARTYPE26 PARNAME26, PARTYPE27 PARNAME27, PARTYPE28 PARNAME28, PARTYPE29 PARNAME29, PARTYPE30 PARNAME30, PARTYPE31 PARNAME31, PARTYPE32 PARNAME32)

  #define npf33(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25, PARTYPE26, PARNAME26, PARTYPE27, PARNAME27, PARTYPE28, PARNAME28, PARTYPE29, PARNAME29, PARTYPE30, PARNAME30, PARTYPE31, PARNAME31, PARTYPE32, PARNAME32, PARTYPE33, PARNAME33) \
    enum { _named_parameter_##FNNAME##_##PARNAME1 = 1 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME2 = 2 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME3 = 3 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME4 = 4 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME5 = 5 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME6 = 6 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME7 = 7 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME8 = 8 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME9 = 9 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME10 = 10 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME11 = 11 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME12 = 12 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME13 = 13 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME14 = 14 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME15 = 15 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME16 = 16 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME17 = 17 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME18 = 18 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME19 = 19 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME20 = 20 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME21 = 21 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME22 = 22 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME23 = 23 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME24 = 24 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME25 = 25 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME26 = 26 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME27 = 27 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME28 = 28 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME29 = 29 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME30 = 30 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME31 = 31 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME32 = 32 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME33 = 33 }; \
    FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25, PARTYPE26 PARNAME26, PARTYPE27 PARNAME27, PARTYPE28 PARNAME28, PARTYPE29 PARNAME29, PARTYPE30 PARNAME30, PARTYPE31 PARNAME31, PARTYPE32 PARNAME32, PARTYPE33 PARNAME33)

  #define npf34(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25, PARTYPE26, PARNAME26, PARTYPE27, PARNAME27, PARTYPE28, PARNAME28, PARTYPE29, PARNAME29, PARTYPE30, PARNAME30, PARTYPE31, PARNAME31, PARTYPE32, PARNAME32, PARTYPE33, PARNAME33, PARTYPE34, PARNAME34) \
    enum { _named_parameter_##FNNAME##_##PARNAME1 = 1 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME2 = 2 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME3 = 3 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME4 = 4 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME5 = 5 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME6 = 6 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME7 = 7 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME8 = 8 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME9 = 9 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME10 = 10 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME11 = 11 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME12 = 12 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME13 = 13 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME14 = 14 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME15 = 15 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME16 = 16 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME17 = 17 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME18 = 18 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME19 = 19 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME20 = 20 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME21 = 21 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME22 = 22 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME23 = 23 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME24 = 24 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME25 = 25 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME26 = 26 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME27 = 27 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME28 = 28 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME29 = 29 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME30 = 30 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME31 = 31 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME32 = 32 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME33 = 33 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME34 = 34 }; \
    FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25, PARTYPE26 PARNAME26, PARTYPE27 PARNAME27, PARTYPE28 PARNAME28, PARTYPE29 PARNAME29, PARTYPE30 PARNAME30, PARTYPE31 PARNAME31, PARTYPE32 PARNAME32, PARTYPE33 PARNAME33, PARTYPE34 PARNAME34)

  #define npf35(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25, PARTYPE26, PARNAME26, PARTYPE27, PARNAME27, PARTYPE28, PARNAME28, PARTYPE29, PARNAME29, PARTYPE30, PARNAME30, PARTYPE31, PARNAME31, PARTYPE32, PARNAME32, PARTYPE33, PARNAME33, PARTYPE34, PARNAME34, PARTYPE35, PARNAME35) \
    enum { _named_parameter_##FNNAME##_##PARNAME1 = 1 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME2 = 2 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME3 = 3 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME4 = 4 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME5 = 5 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME6 = 6 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME7 = 7 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME8 = 8 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME9 = 9 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME10 = 10 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME11 = 11 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME12 = 12 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME13 = 13 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME14 = 14 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME15 = 15 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME16 = 16 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME17 = 17 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME18 = 18 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME19 = 19 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME20 = 20 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME21 = 21 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME22 = 22 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME23 = 23 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME24 = 24 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME25 = 25 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME26 = 26 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME27 = 27 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME28 = 28 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME29 = 29 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME30 = 30 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME31 = 31 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME32 = 32 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME33 = 33 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME34 = 34 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME35 = 35 }; \
    FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25, PARTYPE26 PARNAME26, PARTYPE27 PARNAME27, PARTYPE28 PARNAME28, PARTYPE29 PARNAME29, PARTYPE30 PARNAME30, PARTYPE31 PARNAME31, PARTYPE32 PARNAME32, PARTYPE33 PARNAME33, PARTYPE34 PARNAME34, PARTYPE35 PARNAME35)

  #define npf36(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25, PARTYPE26, PARNAME26, PARTYPE27, PARNAME27, PARTYPE28, PARNAME28, PARTYPE29, PARNAME29, PARTYPE30, PARNAME30, PARTYPE31, PARNAME31, PARTYPE32, PARNAME32, PARTYPE33, PARNAME33, PARTYPE34, PARNAME34, PARTYPE35, PARNAME35, PARTYPE36, PARNAME36) \
    enum { _named_parameter_##FNNAME##_##PARNAME1 = 1 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME2 = 2 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME3 = 3 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME4 = 4 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME5 = 5 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME6 = 6 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME7 = 7 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME8 = 8 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME9 = 9 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME10 = 10 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME11 = 11 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME12 = 12 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME13 = 13 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME14 = 14 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME15 = 15 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME16 = 16 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME17 = 17 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME18 = 18 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME19 = 19 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME20 = 20 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME21 = 21 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME22 = 22 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME23 = 23 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME24 = 24 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME25 = 25 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME26 = 26 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME27 = 27 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME28 = 28 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME29 = 29 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME30 = 30 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME31 = 31 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME32 = 32 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME33 = 33 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME34 = 34 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME35 = 35 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME36 = 36 }; \
    FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25, PARTYPE26 PARNAME26, PARTYPE27 PARNAME27, PARTYPE28 PARNAME28, PARTYPE29 PARNAME29, PARTYPE30 PARNAME30, PARTYPE31 PARNAME31, PARTYPE32 PARNAME32, PARTYPE33 PARNAME33, PARTYPE34 PARNAME34, PARTYPE35 PARNAME35, PARTYPE36 PARNAME36)

  #define npf37(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25, PARTYPE26, PARNAME26, PARTYPE27, PARNAME27, PARTYPE28, PARNAME28, PARTYPE29, PARNAME29, PARTYPE30, PARNAME30, PARTYPE31, PARNAME31, PARTYPE32, PARNAME32, PARTYPE33, PARNAME33, PARTYPE34, PARNAME34, PARTYPE35, PARNAME35, PARTYPE36, PARNAME36, PARTYPE37, PARNAME37) \
    enum { _named_parameter_##FNNAME##_##PARNAME1 = 1 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME2 = 2 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME3 = 3 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME4 = 4 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME5 = 5 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME6 = 6 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME7 = 7 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME8 = 8 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME9 = 9 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME10 = 10 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME11 = 11 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME12 = 12 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME13 = 13 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME14 = 14 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME15 = 15 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME16 = 16 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME17 = 17 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME18 = 18 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME19 = 19 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME20 = 20 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME21 = 21 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME22 = 22 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME23 = 23 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME24 = 24 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME25 = 25 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME26 = 26 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME27 = 27 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME28 = 28 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME29 = 29 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME30 = 30 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME31 = 31 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME32 = 32 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME33 = 33 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME34 = 34 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME35 = 35 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME36 = 36 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME37 = 37 }; \
    FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25, PARTYPE26 PARNAME26, PARTYPE27 PARNAME27, PARTYPE28 PARNAME28, PARTYPE29 PARNAME29, PARTYPE30 PARNAME30, PARTYPE31 PARNAME31, PARTYPE32 PARNAME32, PARTYPE33 PARNAME33, PARTYPE34 PARNAME34, PARTYPE35 PARNAME35, PARTYPE36 PARNAME36, PARTYPE37 PARNAME37)

  #define npf38(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25, PARTYPE26, PARNAME26, PARTYPE27, PARNAME27, PARTYPE28, PARNAME28, PARTYPE29, PARNAME29, PARTYPE30, PARNAME30, PARTYPE31, PARNAME31, PARTYPE32, PARNAME32, PARTYPE33, PARNAME33, PARTYPE34, PARNAME34, PARTYPE35, PARNAME35, PARTYPE36, PARNAME36, PARTYPE37, PARNAME37, PARTYPE38, PARNAME38) \
    enum { _named_parameter_##FNNAME##_##PARNAME1 = 1 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME2 = 2 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME3 = 3 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME4 = 4 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME5 = 5 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME6 = 6 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME7 = 7 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME8 = 8 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME9 = 9 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME10 = 10 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME11 = 11 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME12 = 12 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME13 = 13 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME14 = 14 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME15 = 15 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME16 = 16 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME17 = 17 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME18 = 18 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME19 = 19 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME20 = 20 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME21 = 21 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME22 = 22 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME23 = 23 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME24 = 24 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME25 = 25 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME26 = 26 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME27 = 27 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME28 = 28 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME29 = 29 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME30 = 30 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME31 = 31 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME32 = 32 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME33 = 33 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME34 = 34 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME35 = 35 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME36 = 36 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME37 = 37 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME38 = 38 }; \
    FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25, PARTYPE26 PARNAME26, PARTYPE27 PARNAME27, PARTYPE28 PARNAME28, PARTYPE29 PARNAME29, PARTYPE30 PARNAME30, PARTYPE31 PARNAME31, PARTYPE32 PARNAME32, PARTYPE33 PARNAME33, PARTYPE34 PARNAME34, PARTYPE35 PARNAME35, PARTYPE36 PARNAME36, PARTYPE37 PARNAME37, PARTYPE38 PARNAME38)

  #define npf39(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25, PARTYPE26, PARNAME26, PARTYPE27, PARNAME27, PARTYPE28, PARNAME28, PARTYPE29, PARNAME29, PARTYPE30, PARNAME30, PARTYPE31, PARNAME31, PARTYPE32, PARNAME32, PARTYPE33, PARNAME33, PARTYPE34, PARNAME34, PARTYPE35, PARNAME35, PARTYPE36, PARNAME36, PARTYPE37, PARNAME37, PARTYPE38, PARNAME38, PARTYPE39, PARNAME39) \
    enum { _named_parameter_##FNNAME##_##PARNAME1 = 1 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME2 = 2 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME3 = 3 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME4 = 4 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME5 = 5 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME6 = 6 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME7 = 7 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME8 = 8 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME9 = 9 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME10 = 10 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME11 = 11 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME12 = 12 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME13 = 13 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME14 = 14 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME15 = 15 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME16 = 16 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME17 = 17 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME18 = 18 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME19 = 19 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME20 = 20 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME21 = 21 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME22 = 22 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME23 = 23 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME24 = 24 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME25 = 25 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME26 = 26 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME27 = 27 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME28 = 28 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME29 = 29 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME30 = 30 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME31 = 31 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME32 = 32 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME33 = 33 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME34 = 34 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME35 = 35 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME36 = 36 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME37 = 37 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME38 = 38 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME39 = 39 }; \
    FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25, PARTYPE26 PARNAME26, PARTYPE27 PARNAME27, PARTYPE28 PARNAME28, PARTYPE29 PARNAME29, PARTYPE30 PARNAME30, PARTYPE31 PARNAME31, PARTYPE32 PARNAME32, PARTYPE33 PARNAME33, PARTYPE34 PARNAME34, PARTYPE35 PARNAME35, PARTYPE36 PARNAME36, PARTYPE37 PARNAME37, PARTYPE38 PARNAME38, PARTYPE39 PARNAME39)

  #define npf40(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25, PARTYPE26, PARNAME26, PARTYPE27, PARNAME27, PARTYPE28, PARNAME28, PARTYPE29, PARNAME29, PARTYPE30, PARNAME30, PARTYPE31, PARNAME31, PARTYPE32, PARNAME32, PARTYPE33, PARNAME33, PARTYPE34, PARNAME34, PARTYPE35, PARNAME35, PARTYPE36, PARNAME36, PARTYPE37, PARNAME37, PARTYPE38, PARNAME38, PARTYPE39, PARNAME39, PARTYPE40, PARNAME40) \
    enum { _named_parameter_##FNNAME##_##PARNAME1 = 1 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME2 = 2 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME3 = 3 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME4 = 4 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME5 = 5 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME6 = 6 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME7 = 7 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME8 = 8 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME9 = 9 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME10 = 10 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME11 = 11 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME12 = 12 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME13 = 13 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME14 = 14 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME15 = 15 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME16 = 16 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME17 = 17 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME18 = 18 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME19 = 19 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME20 = 20 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME21 = 21 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME22 = 22 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME23 = 23 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME24 = 24 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME25 = 25 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME26 = 26 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME27 = 27 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME28 = 28 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME29 = 29 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME30 = 30 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME31 = 31 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME32 = 32 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME33 = 33 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME34 = 34 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME35 = 35 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME36 = 36 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME37 = 37 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME38 = 38 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME39 = 39 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME40 = 40 }; \
    FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25, PARTYPE26 PARNAME26, PARTYPE27 PARNAME27, PARTYPE28 PARNAME28, PARTYPE29 PARNAME29, PARTYPE30 PARNAME30, PARTYPE31 PARNAME31, PARTYPE32 PARNAME32, PARTYPE33 PARNAME33, PARTYPE34 PARNAME34, PARTYPE35 PARNAME35, PARTYPE36 PARNAME36, PARTYPE37 PARNAME37, PARTYPE38 PARNAME38, PARTYPE39 PARNAME39, PARTYPE40 PARNAME40)

  #define npf41(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25, PARTYPE26, PARNAME26, PARTYPE27, PARNAME27, PARTYPE28, PARNAME28, PARTYPE29, PARNAME29, PARTYPE30, PARNAME30, PARTYPE31, PARNAME31, PARTYPE32, PARNAME32, PARTYPE33, PARNAME33, PARTYPE34, PARNAME34, PARTYPE35, PARNAME35, PARTYPE36, PARNAME36, PARTYPE37, PARNAME37, PARTYPE38, PARNAME38, PARTYPE39, PARNAME39, PARTYPE40, PARNAME40, PARTYPE41, PARNAME41) \
    enum { _named_parameter_##FNNAME##_##PARNAME1 = 1 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME2 = 2 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME3 = 3 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME4 = 4 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME5 = 5 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME6 = 6 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME7 = 7 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME8 = 8 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME9 = 9 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME10 = 10 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME11 = 11 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME12 = 12 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME13 = 13 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME14 = 14 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME15 = 15 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME16 = 16 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME17 = 17 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME18 = 18 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME19 = 19 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME20 = 20 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME21 = 21 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME22 = 22 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME23 = 23 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME24 = 24 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME25 = 25 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME26 = 26 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME27 = 27 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME28 = 28 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME29 = 29 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME30 = 30 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME31 = 31 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME32 = 32 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME33 = 33 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME34 = 34 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME35 = 35 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME36 = 36 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME37 = 37 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME38 = 38 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME39 = 39 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME40 = 40 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME41 = 41 }; \
    FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25, PARTYPE26 PARNAME26, PARTYPE27 PARNAME27, PARTYPE28 PARNAME28, PARTYPE29 PARNAME29, PARTYPE30 PARNAME30, PARTYPE31 PARNAME31, PARTYPE32 PARNAME32, PARTYPE33 PARNAME33, PARTYPE34 PARNAME34, PARTYPE35 PARNAME35, PARTYPE36 PARNAME36, PARTYPE37 PARNAME37, PARTYPE38 PARNAME38, PARTYPE39 PARNAME39, PARTYPE40 PARNAME40, PARTYPE41 PARNAME41)

  #define npf42(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25, PARTYPE26, PARNAME26, PARTYPE27, PARNAME27, PARTYPE28, PARNAME28, PARTYPE29, PARNAME29, PARTYPE30, PARNAME30, PARTYPE31, PARNAME31, PARTYPE32, PARNAME32, PARTYPE33, PARNAME33, PARTYPE34, PARNAME34, PARTYPE35, PARNAME35, PARTYPE36, PARNAME36, PARTYPE37, PARNAME37, PARTYPE38, PARNAME38, PARTYPE39, PARNAME39, PARTYPE40, PARNAME40, PARTYPE41, PARNAME41, PARTYPE42, PARNAME42) \
    enum { _named_parameter_##FNNAME##_##PARNAME1 = 1 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME2 = 2 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME3 = 3 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME4 = 4 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME5 = 5 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME6 = 6 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME7 = 7 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME8 = 8 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME9 = 9 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME10 = 10 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME11 = 11 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME12 = 12 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME13 = 13 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME14 = 14 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME15 = 15 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME16 = 16 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME17 = 17 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME18 = 18 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME19 = 19 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME20 = 20 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME21 = 21 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME22 = 22 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME23 = 23 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME24 = 24 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME25 = 25 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME26 = 26 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME27 = 27 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME28 = 28 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME29 = 29 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME30 = 30 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME31 = 31 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME32 = 32 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME33 = 33 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME34 = 34 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME35 = 35 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME36 = 36 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME37 = 37 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME38 = 38 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME39 = 39 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME40 = 40 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME41 = 41 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME42 = 42 }; \
    FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25, PARTYPE26 PARNAME26, PARTYPE27 PARNAME27, PARTYPE28 PARNAME28, PARTYPE29 PARNAME29, PARTYPE30 PARNAME30, PARTYPE31 PARNAME31, PARTYPE32 PARNAME32, PARTYPE33 PARNAME33, PARTYPE34 PARNAME34, PARTYPE35 PARNAME35, PARTYPE36 PARNAME36, PARTYPE37 PARNAME37, PARTYPE38 PARNAME38, PARTYPE39 PARNAME39, PARTYPE40 PARNAME40, PARTYPE41 PARNAME41, PARTYPE42 PARNAME42)

  #define npf43(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25, PARTYPE26, PARNAME26, PARTYPE27, PARNAME27, PARTYPE28, PARNAME28, PARTYPE29, PARNAME29, PARTYPE30, PARNAME30, PARTYPE31, PARNAME31, PARTYPE32, PARNAME32, PARTYPE33, PARNAME33, PARTYPE34, PARNAME34, PARTYPE35, PARNAME35, PARTYPE36, PARNAME36, PARTYPE37, PARNAME37, PARTYPE38, PARNAME38, PARTYPE39, PARNAME39, PARTYPE40, PARNAME40, PARTYPE41, PARNAME41, PARTYPE42, PARNAME42, PARTYPE43, PARNAME43) \
    enum { _named_parameter_##FNNAME##_##PARNAME1 = 1 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME2 = 2 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME3 = 3 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME4 = 4 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME5 = 5 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME6 = 6 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME7 = 7 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME8 = 8 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME9 = 9 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME10 = 10 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME11 = 11 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME12 = 12 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME13 = 13 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME14 = 14 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME15 = 15 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME16 = 16 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME17 = 17 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME18 = 18 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME19 = 19 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME20 = 20 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME21 = 21 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME22 = 22 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME23 = 23 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME24 = 24 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME25 = 25 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME26 = 26 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME27 = 27 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME28 = 28 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME29 = 29 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME30 = 30 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME31 = 31 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME32 = 32 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME33 = 33 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME34 = 34 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME35 = 35 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME36 = 36 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME37 = 37 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME38 = 38 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME39 = 39 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME40 = 40 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME41 = 41 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME42 = 42 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME43 = 43 }; \
    FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25, PARTYPE26 PARNAME26, PARTYPE27 PARNAME27, PARTYPE28 PARNAME28, PARTYPE29 PARNAME29, PARTYPE30 PARNAME30, PARTYPE31 PARNAME31, PARTYPE32 PARNAME32, PARTYPE33 PARNAME33, PARTYPE34 PARNAME34, PARTYPE35 PARNAME35, PARTYPE36 PARNAME36, PARTYPE37 PARNAME37, PARTYPE38 PARNAME38, PARTYPE39 PARNAME39, PARTYPE40 PARNAME40, PARTYPE41 PARNAME41, PARTYPE42 PARNAME42, PARTYPE43 PARNAME43)

  #define npf44(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25, PARTYPE26, PARNAME26, PARTYPE27, PARNAME27, PARTYPE28, PARNAME28, PARTYPE29, PARNAME29, PARTYPE30, PARNAME30, PARTYPE31, PARNAME31, PARTYPE32, PARNAME32, PARTYPE33, PARNAME33, PARTYPE34, PARNAME34, PARTYPE35, PARNAME35, PARTYPE36, PARNAME36, PARTYPE37, PARNAME37, PARTYPE38, PARNAME38, PARTYPE39, PARNAME39, PARTYPE40, PARNAME40, PARTYPE41, PARNAME41, PARTYPE42, PARNAME42, PARTYPE43, PARNAME43, PARTYPE44, PARNAME44) \
    enum { _named_parameter_##FNNAME##_##PARNAME1 = 1 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME2 = 2 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME3 = 3 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME4 = 4 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME5 = 5 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME6 = 6 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME7 = 7 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME8 = 8 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME9 = 9 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME10 = 10 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME11 = 11 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME12 = 12 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME13 = 13 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME14 = 14 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME15 = 15 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME16 = 16 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME17 = 17 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME18 = 18 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME19 = 19 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME20 = 20 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME21 = 21 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME22 = 22 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME23 = 23 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME24 = 24 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME25 = 25 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME26 = 26 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME27 = 27 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME28 = 28 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME29 = 29 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME30 = 30 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME31 = 31 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME32 = 32 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME33 = 33 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME34 = 34 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME35 = 35 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME36 = 36 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME37 = 37 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME38 = 38 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME39 = 39 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME40 = 40 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME41 = 41 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME42 = 42 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME43 = 43 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME44 = 44 }; \
    FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25, PARTYPE26 PARNAME26, PARTYPE27 PARNAME27, PARTYPE28 PARNAME28, PARTYPE29 PARNAME29, PARTYPE30 PARNAME30, PARTYPE31 PARNAME31, PARTYPE32 PARNAME32, PARTYPE33 PARNAME33, PARTYPE34 PARNAME34, PARTYPE35 PARNAME35, PARTYPE36 PARNAME36, PARTYPE37 PARNAME37, PARTYPE38 PARNAME38, PARTYPE39 PARNAME39, PARTYPE40 PARNAME40, PARTYPE41 PARNAME41, PARTYPE42 PARNAME42, PARTYPE43 PARNAME43, PARTYPE44 PARNAME44)

  #define npf45(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25, PARTYPE26, PARNAME26, PARTYPE27, PARNAME27, PARTYPE28, PARNAME28, PARTYPE29, PARNAME29, PARTYPE30, PARNAME30, PARTYPE31, PARNAME31, PARTYPE32, PARNAME32, PARTYPE33, PARNAME33, PARTYPE34, PARNAME34, PARTYPE35, PARNAME35, PARTYPE36, PARNAME36, PARTYPE37, PARNAME37, PARTYPE38, PARNAME38, PARTYPE39, PARNAME39, PARTYPE40, PARNAME40, PARTYPE41, PARNAME41, PARTYPE42, PARNAME42, PARTYPE43, PARNAME43, PARTYPE44, PARNAME44, PARTYPE45, PARNAME45) \
    enum { _named_parameter_##FNNAME##_##PARNAME1 = 1 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME2 = 2 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME3 = 3 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME4 = 4 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME5 = 5 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME6 = 6 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME7 = 7 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME8 = 8 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME9 = 9 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME10 = 10 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME11 = 11 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME12 = 12 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME13 = 13 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME14 = 14 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME15 = 15 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME16 = 16 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME17 = 17 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME18 = 18 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME19 = 19 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME20 = 20 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME21 = 21 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME22 = 22 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME23 = 23 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME24 = 24 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME25 = 25 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME26 = 26 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME27 = 27 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME28 = 28 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME29 = 29 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME30 = 30 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME31 = 31 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME32 = 32 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME33 = 33 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME34 = 34 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME35 = 35 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME36 = 36 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME37 = 37 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME38 = 38 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME39 = 39 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME40 = 40 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME41 = 41 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME42 = 42 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME43 = 43 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME44 = 44 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME45 = 45 }; \
    FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25, PARTYPE26 PARNAME26, PARTYPE27 PARNAME27, PARTYPE28 PARNAME28, PARTYPE29 PARNAME29, PARTYPE30 PARNAME30, PARTYPE31 PARNAME31, PARTYPE32 PARNAME32, PARTYPE33 PARNAME33, PARTYPE34 PARNAME34, PARTYPE35 PARNAME35, PARTYPE36 PARNAME36, PARTYPE37 PARNAME37, PARTYPE38 PARNAME38, PARTYPE39 PARNAME39, PARTYPE40 PARNAME40, PARTYPE41 PARNAME41, PARTYPE42 PARNAME42, PARTYPE43 PARNAME43, PARTYPE44 PARNAME44, PARTYPE45 PARNAME45)

  #define npf46(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25, PARTYPE26, PARNAME26, PARTYPE27, PARNAME27, PARTYPE28, PARNAME28, PARTYPE29, PARNAME29, PARTYPE30, PARNAME30, PARTYPE31, PARNAME31, PARTYPE32, PARNAME32, PARTYPE33, PARNAME33, PARTYPE34, PARNAME34, PARTYPE35, PARNAME35, PARTYPE36, PARNAME36, PARTYPE37, PARNAME37, PARTYPE38, PARNAME38, PARTYPE39, PARNAME39, PARTYPE40, PARNAME40, PARTYPE41, PARNAME41, PARTYPE42, PARNAME42, PARTYPE43, PARNAME43, PARTYPE44, PARNAME44, PARTYPE45, PARNAME45, PARTYPE46, PARNAME46) \
    enum { _named_parameter_##FNNAME##_##PARNAME1 = 1 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME2 = 2 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME3 = 3 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME4 = 4 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME5 = 5 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME6 = 6 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME7 = 7 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME8 = 8 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME9 = 9 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME10 = 10 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME11 = 11 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME12 = 12 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME13 = 13 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME14 = 14 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME15 = 15 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME16 = 16 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME17 = 17 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME18 = 18 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME19 = 19 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME20 = 20 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME21 = 21 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME22 = 22 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME23 = 23 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME24 = 24 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME25 = 25 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME26 = 26 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME27 = 27 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME28 = 28 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME29 = 29 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME30 = 30 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME31 = 31 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME32 = 32 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME33 = 33 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME34 = 34 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME35 = 35 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME36 = 36 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME37 = 37 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME38 = 38 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME39 = 39 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME40 = 40 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME41 = 41 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME42 = 42 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME43 = 43 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME44 = 44 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME45 = 45 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME46 = 46 }; \
    FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25, PARTYPE26 PARNAME26, PARTYPE27 PARNAME27, PARTYPE28 PARNAME28, PARTYPE29 PARNAME29, PARTYPE30 PARNAME30, PARTYPE31 PARNAME31, PARTYPE32 PARNAME32, PARTYPE33 PARNAME33, PARTYPE34 PARNAME34, PARTYPE35 PARNAME35, PARTYPE36 PARNAME36, PARTYPE37 PARNAME37, PARTYPE38 PARNAME38, PARTYPE39 PARNAME39, PARTYPE40 PARNAME40, PARTYPE41 PARNAME41, PARTYPE42 PARNAME42, PARTYPE43 PARNAME43, PARTYPE44 PARNAME44, PARTYPE45 PARNAME45, PARTYPE46 PARNAME46)

  #define npf47(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25, PARTYPE26, PARNAME26, PARTYPE27, PARNAME27, PARTYPE28, PARNAME28, PARTYPE29, PARNAME29, PARTYPE30, PARNAME30, PARTYPE31, PARNAME31, PARTYPE32, PARNAME32, PARTYPE33, PARNAME33, PARTYPE34, PARNAME34, PARTYPE35, PARNAME35, PARTYPE36, PARNAME36, PARTYPE37, PARNAME37, PARTYPE38, PARNAME38, PARTYPE39, PARNAME39, PARTYPE40, PARNAME40, PARTYPE41, PARNAME41, PARTYPE42, PARNAME42, PARTYPE43, PARNAME43, PARTYPE44, PARNAME44, PARTYPE45, PARNAME45, PARTYPE46, PARNAME46, PARTYPE47, PARNAME47) \
    enum { _named_parameter_##FNNAME##_##PARNAME1 = 1 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME2 = 2 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME3 = 3 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME4 = 4 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME5 = 5 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME6 = 6 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME7 = 7 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME8 = 8 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME9 = 9 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME10 = 10 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME11 = 11 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME12 = 12 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME13 = 13 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME14 = 14 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME15 = 15 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME16 = 16 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME17 = 17 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME18 = 18 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME19 = 19 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME20 = 20 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME21 = 21 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME22 = 22 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME23 = 23 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME24 = 24 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME25 = 25 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME26 = 26 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME27 = 27 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME28 = 28 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME29 = 29 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME30 = 30 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME31 = 31 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME32 = 32 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME33 = 33 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME34 = 34 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME35 = 35 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME36 = 36 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME37 = 37 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME38 = 38 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME39 = 39 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME40 = 40 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME41 = 41 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME42 = 42 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME43 = 43 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME44 = 44 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME45 = 45 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME46 = 46 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME47 = 47 }; \
    FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25, PARTYPE26 PARNAME26, PARTYPE27 PARNAME27, PARTYPE28 PARNAME28, PARTYPE29 PARNAME29, PARTYPE30 PARNAME30, PARTYPE31 PARNAME31, PARTYPE32 PARNAME32, PARTYPE33 PARNAME33, PARTYPE34 PARNAME34, PARTYPE35 PARNAME35, PARTYPE36 PARNAME36, PARTYPE37 PARNAME37, PARTYPE38 PARNAME38, PARTYPE39 PARNAME39, PARTYPE40 PARNAME40, PARTYPE41 PARNAME41, PARTYPE42 PARNAME42, PARTYPE43 PARNAME43, PARTYPE44 PARNAME44, PARTYPE45 PARNAME45, PARTYPE46 PARNAME46, PARTYPE47 PARNAME47)

  #define npf48(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25, PARTYPE26, PARNAME26, PARTYPE27, PARNAME27, PARTYPE28, PARNAME28, PARTYPE29, PARNAME29, PARTYPE30, PARNAME30, PARTYPE31, PARNAME31, PARTYPE32, PARNAME32, PARTYPE33, PARNAME33, PARTYPE34, PARNAME34, PARTYPE35, PARNAME35, PARTYPE36, PARNAME36, PARTYPE37, PARNAME37, PARTYPE38, PARNAME38, PARTYPE39, PARNAME39, PARTYPE40, PARNAME40, PARTYPE41, PARNAME41, PARTYPE42, PARNAME42, PARTYPE43, PARNAME43, PARTYPE44, PARNAME44, PARTYPE45, PARNAME45, PARTYPE46, PARNAME46, PARTYPE47, PARNAME47, PARTYPE48, PARNAME48) \
    enum { _named_parameter_##FNNAME##_##PARNAME1 = 1 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME2 = 2 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME3 = 3 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME4 = 4 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME5 = 5 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME6 = 6 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME7 = 7 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME8 = 8 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME9 = 9 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME10 = 10 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME11 = 11 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME12 = 12 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME13 = 13 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME14 = 14 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME15 = 15 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME16 = 16 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME17 = 17 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME18 = 18 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME19 = 19 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME20 = 20 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME21 = 21 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME22 = 22 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME23 = 23 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME24 = 24 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME25 = 25 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME26 = 26 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME27 = 27 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME28 = 28 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME29 = 29 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME30 = 30 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME31 = 31 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME32 = 32 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME33 = 33 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME34 = 34 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME35 = 35 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME36 = 36 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME37 = 37 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME38 = 38 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME39 = 39 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME40 = 40 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME41 = 41 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME42 = 42 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME43 = 43 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME44 = 44 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME45 = 45 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME46 = 46 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME47 = 47 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME48 = 48 }; \
    FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25, PARTYPE26 PARNAME26, PARTYPE27 PARNAME27, PARTYPE28 PARNAME28, PARTYPE29 PARNAME29, PARTYPE30 PARNAME30, PARTYPE31 PARNAME31, PARTYPE32 PARNAME32, PARTYPE33 PARNAME33, PARTYPE34 PARNAME34, PARTYPE35 PARNAME35, PARTYPE36 PARNAME36, PARTYPE37 PARNAME37, PARTYPE38 PARNAME38, PARTYPE39 PARNAME39, PARTYPE40 PARNAME40, PARTYPE41 PARNAME41, PARTYPE42 PARNAME42, PARTYPE43 PARNAME43, PARTYPE44 PARNAME44, PARTYPE45 PARNAME45, PARTYPE46 PARNAME46, PARTYPE47 PARNAME47, PARTYPE48 PARNAME48)

  #define npf49(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25, PARTYPE26, PARNAME26, PARTYPE27, PARNAME27, PARTYPE28, PARNAME28, PARTYPE29, PARNAME29, PARTYPE30, PARNAME30, PARTYPE31, PARNAME31, PARTYPE32, PARNAME32, PARTYPE33, PARNAME33, PARTYPE34, PARNAME34, PARTYPE35, PARNAME35, PARTYPE36, PARNAME36, PARTYPE37, PARNAME37, PARTYPE38, PARNAME38, PARTYPE39, PARNAME39, PARTYPE40, PARNAME40, PARTYPE41, PARNAME41, PARTYPE42, PARNAME42, PARTYPE43, PARNAME43, PARTYPE44, PARNAME44, PARTYPE45, PARNAME45, PARTYPE46, PARNAME46, PARTYPE47, PARNAME47, PARTYPE48, PARNAME48, PARTYPE49, PARNAME49) \
    enum { _named_parameter_##FNNAME##_##PARNAME1 = 1 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME2 = 2 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME3 = 3 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME4 = 4 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME5 = 5 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME6 = 6 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME7 = 7 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME8 = 8 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME9 = 9 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME10 = 10 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME11 = 11 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME12 = 12 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME13 = 13 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME14 = 14 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME15 = 15 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME16 = 16 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME17 = 17 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME18 = 18 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME19 = 19 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME20 = 20 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME21 = 21 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME22 = 22 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME23 = 23 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME24 = 24 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME25 = 25 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME26 = 26 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME27 = 27 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME28 = 28 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME29 = 29 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME30 = 30 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME31 = 31 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME32 = 32 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME33 = 33 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME34 = 34 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME35 = 35 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME36 = 36 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME37 = 37 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME38 = 38 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME39 = 39 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME40 = 40 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME41 = 41 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME42 = 42 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME43 = 43 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME44 = 44 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME45 = 45 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME46 = 46 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME47 = 47 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME48 = 48 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME49 = 49 }; \
    FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25, PARTYPE26 PARNAME26, PARTYPE27 PARNAME27, PARTYPE28 PARNAME28, PARTYPE29 PARNAME29, PARTYPE30 PARNAME30, PARTYPE31 PARNAME31, PARTYPE32 PARNAME32, PARTYPE33 PARNAME33, PARTYPE34 PARNAME34, PARTYPE35 PARNAME35, PARTYPE36 PARNAME36, PARTYPE37 PARNAME37, PARTYPE38 PARNAME38, PARTYPE39 PARNAME39, PARTYPE40 PARNAME40, PARTYPE41 PARNAME41, PARTYPE42 PARNAME42, PARTYPE43 PARNAME43, PARTYPE44 PARNAME44, PARTYPE45 PARNAME45, PARTYPE46 PARNAME46, PARTYPE47 PARNAME47, PARTYPE48 PARNAME48, PARTYPE49 PARNAME49)

  #define npf50(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25, PARTYPE26, PARNAME26, PARTYPE27, PARNAME27, PARTYPE28, PARNAME28, PARTYPE29, PARNAME29, PARTYPE30, PARNAME30, PARTYPE31, PARNAME31, PARTYPE32, PARNAME32, PARTYPE33, PARNAME33, PARTYPE34, PARNAME34, PARTYPE35, PARNAME35, PARTYPE36, PARNAME36, PARTYPE37, PARNAME37, PARTYPE38, PARNAME38, PARTYPE39, PARNAME39, PARTYPE40, PARNAME40, PARTYPE41, PARNAME41, PARTYPE42, PARNAME42, PARTYPE43, PARNAME43, PARTYPE44, PARNAME44, PARTYPE45, PARNAME45, PARTYPE46, PARNAME46, PARTYPE47, PARNAME47, PARTYPE48, PARNAME48, PARTYPE49, PARNAME49, PARTYPE50, PARNAME50) \
    enum { _named_parameter_##FNNAME##_##PARNAME1 = 1 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME2 = 2 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME3 = 3 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME4 = 4 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME5 = 5 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME6 = 6 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME7 = 7 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME8 = 8 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME9 = 9 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME10 = 10 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME11 = 11 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME12 = 12 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME13 = 13 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME14 = 14 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME15 = 15 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME16 = 16 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME17 = 17 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME18 = 18 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME19 = 19 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME20 = 20 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME21 = 21 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME22 = 22 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME23 = 23 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME24 = 24 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME25 = 25 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME26 = 26 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME27 = 27 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME28 = 28 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME29 = 29 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME30 = 30 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME31 = 31 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME32 = 32 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME33 = 33 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME34 = 34 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME35 = 35 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME36 = 36 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME37 = 37 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME38 = 38 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME39 = 39 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME40 = 40 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME41 = 41 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME42 = 42 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME43 = 43 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME44 = 44 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME45 = 45 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME46 = 46 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME47 = 47 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME48 = 48 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME49 = 49 }; \
    enum { _named_parameter_##FNNAME##_##PARNAME50 = 50 }; \
    FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25, PARTYPE26 PARNAME26, PARTYPE27 PARNAME27, PARTYPE28 PARNAME28, PARTYPE29 PARNAME29, PARTYPE30 PARNAME30, PARTYPE31 PARNAME31, PARTYPE32 PARNAME32, PARTYPE33 PARNAME33, PARTYPE34 PARNAME34, PARTYPE35 PARNAME35, PARTYPE36 PARNAME36, PARTYPE37 PARNAME37, PARTYPE38 PARNAME38, PARTYPE39 PARNAME39, PARTYPE40 PARNAME40, PARTYPE41 PARNAME41, PARTYPE42 PARNAME42, PARTYPE43 PARNAME43, PARTYPE44 PARNAME44, PARTYPE45 PARNAME45, PARTYPE46 PARNAME46, PARTYPE47 PARNAME47, PARTYPE48 PARNAME48, PARTYPE49 PARNAME49, PARTYPE50 PARNAME50)

#else // #if !defined(DISABLE_NAMED_PARAMETERS)

  #define npf1(FNDECL, FNNAME, PARTYPE1, PARNAME1) FNDECL FNNAME(PARTYPE1 PARNAME1)

  #define npf2(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2) FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2)

  #define npf3(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3) FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3)

  #define npf4(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4) FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4)

  #define npf5(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5) FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5)

  #define npf6(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6) FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6)

  #define npf7(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7) FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7)

  #define npf8(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8) FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8)

  #define npf9(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9) FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9)

  #define npf10(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10) FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10)

  #define npf11(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11) FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11)

  #define npf12(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12) FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12)

  #define npf13(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13) FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13)

  #define npf14(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14) FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14)

  #define npf15(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15) FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15)

  #define npf16(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16) FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16)

  #define npf17(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17) FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17)

  #define npf18(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18) FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18)

  #define npf19(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19) FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19)

  #define npf20(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20) FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20)

  #define npf21(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21) FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21)

  #define npf22(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22) FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22)

  #define npf23(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23) FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23)

  #define npf24(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24) FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24)

  #define npf25(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25) FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25)

  #define npf26(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25, PARTYPE26, PARNAME26) FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25, PARTYPE26 PARNAME26)

  #define npf27(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25, PARTYPE26, PARNAME26, PARTYPE27, PARNAME27) FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25, PARTYPE26 PARNAME26, PARTYPE27 PARNAME27)

  #define npf28(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25, PARTYPE26, PARNAME26, PARTYPE27, PARNAME27, PARTYPE28, PARNAME28) FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25, PARTYPE26 PARNAME26, PARTYPE27 PARNAME27, PARTYPE28 PARNAME28)

  #define npf29(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25, PARTYPE26, PARNAME26, PARTYPE27, PARNAME27, PARTYPE28, PARNAME28, PARTYPE29, PARNAME29) FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25, PARTYPE26 PARNAME26, PARTYPE27 PARNAME27, PARTYPE28 PARNAME28, PARTYPE29 PARNAME29)

  #define npf30(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25, PARTYPE26, PARNAME26, PARTYPE27, PARNAME27, PARTYPE28, PARNAME28, PARTYPE29, PARNAME29, PARTYPE30, PARNAME30) FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25, PARTYPE26 PARNAME26, PARTYPE27 PARNAME27, PARTYPE28 PARNAME28, PARTYPE29 PARNAME29, PARTYPE30 PARNAME30)

  #define npf31(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25, PARTYPE26, PARNAME26, PARTYPE27, PARNAME27, PARTYPE28, PARNAME28, PARTYPE29, PARNAME29, PARTYPE30, PARNAME30, PARTYPE31, PARNAME31) FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25, PARTYPE26 PARNAME26, PARTYPE27 PARNAME27, PARTYPE28 PARNAME28, PARTYPE29 PARNAME29, PARTYPE30 PARNAME30, PARTYPE31 PARNAME31)

  #define npf32(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25, PARTYPE26, PARNAME26, PARTYPE27, PARNAME27, PARTYPE28, PARNAME28, PARTYPE29, PARNAME29, PARTYPE30, PARNAME30, PARTYPE31, PARNAME31, PARTYPE32, PARNAME32) FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25, PARTYPE26 PARNAME26, PARTYPE27 PARNAME27, PARTYPE28 PARNAME28, PARTYPE29 PARNAME29, PARTYPE30 PARNAME30, PARTYPE31 PARNAME31, PARTYPE32 PARNAME32)

  #define npf33(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25, PARTYPE26, PARNAME26, PARTYPE27, PARNAME27, PARTYPE28, PARNAME28, PARTYPE29, PARNAME29, PARTYPE30, PARNAME30, PARTYPE31, PARNAME31, PARTYPE32, PARNAME32, PARTYPE33, PARNAME33) FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25, PARTYPE26 PARNAME26, PARTYPE27 PARNAME27, PARTYPE28 PARNAME28, PARTYPE29 PARNAME29, PARTYPE30 PARNAME30, PARTYPE31 PARNAME31, PARTYPE32 PARNAME32, PARTYPE33 PARNAME33)

  #define npf34(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25, PARTYPE26, PARNAME26, PARTYPE27, PARNAME27, PARTYPE28, PARNAME28, PARTYPE29, PARNAME29, PARTYPE30, PARNAME30, PARTYPE31, PARNAME31, PARTYPE32, PARNAME32, PARTYPE33, PARNAME33, PARTYPE34, PARNAME34) FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25, PARTYPE26 PARNAME26, PARTYPE27 PARNAME27, PARTYPE28 PARNAME28, PARTYPE29 PARNAME29, PARTYPE30 PARNAME30, PARTYPE31 PARNAME31, PARTYPE32 PARNAME32, PARTYPE33 PARNAME33, PARTYPE34 PARNAME34)

  #define npf35(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25, PARTYPE26, PARNAME26, PARTYPE27, PARNAME27, PARTYPE28, PARNAME28, PARTYPE29, PARNAME29, PARTYPE30, PARNAME30, PARTYPE31, PARNAME31, PARTYPE32, PARNAME32, PARTYPE33, PARNAME33, PARTYPE34, PARNAME34, PARTYPE35, PARNAME35) FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25, PARTYPE26 PARNAME26, PARTYPE27 PARNAME27, PARTYPE28 PARNAME28, PARTYPE29 PARNAME29, PARTYPE30 PARNAME30, PARTYPE31 PARNAME31, PARTYPE32 PARNAME32, PARTYPE33 PARNAME33, PARTYPE34 PARNAME34, PARTYPE35 PARNAME35)

  #define npf36(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25, PARTYPE26, PARNAME26, PARTYPE27, PARNAME27, PARTYPE28, PARNAME28, PARTYPE29, PARNAME29, PARTYPE30, PARNAME30, PARTYPE31, PARNAME31, PARTYPE32, PARNAME32, PARTYPE33, PARNAME33, PARTYPE34, PARNAME34, PARTYPE35, PARNAME35, PARTYPE36, PARNAME36) FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25, PARTYPE26 PARNAME26, PARTYPE27 PARNAME27, PARTYPE28 PARNAME28, PARTYPE29 PARNAME29, PARTYPE30 PARNAME30, PARTYPE31 PARNAME31, PARTYPE32 PARNAME32, PARTYPE33 PARNAME33, PARTYPE34 PARNAME34, PARTYPE35 PARNAME35, PARTYPE36 PARNAME36)

  #define npf37(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25, PARTYPE26, PARNAME26, PARTYPE27, PARNAME27, PARTYPE28, PARNAME28, PARTYPE29, PARNAME29, PARTYPE30, PARNAME30, PARTYPE31, PARNAME31, PARTYPE32, PARNAME32, PARTYPE33, PARNAME33, PARTYPE34, PARNAME34, PARTYPE35, PARNAME35, PARTYPE36, PARNAME36, PARTYPE37, PARNAME37) FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25, PARTYPE26 PARNAME26, PARTYPE27 PARNAME27, PARTYPE28 PARNAME28, PARTYPE29 PARNAME29, PARTYPE30 PARNAME30, PARTYPE31 PARNAME31, PARTYPE32 PARNAME32, PARTYPE33 PARNAME33, PARTYPE34 PARNAME34, PARTYPE35 PARNAME35, PARTYPE36 PARNAME36, PARTYPE37 PARNAME37)

  #define npf38(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25, PARTYPE26, PARNAME26, PARTYPE27, PARNAME27, PARTYPE28, PARNAME28, PARTYPE29, PARNAME29, PARTYPE30, PARNAME30, PARTYPE31, PARNAME31, PARTYPE32, PARNAME32, PARTYPE33, PARNAME33, PARTYPE34, PARNAME34, PARTYPE35, PARNAME35, PARTYPE36, PARNAME36, PARTYPE37, PARNAME37, PARTYPE38, PARNAME38) FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25, PARTYPE26 PARNAME26, PARTYPE27 PARNAME27, PARTYPE28 PARNAME28, PARTYPE29 PARNAME29, PARTYPE30 PARNAME30, PARTYPE31 PARNAME31, PARTYPE32 PARNAME32, PARTYPE33 PARNAME33, PARTYPE34 PARNAME34, PARTYPE35 PARNAME35, PARTYPE36 PARNAME36, PARTYPE37 PARNAME37, PARTYPE38 PARNAME38)

  #define npf39(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25, PARTYPE26, PARNAME26, PARTYPE27, PARNAME27, PARTYPE28, PARNAME28, PARTYPE29, PARNAME29, PARTYPE30, PARNAME30, PARTYPE31, PARNAME31, PARTYPE32, PARNAME32, PARTYPE33, PARNAME33, PARTYPE34, PARNAME34, PARTYPE35, PARNAME35, PARTYPE36, PARNAME36, PARTYPE37, PARNAME37, PARTYPE38, PARNAME38, PARTYPE39, PARNAME39) FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25, PARTYPE26 PARNAME26, PARTYPE27 PARNAME27, PARTYPE28 PARNAME28, PARTYPE29 PARNAME29, PARTYPE30 PARNAME30, PARTYPE31 PARNAME31, PARTYPE32 PARNAME32, PARTYPE33 PARNAME33, PARTYPE34 PARNAME34, PARTYPE35 PARNAME35, PARTYPE36 PARNAME36, PARTYPE37 PARNAME37, PARTYPE38 PARNAME38, PARTYPE39 PARNAME39)

  #define npf40(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25, PARTYPE26, PARNAME26, PARTYPE27, PARNAME27, PARTYPE28, PARNAME28, PARTYPE29, PARNAME29, PARTYPE30, PARNAME30, PARTYPE31, PARNAME31, PARTYPE32, PARNAME32, PARTYPE33, PARNAME33, PARTYPE34, PARNAME34, PARTYPE35, PARNAME35, PARTYPE36, PARNAME36, PARTYPE37, PARNAME37, PARTYPE38, PARNAME38, PARTYPE39, PARNAME39, PARTYPE40, PARNAME40) FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25, PARTYPE26 PARNAME26, PARTYPE27 PARNAME27, PARTYPE28 PARNAME28, PARTYPE29 PARNAME29, PARTYPE30 PARNAME30, PARTYPE31 PARNAME31, PARTYPE32 PARNAME32, PARTYPE33 PARNAME33, PARTYPE34 PARNAME34, PARTYPE35 PARNAME35, PARTYPE36 PARNAME36, PARTYPE37 PARNAME37, PARTYPE38 PARNAME38, PARTYPE39 PARNAME39, PARTYPE40 PARNAME40)

  #define npf41(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25, PARTYPE26, PARNAME26, PARTYPE27, PARNAME27, PARTYPE28, PARNAME28, PARTYPE29, PARNAME29, PARTYPE30, PARNAME30, PARTYPE31, PARNAME31, PARTYPE32, PARNAME32, PARTYPE33, PARNAME33, PARTYPE34, PARNAME34, PARTYPE35, PARNAME35, PARTYPE36, PARNAME36, PARTYPE37, PARNAME37, PARTYPE38, PARNAME38, PARTYPE39, PARNAME39, PARTYPE40, PARNAME40, PARTYPE41, PARNAME41) FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25, PARTYPE26 PARNAME26, PARTYPE27 PARNAME27, PARTYPE28 PARNAME28, PARTYPE29 PARNAME29, PARTYPE30 PARNAME30, PARTYPE31 PARNAME31, PARTYPE32 PARNAME32, PARTYPE33 PARNAME33, PARTYPE34 PARNAME34, PARTYPE35 PARNAME35, PARTYPE36 PARNAME36, PARTYPE37 PARNAME37, PARTYPE38 PARNAME38, PARTYPE39 PARNAME39, PARTYPE40 PARNAME40, PARTYPE41 PARNAME41)

  #define npf42(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25, PARTYPE26, PARNAME26, PARTYPE27, PARNAME27, PARTYPE28, PARNAME28, PARTYPE29, PARNAME29, PARTYPE30, PARNAME30, PARTYPE31, PARNAME31, PARTYPE32, PARNAME32, PARTYPE33, PARNAME33, PARTYPE34, PARNAME34, PARTYPE35, PARNAME35, PARTYPE36, PARNAME36, PARTYPE37, PARNAME37, PARTYPE38, PARNAME38, PARTYPE39, PARNAME39, PARTYPE40, PARNAME40, PARTYPE41, PARNAME41, PARTYPE42, PARNAME42) FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25, PARTYPE26 PARNAME26, PARTYPE27 PARNAME27, PARTYPE28 PARNAME28, PARTYPE29 PARNAME29, PARTYPE30 PARNAME30, PARTYPE31 PARNAME31, PARTYPE32 PARNAME32, PARTYPE33 PARNAME33, PARTYPE34 PARNAME34, PARTYPE35 PARNAME35, PARTYPE36 PARNAME36, PARTYPE37 PARNAME37, PARTYPE38 PARNAME38, PARTYPE39 PARNAME39, PARTYPE40 PARNAME40, PARTYPE41 PARNAME41, PARTYPE42 PARNAME42)

  #define npf43(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25, PARTYPE26, PARNAME26, PARTYPE27, PARNAME27, PARTYPE28, PARNAME28, PARTYPE29, PARNAME29, PARTYPE30, PARNAME30, PARTYPE31, PARNAME31, PARTYPE32, PARNAME32, PARTYPE33, PARNAME33, PARTYPE34, PARNAME34, PARTYPE35, PARNAME35, PARTYPE36, PARNAME36, PARTYPE37, PARNAME37, PARTYPE38, PARNAME38, PARTYPE39, PARNAME39, PARTYPE40, PARNAME40, PARTYPE41, PARNAME41, PARTYPE42, PARNAME42, PARTYPE43, PARNAME43) FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25, PARTYPE26 PARNAME26, PARTYPE27 PARNAME27, PARTYPE28 PARNAME28, PARTYPE29 PARNAME29, PARTYPE30 PARNAME30, PARTYPE31 PARNAME31, PARTYPE32 PARNAME32, PARTYPE33 PARNAME33, PARTYPE34 PARNAME34, PARTYPE35 PARNAME35, PARTYPE36 PARNAME36, PARTYPE37 PARNAME37, PARTYPE38 PARNAME38, PARTYPE39 PARNAME39, PARTYPE40 PARNAME40, PARTYPE41 PARNAME41, PARTYPE42 PARNAME42, PARTYPE43 PARNAME43)

  #define npf44(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25, PARTYPE26, PARNAME26, PARTYPE27, PARNAME27, PARTYPE28, PARNAME28, PARTYPE29, PARNAME29, PARTYPE30, PARNAME30, PARTYPE31, PARNAME31, PARTYPE32, PARNAME32, PARTYPE33, PARNAME33, PARTYPE34, PARNAME34, PARTYPE35, PARNAME35, PARTYPE36, PARNAME36, PARTYPE37, PARNAME37, PARTYPE38, PARNAME38, PARTYPE39, PARNAME39, PARTYPE40, PARNAME40, PARTYPE41, PARNAME41, PARTYPE42, PARNAME42, PARTYPE43, PARNAME43, PARTYPE44, PARNAME44) FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25, PARTYPE26 PARNAME26, PARTYPE27 PARNAME27, PARTYPE28 PARNAME28, PARTYPE29 PARNAME29, PARTYPE30 PARNAME30, PARTYPE31 PARNAME31, PARTYPE32 PARNAME32, PARTYPE33 PARNAME33, PARTYPE34 PARNAME34, PARTYPE35 PARNAME35, PARTYPE36 PARNAME36, PARTYPE37 PARNAME37, PARTYPE38 PARNAME38, PARTYPE39 PARNAME39, PARTYPE40 PARNAME40, PARTYPE41 PARNAME41, PARTYPE42 PARNAME42, PARTYPE43 PARNAME43, PARTYPE44 PARNAME44)

  #define npf45(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25, PARTYPE26, PARNAME26, PARTYPE27, PARNAME27, PARTYPE28, PARNAME28, PARTYPE29, PARNAME29, PARTYPE30, PARNAME30, PARTYPE31, PARNAME31, PARTYPE32, PARNAME32, PARTYPE33, PARNAME33, PARTYPE34, PARNAME34, PARTYPE35, PARNAME35, PARTYPE36, PARNAME36, PARTYPE37, PARNAME37, PARTYPE38, PARNAME38, PARTYPE39, PARNAME39, PARTYPE40, PARNAME40, PARTYPE41, PARNAME41, PARTYPE42, PARNAME42, PARTYPE43, PARNAME43, PARTYPE44, PARNAME44, PARTYPE45, PARNAME45) FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25, PARTYPE26 PARNAME26, PARTYPE27 PARNAME27, PARTYPE28 PARNAME28, PARTYPE29 PARNAME29, PARTYPE30 PARNAME30, PARTYPE31 PARNAME31, PARTYPE32 PARNAME32, PARTYPE33 PARNAME33, PARTYPE34 PARNAME34, PARTYPE35 PARNAME35, PARTYPE36 PARNAME36, PARTYPE37 PARNAME37, PARTYPE38 PARNAME38, PARTYPE39 PARNAME39, PARTYPE40 PARNAME40, PARTYPE41 PARNAME41, PARTYPE42 PARNAME42, PARTYPE43 PARNAME43, PARTYPE44 PARNAME44, PARTYPE45 PARNAME45)

  #define npf46(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25, PARTYPE26, PARNAME26, PARTYPE27, PARNAME27, PARTYPE28, PARNAME28, PARTYPE29, PARNAME29, PARTYPE30, PARNAME30, PARTYPE31, PARNAME31, PARTYPE32, PARNAME32, PARTYPE33, PARNAME33, PARTYPE34, PARNAME34, PARTYPE35, PARNAME35, PARTYPE36, PARNAME36, PARTYPE37, PARNAME37, PARTYPE38, PARNAME38, PARTYPE39, PARNAME39, PARTYPE40, PARNAME40, PARTYPE41, PARNAME41, PARTYPE42, PARNAME42, PARTYPE43, PARNAME43, PARTYPE44, PARNAME44, PARTYPE45, PARNAME45, PARTYPE46, PARNAME46) FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25, PARTYPE26 PARNAME26, PARTYPE27 PARNAME27, PARTYPE28 PARNAME28, PARTYPE29 PARNAME29, PARTYPE30 PARNAME30, PARTYPE31 PARNAME31, PARTYPE32 PARNAME32, PARTYPE33 PARNAME33, PARTYPE34 PARNAME34, PARTYPE35 PARNAME35, PARTYPE36 PARNAME36, PARTYPE37 PARNAME37, PARTYPE38 PARNAME38, PARTYPE39 PARNAME39, PARTYPE40 PARNAME40, PARTYPE41 PARNAME41, PARTYPE42 PARNAME42, PARTYPE43 PARNAME43, PARTYPE44 PARNAME44, PARTYPE45 PARNAME45, PARTYPE46 PARNAME46)

  #define npf47(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25, PARTYPE26, PARNAME26, PARTYPE27, PARNAME27, PARTYPE28, PARNAME28, PARTYPE29, PARNAME29, PARTYPE30, PARNAME30, PARTYPE31, PARNAME31, PARTYPE32, PARNAME32, PARTYPE33, PARNAME33, PARTYPE34, PARNAME34, PARTYPE35, PARNAME35, PARTYPE36, PARNAME36, PARTYPE37, PARNAME37, PARTYPE38, PARNAME38, PARTYPE39, PARNAME39, PARTYPE40, PARNAME40, PARTYPE41, PARNAME41, PARTYPE42, PARNAME42, PARTYPE43, PARNAME43, PARTYPE44, PARNAME44, PARTYPE45, PARNAME45, PARTYPE46, PARNAME46, PARTYPE47, PARNAME47) FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25, PARTYPE26 PARNAME26, PARTYPE27 PARNAME27, PARTYPE28 PARNAME28, PARTYPE29 PARNAME29, PARTYPE30 PARNAME30, PARTYPE31 PARNAME31, PARTYPE32 PARNAME32, PARTYPE33 PARNAME33, PARTYPE34 PARNAME34, PARTYPE35 PARNAME35, PARTYPE36 PARNAME36, PARTYPE37 PARNAME37, PARTYPE38 PARNAME38, PARTYPE39 PARNAME39, PARTYPE40 PARNAME40, PARTYPE41 PARNAME41, PARTYPE42 PARNAME42, PARTYPE43 PARNAME43, PARTYPE44 PARNAME44, PARTYPE45 PARNAME45, PARTYPE46 PARNAME46, PARTYPE47 PARNAME47)

  #define npf48(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25, PARTYPE26, PARNAME26, PARTYPE27, PARNAME27, PARTYPE28, PARNAME28, PARTYPE29, PARNAME29, PARTYPE30, PARNAME30, PARTYPE31, PARNAME31, PARTYPE32, PARNAME32, PARTYPE33, PARNAME33, PARTYPE34, PARNAME34, PARTYPE35, PARNAME35, PARTYPE36, PARNAME36, PARTYPE37, PARNAME37, PARTYPE38, PARNAME38, PARTYPE39, PARNAME39, PARTYPE40, PARNAME40, PARTYPE41, PARNAME41, PARTYPE42, PARNAME42, PARTYPE43, PARNAME43, PARTYPE44, PARNAME44, PARTYPE45, PARNAME45, PARTYPE46, PARNAME46, PARTYPE47, PARNAME47, PARTYPE48, PARNAME48) FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25, PARTYPE26 PARNAME26, PARTYPE27 PARNAME27, PARTYPE28 PARNAME28, PARTYPE29 PARNAME29, PARTYPE30 PARNAME30, PARTYPE31 PARNAME31, PARTYPE32 PARNAME32, PARTYPE33 PARNAME33, PARTYPE34 PARNAME34, PARTYPE35 PARNAME35, PARTYPE36 PARNAME36, PARTYPE37 PARNAME37, PARTYPE38 PARNAME38, PARTYPE39 PARNAME39, PARTYPE40 PARNAME40, PARTYPE41 PARNAME41, PARTYPE42 PARNAME42, PARTYPE43 PARNAME43, PARTYPE44 PARNAME44, PARTYPE45 PARNAME45, PARTYPE46 PARNAME46, PARTYPE47 PARNAME47, PARTYPE48 PARNAME48)

  #define npf49(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25, PARTYPE26, PARNAME26, PARTYPE27, PARNAME27, PARTYPE28, PARNAME28, PARTYPE29, PARNAME29, PARTYPE30, PARNAME30, PARTYPE31, PARNAME31, PARTYPE32, PARNAME32, PARTYPE33, PARNAME33, PARTYPE34, PARNAME34, PARTYPE35, PARNAME35, PARTYPE36, PARNAME36, PARTYPE37, PARNAME37, PARTYPE38, PARNAME38, PARTYPE39, PARNAME39, PARTYPE40, PARNAME40, PARTYPE41, PARNAME41, PARTYPE42, PARNAME42, PARTYPE43, PARNAME43, PARTYPE44, PARNAME44, PARTYPE45, PARNAME45, PARTYPE46, PARNAME46, PARTYPE47, PARNAME47, PARTYPE48, PARNAME48, PARTYPE49, PARNAME49) FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25, PARTYPE26 PARNAME26, PARTYPE27 PARNAME27, PARTYPE28 PARNAME28, PARTYPE29 PARNAME29, PARTYPE30 PARNAME30, PARTYPE31 PARNAME31, PARTYPE32 PARNAME32, PARTYPE33 PARNAME33, PARTYPE34 PARNAME34, PARTYPE35 PARNAME35, PARTYPE36 PARNAME36, PARTYPE37 PARNAME37, PARTYPE38 PARNAME38, PARTYPE39 PARNAME39, PARTYPE40 PARNAME40, PARTYPE41 PARNAME41, PARTYPE42 PARNAME42, PARTYPE43 PARNAME43, PARTYPE44 PARNAME44, PARTYPE45 PARNAME45, PARTYPE46 PARNAME46, PARTYPE47 PARNAME47, PARTYPE48 PARNAME48, PARTYPE49 PARNAME49)

  #define npf50(FNDECL, FNNAME, PARTYPE1, PARNAME1, PARTYPE2, PARNAME2, PARTYPE3, PARNAME3, PARTYPE4, PARNAME4, PARTYPE5, PARNAME5, PARTYPE6, PARNAME6, PARTYPE7, PARNAME7, PARTYPE8, PARNAME8, PARTYPE9, PARNAME9, PARTYPE10, PARNAME10, PARTYPE11, PARNAME11, PARTYPE12, PARNAME12, PARTYPE13, PARNAME13, PARTYPE14, PARNAME14, PARTYPE15, PARNAME15, PARTYPE16, PARNAME16, PARTYPE17, PARNAME17, PARTYPE18, PARNAME18, PARTYPE19, PARNAME19, PARTYPE20, PARNAME20, PARTYPE21, PARNAME21, PARTYPE22, PARNAME22, PARTYPE23, PARNAME23, PARTYPE24, PARNAME24, PARTYPE25, PARNAME25, PARTYPE26, PARNAME26, PARTYPE27, PARNAME27, PARTYPE28, PARNAME28, PARTYPE29, PARNAME29, PARTYPE30, PARNAME30, PARTYPE31, PARNAME31, PARTYPE32, PARNAME32, PARTYPE33, PARNAME33, PARTYPE34, PARNAME34, PARTYPE35, PARNAME35, PARTYPE36, PARNAME36, PARTYPE37, PARNAME37, PARTYPE38, PARNAME38, PARTYPE39, PARNAME39, PARTYPE40, PARNAME40, PARTYPE41, PARNAME41, PARTYPE42, PARNAME42, PARTYPE43, PARNAME43, PARTYPE44, PARNAME44, PARTYPE45, PARNAME45, PARTYPE46, PARNAME46, PARTYPE47, PARNAME47, PARTYPE48, PARNAME48, PARTYPE49, PARNAME49, PARTYPE50, PARNAME50) FNDECL FNNAME(PARTYPE1 PARNAME1, PARTYPE2 PARNAME2, PARTYPE3 PARNAME3, PARTYPE4 PARNAME4, PARTYPE5 PARNAME5, PARTYPE6 PARNAME6, PARTYPE7 PARNAME7, PARTYPE8 PARNAME8, PARTYPE9 PARNAME9, PARTYPE10 PARNAME10, PARTYPE11 PARNAME11, PARTYPE12 PARNAME12, PARTYPE13 PARNAME13, PARTYPE14 PARNAME14, PARTYPE15 PARNAME15, PARTYPE16 PARNAME16, PARTYPE17 PARNAME17, PARTYPE18 PARNAME18, PARTYPE19 PARNAME19, PARTYPE20 PARNAME20, PARTYPE21 PARNAME21, PARTYPE22 PARNAME22, PARTYPE23 PARNAME23, PARTYPE24 PARNAME24, PARTYPE25 PARNAME25, PARTYPE26 PARNAME26, PARTYPE27 PARNAME27, PARTYPE28 PARNAME28, PARTYPE29 PARNAME29, PARTYPE30 PARNAME30, PARTYPE31 PARNAME31, PARTYPE32 PARNAME32, PARTYPE33 PARNAME33, PARTYPE34 PARNAME34, PARTYPE35 PARNAME35, PARTYPE36 PARNAME36, PARTYPE37 PARNAME37, PARTYPE38 PARNAME38, PARTYPE39 PARNAME39, PARTYPE40 PARNAME40, PARTYPE41 PARNAME41, PARTYPE42 PARNAME42, PARTYPE43 PARNAME43, PARTYPE44 PARNAME44, PARTYPE45 PARNAME45, PARTYPE46 PARNAME46, PARTYPE47 PARNAME47, PARTYPE48 PARNAME48, PARTYPE49 PARNAME49, PARTYPE50 PARNAME50)

#endif // #if !defined(DISABLE_NAMED_PARAMETERS)

// np

#if !defined(DISABLE_NAMED_PARAMETERS)

  #define np1(FNNAME, PARNAME1, PARVAL1) FNNAME(PARVAL1); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter.")

  #define np2(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2) FNNAME(PARVAL1, PARVAL2); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter.")

  #define np3(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3) FNNAME(PARVAL1, PARVAL2, PARVAL3); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter.")

  #define np4(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter.")

  #define np5(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter.")

  #define np6(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter.")

  #define np7(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter.")

  #define np8(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter.")

  #define np9(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter.")

  #define np10(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter.")

  #define np11(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter.")

  #define np12(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter.")

  #define np13(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter.")

  #define np14(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter.")

  #define np15(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter.")

  #define np16(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter.")

  #define np17(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter.")

  #define np18(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter.")

  #define np19(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter.")

  #define np20(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter.")

  #define np21(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter.")

  #define np22(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter.")

  #define np23(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter.")

  #define np24(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter.")

  #define np25(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter.")

  #define np26(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME26 == 26, "Error: named parameter #26 " #PARNAME26 " is a wrong name for the parameter.")

  #define np27(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME26 == 26, "Error: named parameter #26 " #PARNAME26 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME27 == 27, "Error: named parameter #27 " #PARNAME27 " is a wrong name for the parameter.")

  #define np28(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME26 == 26, "Error: named parameter #26 " #PARNAME26 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME27 == 27, "Error: named parameter #27 " #PARNAME27 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME28 == 28, "Error: named parameter #28 " #PARNAME28 " is a wrong name for the parameter.")

  #define np29(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME26 == 26, "Error: named parameter #26 " #PARNAME26 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME27 == 27, "Error: named parameter #27 " #PARNAME27 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME28 == 28, "Error: named parameter #28 " #PARNAME28 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME29 == 29, "Error: named parameter #29 " #PARNAME29 " is a wrong name for the parameter.")

  #define np30(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME26 == 26, "Error: named parameter #26 " #PARNAME26 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME27 == 27, "Error: named parameter #27 " #PARNAME27 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME28 == 28, "Error: named parameter #28 " #PARNAME28 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME29 == 29, "Error: named parameter #29 " #PARNAME29 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME30 == 30, "Error: named parameter #30 " #PARNAME30 " is a wrong name for the parameter.")

  #define np31(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME26 == 26, "Error: named parameter #26 " #PARNAME26 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME27 == 27, "Error: named parameter #27 " #PARNAME27 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME28 == 28, "Error: named parameter #28 " #PARNAME28 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME29 == 29, "Error: named parameter #29 " #PARNAME29 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME30 == 30, "Error: named parameter #30 " #PARNAME30 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME31 == 31, "Error: named parameter #31 " #PARNAME31 " is a wrong name for the parameter.")

  #define np32(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME26 == 26, "Error: named parameter #26 " #PARNAME26 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME27 == 27, "Error: named parameter #27 " #PARNAME27 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME28 == 28, "Error: named parameter #28 " #PARNAME28 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME29 == 29, "Error: named parameter #29 " #PARNAME29 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME30 == 30, "Error: named parameter #30 " #PARNAME30 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME31 == 31, "Error: named parameter #31 " #PARNAME31 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME32 == 32, "Error: named parameter #32 " #PARNAME32 " is a wrong name for the parameter.")

  #define np33(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME26 == 26, "Error: named parameter #26 " #PARNAME26 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME27 == 27, "Error: named parameter #27 " #PARNAME27 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME28 == 28, "Error: named parameter #28 " #PARNAME28 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME29 == 29, "Error: named parameter #29 " #PARNAME29 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME30 == 30, "Error: named parameter #30 " #PARNAME30 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME31 == 31, "Error: named parameter #31 " #PARNAME31 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME32 == 32, "Error: named parameter #32 " #PARNAME32 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME33 == 33, "Error: named parameter #33 " #PARNAME33 " is a wrong name for the parameter.")

  #define np34(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME26 == 26, "Error: named parameter #26 " #PARNAME26 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME27 == 27, "Error: named parameter #27 " #PARNAME27 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME28 == 28, "Error: named parameter #28 " #PARNAME28 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME29 == 29, "Error: named parameter #29 " #PARNAME29 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME30 == 30, "Error: named parameter #30 " #PARNAME30 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME31 == 31, "Error: named parameter #31 " #PARNAME31 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME32 == 32, "Error: named parameter #32 " #PARNAME32 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME33 == 33, "Error: named parameter #33 " #PARNAME33 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME34 == 34, "Error: named parameter #34 " #PARNAME34 " is a wrong name for the parameter.")

  #define np35(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME26 == 26, "Error: named parameter #26 " #PARNAME26 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME27 == 27, "Error: named parameter #27 " #PARNAME27 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME28 == 28, "Error: named parameter #28 " #PARNAME28 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME29 == 29, "Error: named parameter #29 " #PARNAME29 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME30 == 30, "Error: named parameter #30 " #PARNAME30 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME31 == 31, "Error: named parameter #31 " #PARNAME31 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME32 == 32, "Error: named parameter #32 " #PARNAME32 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME33 == 33, "Error: named parameter #33 " #PARNAME33 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME34 == 34, "Error: named parameter #34 " #PARNAME34 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME35 == 35, "Error: named parameter #35 " #PARNAME35 " is a wrong name for the parameter.")

  #define np36(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME26 == 26, "Error: named parameter #26 " #PARNAME26 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME27 == 27, "Error: named parameter #27 " #PARNAME27 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME28 == 28, "Error: named parameter #28 " #PARNAME28 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME29 == 29, "Error: named parameter #29 " #PARNAME29 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME30 == 30, "Error: named parameter #30 " #PARNAME30 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME31 == 31, "Error: named parameter #31 " #PARNAME31 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME32 == 32, "Error: named parameter #32 " #PARNAME32 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME33 == 33, "Error: named parameter #33 " #PARNAME33 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME34 == 34, "Error: named parameter #34 " #PARNAME34 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME35 == 35, "Error: named parameter #35 " #PARNAME35 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME36 == 36, "Error: named parameter #36 " #PARNAME36 " is a wrong name for the parameter.")

  #define np37(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME26 == 26, "Error: named parameter #26 " #PARNAME26 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME27 == 27, "Error: named parameter #27 " #PARNAME27 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME28 == 28, "Error: named parameter #28 " #PARNAME28 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME29 == 29, "Error: named parameter #29 " #PARNAME29 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME30 == 30, "Error: named parameter #30 " #PARNAME30 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME31 == 31, "Error: named parameter #31 " #PARNAME31 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME32 == 32, "Error: named parameter #32 " #PARNAME32 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME33 == 33, "Error: named parameter #33 " #PARNAME33 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME34 == 34, "Error: named parameter #34 " #PARNAME34 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME35 == 35, "Error: named parameter #35 " #PARNAME35 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME36 == 36, "Error: named parameter #36 " #PARNAME36 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME37 == 37, "Error: named parameter #37 " #PARNAME37 " is a wrong name for the parameter.")

  #define np38(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME26 == 26, "Error: named parameter #26 " #PARNAME26 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME27 == 27, "Error: named parameter #27 " #PARNAME27 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME28 == 28, "Error: named parameter #28 " #PARNAME28 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME29 == 29, "Error: named parameter #29 " #PARNAME29 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME30 == 30, "Error: named parameter #30 " #PARNAME30 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME31 == 31, "Error: named parameter #31 " #PARNAME31 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME32 == 32, "Error: named parameter #32 " #PARNAME32 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME33 == 33, "Error: named parameter #33 " #PARNAME33 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME34 == 34, "Error: named parameter #34 " #PARNAME34 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME35 == 35, "Error: named parameter #35 " #PARNAME35 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME36 == 36, "Error: named parameter #36 " #PARNAME36 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME37 == 37, "Error: named parameter #37 " #PARNAME37 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME38 == 38, "Error: named parameter #38 " #PARNAME38 " is a wrong name for the parameter.")

  #define np39(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38, PARNAME39, PARVAL39) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38, PARVAL39); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME26 == 26, "Error: named parameter #26 " #PARNAME26 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME27 == 27, "Error: named parameter #27 " #PARNAME27 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME28 == 28, "Error: named parameter #28 " #PARNAME28 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME29 == 29, "Error: named parameter #29 " #PARNAME29 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME30 == 30, "Error: named parameter #30 " #PARNAME30 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME31 == 31, "Error: named parameter #31 " #PARNAME31 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME32 == 32, "Error: named parameter #32 " #PARNAME32 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME33 == 33, "Error: named parameter #33 " #PARNAME33 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME34 == 34, "Error: named parameter #34 " #PARNAME34 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME35 == 35, "Error: named parameter #35 " #PARNAME35 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME36 == 36, "Error: named parameter #36 " #PARNAME36 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME37 == 37, "Error: named parameter #37 " #PARNAME37 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME38 == 38, "Error: named parameter #38 " #PARNAME38 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME39 == 39, "Error: named parameter #39 " #PARNAME39 " is a wrong name for the parameter.")

  #define np40(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38, PARNAME39, PARVAL39, PARNAME40, PARVAL40) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38, PARVAL39, PARVAL40); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME26 == 26, "Error: named parameter #26 " #PARNAME26 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME27 == 27, "Error: named parameter #27 " #PARNAME27 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME28 == 28, "Error: named parameter #28 " #PARNAME28 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME29 == 29, "Error: named parameter #29 " #PARNAME29 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME30 == 30, "Error: named parameter #30 " #PARNAME30 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME31 == 31, "Error: named parameter #31 " #PARNAME31 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME32 == 32, "Error: named parameter #32 " #PARNAME32 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME33 == 33, "Error: named parameter #33 " #PARNAME33 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME34 == 34, "Error: named parameter #34 " #PARNAME34 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME35 == 35, "Error: named parameter #35 " #PARNAME35 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME36 == 36, "Error: named parameter #36 " #PARNAME36 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME37 == 37, "Error: named parameter #37 " #PARNAME37 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME38 == 38, "Error: named parameter #38 " #PARNAME38 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME39 == 39, "Error: named parameter #39 " #PARNAME39 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME40 == 40, "Error: named parameter #40 " #PARNAME40 " is a wrong name for the parameter.")

  #define np41(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38, PARNAME39, PARVAL39, PARNAME40, PARVAL40, PARNAME41, PARVAL41) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38, PARVAL39, PARVAL40, PARVAL41); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME26 == 26, "Error: named parameter #26 " #PARNAME26 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME27 == 27, "Error: named parameter #27 " #PARNAME27 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME28 == 28, "Error: named parameter #28 " #PARNAME28 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME29 == 29, "Error: named parameter #29 " #PARNAME29 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME30 == 30, "Error: named parameter #30 " #PARNAME30 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME31 == 31, "Error: named parameter #31 " #PARNAME31 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME32 == 32, "Error: named parameter #32 " #PARNAME32 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME33 == 33, "Error: named parameter #33 " #PARNAME33 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME34 == 34, "Error: named parameter #34 " #PARNAME34 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME35 == 35, "Error: named parameter #35 " #PARNAME35 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME36 == 36, "Error: named parameter #36 " #PARNAME36 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME37 == 37, "Error: named parameter #37 " #PARNAME37 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME38 == 38, "Error: named parameter #38 " #PARNAME38 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME39 == 39, "Error: named parameter #39 " #PARNAME39 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME40 == 40, "Error: named parameter #40 " #PARNAME40 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME41 == 41, "Error: named parameter #41 " #PARNAME41 " is a wrong name for the parameter.")

  #define np42(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38, PARNAME39, PARVAL39, PARNAME40, PARVAL40, PARNAME41, PARVAL41, PARNAME42, PARVAL42) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38, PARVAL39, PARVAL40, PARVAL41, PARVAL42); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME26 == 26, "Error: named parameter #26 " #PARNAME26 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME27 == 27, "Error: named parameter #27 " #PARNAME27 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME28 == 28, "Error: named parameter #28 " #PARNAME28 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME29 == 29, "Error: named parameter #29 " #PARNAME29 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME30 == 30, "Error: named parameter #30 " #PARNAME30 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME31 == 31, "Error: named parameter #31 " #PARNAME31 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME32 == 32, "Error: named parameter #32 " #PARNAME32 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME33 == 33, "Error: named parameter #33 " #PARNAME33 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME34 == 34, "Error: named parameter #34 " #PARNAME34 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME35 == 35, "Error: named parameter #35 " #PARNAME35 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME36 == 36, "Error: named parameter #36 " #PARNAME36 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME37 == 37, "Error: named parameter #37 " #PARNAME37 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME38 == 38, "Error: named parameter #38 " #PARNAME38 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME39 == 39, "Error: named parameter #39 " #PARNAME39 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME40 == 40, "Error: named parameter #40 " #PARNAME40 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME41 == 41, "Error: named parameter #41 " #PARNAME41 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME42 == 42, "Error: named parameter #42 " #PARNAME42 " is a wrong name for the parameter.")

  #define np43(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38, PARNAME39, PARVAL39, PARNAME40, PARVAL40, PARNAME41, PARVAL41, PARNAME42, PARVAL42, PARNAME43, PARVAL43) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38, PARVAL39, PARVAL40, PARVAL41, PARVAL42, PARVAL43); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME26 == 26, "Error: named parameter #26 " #PARNAME26 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME27 == 27, "Error: named parameter #27 " #PARNAME27 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME28 == 28, "Error: named parameter #28 " #PARNAME28 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME29 == 29, "Error: named parameter #29 " #PARNAME29 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME30 == 30, "Error: named parameter #30 " #PARNAME30 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME31 == 31, "Error: named parameter #31 " #PARNAME31 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME32 == 32, "Error: named parameter #32 " #PARNAME32 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME33 == 33, "Error: named parameter #33 " #PARNAME33 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME34 == 34, "Error: named parameter #34 " #PARNAME34 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME35 == 35, "Error: named parameter #35 " #PARNAME35 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME36 == 36, "Error: named parameter #36 " #PARNAME36 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME37 == 37, "Error: named parameter #37 " #PARNAME37 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME38 == 38, "Error: named parameter #38 " #PARNAME38 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME39 == 39, "Error: named parameter #39 " #PARNAME39 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME40 == 40, "Error: named parameter #40 " #PARNAME40 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME41 == 41, "Error: named parameter #41 " #PARNAME41 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME42 == 42, "Error: named parameter #42 " #PARNAME42 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME43 == 43, "Error: named parameter #43 " #PARNAME43 " is a wrong name for the parameter.")

  #define np44(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38, PARNAME39, PARVAL39, PARNAME40, PARVAL40, PARNAME41, PARVAL41, PARNAME42, PARVAL42, PARNAME43, PARVAL43, PARNAME44, PARVAL44) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38, PARVAL39, PARVAL40, PARVAL41, PARVAL42, PARVAL43, PARVAL44); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME26 == 26, "Error: named parameter #26 " #PARNAME26 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME27 == 27, "Error: named parameter #27 " #PARNAME27 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME28 == 28, "Error: named parameter #28 " #PARNAME28 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME29 == 29, "Error: named parameter #29 " #PARNAME29 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME30 == 30, "Error: named parameter #30 " #PARNAME30 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME31 == 31, "Error: named parameter #31 " #PARNAME31 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME32 == 32, "Error: named parameter #32 " #PARNAME32 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME33 == 33, "Error: named parameter #33 " #PARNAME33 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME34 == 34, "Error: named parameter #34 " #PARNAME34 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME35 == 35, "Error: named parameter #35 " #PARNAME35 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME36 == 36, "Error: named parameter #36 " #PARNAME36 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME37 == 37, "Error: named parameter #37 " #PARNAME37 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME38 == 38, "Error: named parameter #38 " #PARNAME38 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME39 == 39, "Error: named parameter #39 " #PARNAME39 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME40 == 40, "Error: named parameter #40 " #PARNAME40 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME41 == 41, "Error: named parameter #41 " #PARNAME41 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME42 == 42, "Error: named parameter #42 " #PARNAME42 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME43 == 43, "Error: named parameter #43 " #PARNAME43 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME44 == 44, "Error: named parameter #44 " #PARNAME44 " is a wrong name for the parameter.")

  #define np45(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38, PARNAME39, PARVAL39, PARNAME40, PARVAL40, PARNAME41, PARVAL41, PARNAME42, PARVAL42, PARNAME43, PARVAL43, PARNAME44, PARVAL44, PARNAME45, PARVAL45) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38, PARVAL39, PARVAL40, PARVAL41, PARVAL42, PARVAL43, PARVAL44, PARVAL45); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME26 == 26, "Error: named parameter #26 " #PARNAME26 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME27 == 27, "Error: named parameter #27 " #PARNAME27 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME28 == 28, "Error: named parameter #28 " #PARNAME28 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME29 == 29, "Error: named parameter #29 " #PARNAME29 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME30 == 30, "Error: named parameter #30 " #PARNAME30 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME31 == 31, "Error: named parameter #31 " #PARNAME31 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME32 == 32, "Error: named parameter #32 " #PARNAME32 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME33 == 33, "Error: named parameter #33 " #PARNAME33 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME34 == 34, "Error: named parameter #34 " #PARNAME34 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME35 == 35, "Error: named parameter #35 " #PARNAME35 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME36 == 36, "Error: named parameter #36 " #PARNAME36 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME37 == 37, "Error: named parameter #37 " #PARNAME37 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME38 == 38, "Error: named parameter #38 " #PARNAME38 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME39 == 39, "Error: named parameter #39 " #PARNAME39 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME40 == 40, "Error: named parameter #40 " #PARNAME40 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME41 == 41, "Error: named parameter #41 " #PARNAME41 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME42 == 42, "Error: named parameter #42 " #PARNAME42 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME43 == 43, "Error: named parameter #43 " #PARNAME43 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME44 == 44, "Error: named parameter #44 " #PARNAME44 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME45 == 45, "Error: named parameter #45 " #PARNAME45 " is a wrong name for the parameter.")

  #define np46(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38, PARNAME39, PARVAL39, PARNAME40, PARVAL40, PARNAME41, PARVAL41, PARNAME42, PARVAL42, PARNAME43, PARVAL43, PARNAME44, PARVAL44, PARNAME45, PARVAL45, PARNAME46, PARVAL46) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38, PARVAL39, PARVAL40, PARVAL41, PARVAL42, PARVAL43, PARVAL44, PARVAL45, PARVAL46); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME26 == 26, "Error: named parameter #26 " #PARNAME26 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME27 == 27, "Error: named parameter #27 " #PARNAME27 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME28 == 28, "Error: named parameter #28 " #PARNAME28 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME29 == 29, "Error: named parameter #29 " #PARNAME29 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME30 == 30, "Error: named parameter #30 " #PARNAME30 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME31 == 31, "Error: named parameter #31 " #PARNAME31 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME32 == 32, "Error: named parameter #32 " #PARNAME32 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME33 == 33, "Error: named parameter #33 " #PARNAME33 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME34 == 34, "Error: named parameter #34 " #PARNAME34 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME35 == 35, "Error: named parameter #35 " #PARNAME35 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME36 == 36, "Error: named parameter #36 " #PARNAME36 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME37 == 37, "Error: named parameter #37 " #PARNAME37 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME38 == 38, "Error: named parameter #38 " #PARNAME38 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME39 == 39, "Error: named parameter #39 " #PARNAME39 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME40 == 40, "Error: named parameter #40 " #PARNAME40 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME41 == 41, "Error: named parameter #41 " #PARNAME41 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME42 == 42, "Error: named parameter #42 " #PARNAME42 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME43 == 43, "Error: named parameter #43 " #PARNAME43 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME44 == 44, "Error: named parameter #44 " #PARNAME44 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME45 == 45, "Error: named parameter #45 " #PARNAME45 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME46 == 46, "Error: named parameter #46 " #PARNAME46 " is a wrong name for the parameter.")

  #define np47(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38, PARNAME39, PARVAL39, PARNAME40, PARVAL40, PARNAME41, PARVAL41, PARNAME42, PARVAL42, PARNAME43, PARVAL43, PARNAME44, PARVAL44, PARNAME45, PARVAL45, PARNAME46, PARVAL46, PARNAME47, PARVAL47) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38, PARVAL39, PARVAL40, PARVAL41, PARVAL42, PARVAL43, PARVAL44, PARVAL45, PARVAL46, PARVAL47); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME26 == 26, "Error: named parameter #26 " #PARNAME26 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME27 == 27, "Error: named parameter #27 " #PARNAME27 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME28 == 28, "Error: named parameter #28 " #PARNAME28 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME29 == 29, "Error: named parameter #29 " #PARNAME29 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME30 == 30, "Error: named parameter #30 " #PARNAME30 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME31 == 31, "Error: named parameter #31 " #PARNAME31 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME32 == 32, "Error: named parameter #32 " #PARNAME32 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME33 == 33, "Error: named parameter #33 " #PARNAME33 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME34 == 34, "Error: named parameter #34 " #PARNAME34 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME35 == 35, "Error: named parameter #35 " #PARNAME35 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME36 == 36, "Error: named parameter #36 " #PARNAME36 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME37 == 37, "Error: named parameter #37 " #PARNAME37 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME38 == 38, "Error: named parameter #38 " #PARNAME38 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME39 == 39, "Error: named parameter #39 " #PARNAME39 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME40 == 40, "Error: named parameter #40 " #PARNAME40 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME41 == 41, "Error: named parameter #41 " #PARNAME41 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME42 == 42, "Error: named parameter #42 " #PARNAME42 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME43 == 43, "Error: named parameter #43 " #PARNAME43 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME44 == 44, "Error: named parameter #44 " #PARNAME44 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME45 == 45, "Error: named parameter #45 " #PARNAME45 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME46 == 46, "Error: named parameter #46 " #PARNAME46 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME47 == 47, "Error: named parameter #47 " #PARNAME47 " is a wrong name for the parameter.")

  #define np48(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38, PARNAME39, PARVAL39, PARNAME40, PARVAL40, PARNAME41, PARVAL41, PARNAME42, PARVAL42, PARNAME43, PARVAL43, PARNAME44, PARVAL44, PARNAME45, PARVAL45, PARNAME46, PARVAL46, PARNAME47, PARVAL47, PARNAME48, PARVAL48) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38, PARVAL39, PARVAL40, PARVAL41, PARVAL42, PARVAL43, PARVAL44, PARVAL45, PARVAL46, PARVAL47, PARVAL48); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME26 == 26, "Error: named parameter #26 " #PARNAME26 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME27 == 27, "Error: named parameter #27 " #PARNAME27 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME28 == 28, "Error: named parameter #28 " #PARNAME28 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME29 == 29, "Error: named parameter #29 " #PARNAME29 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME30 == 30, "Error: named parameter #30 " #PARNAME30 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME31 == 31, "Error: named parameter #31 " #PARNAME31 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME32 == 32, "Error: named parameter #32 " #PARNAME32 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME33 == 33, "Error: named parameter #33 " #PARNAME33 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME34 == 34, "Error: named parameter #34 " #PARNAME34 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME35 == 35, "Error: named parameter #35 " #PARNAME35 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME36 == 36, "Error: named parameter #36 " #PARNAME36 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME37 == 37, "Error: named parameter #37 " #PARNAME37 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME38 == 38, "Error: named parameter #38 " #PARNAME38 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME39 == 39, "Error: named parameter #39 " #PARNAME39 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME40 == 40, "Error: named parameter #40 " #PARNAME40 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME41 == 41, "Error: named parameter #41 " #PARNAME41 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME42 == 42, "Error: named parameter #42 " #PARNAME42 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME43 == 43, "Error: named parameter #43 " #PARNAME43 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME44 == 44, "Error: named parameter #44 " #PARNAME44 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME45 == 45, "Error: named parameter #45 " #PARNAME45 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME46 == 46, "Error: named parameter #46 " #PARNAME46 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME47 == 47, "Error: named parameter #47 " #PARNAME47 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME48 == 48, "Error: named parameter #48 " #PARNAME48 " is a wrong name for the parameter.")

  #define np49(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38, PARNAME39, PARVAL39, PARNAME40, PARVAL40, PARNAME41, PARVAL41, PARNAME42, PARVAL42, PARNAME43, PARVAL43, PARNAME44, PARVAL44, PARNAME45, PARVAL45, PARNAME46, PARVAL46, PARNAME47, PARVAL47, PARNAME48, PARVAL48, PARNAME49, PARVAL49) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38, PARVAL39, PARVAL40, PARVAL41, PARVAL42, PARVAL43, PARVAL44, PARVAL45, PARVAL46, PARVAL47, PARVAL48, PARVAL49); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME26 == 26, "Error: named parameter #26 " #PARNAME26 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME27 == 27, "Error: named parameter #27 " #PARNAME27 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME28 == 28, "Error: named parameter #28 " #PARNAME28 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME29 == 29, "Error: named parameter #29 " #PARNAME29 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME30 == 30, "Error: named parameter #30 " #PARNAME30 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME31 == 31, "Error: named parameter #31 " #PARNAME31 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME32 == 32, "Error: named parameter #32 " #PARNAME32 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME33 == 33, "Error: named parameter #33 " #PARNAME33 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME34 == 34, "Error: named parameter #34 " #PARNAME34 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME35 == 35, "Error: named parameter #35 " #PARNAME35 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME36 == 36, "Error: named parameter #36 " #PARNAME36 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME37 == 37, "Error: named parameter #37 " #PARNAME37 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME38 == 38, "Error: named parameter #38 " #PARNAME38 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME39 == 39, "Error: named parameter #39 " #PARNAME39 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME40 == 40, "Error: named parameter #40 " #PARNAME40 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME41 == 41, "Error: named parameter #41 " #PARNAME41 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME42 == 42, "Error: named parameter #42 " #PARNAME42 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME43 == 43, "Error: named parameter #43 " #PARNAME43 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME44 == 44, "Error: named parameter #44 " #PARNAME44 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME45 == 45, "Error: named parameter #45 " #PARNAME45 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME46 == 46, "Error: named parameter #46 " #PARNAME46 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME47 == 47, "Error: named parameter #47 " #PARNAME47 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME48 == 48, "Error: named parameter #48 " #PARNAME48 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME49 == 49, "Error: named parameter #49 " #PARNAME49 " is a wrong name for the parameter.")

  #define np50(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38, PARNAME39, PARVAL39, PARNAME40, PARVAL40, PARNAME41, PARVAL41, PARNAME42, PARVAL42, PARNAME43, PARVAL43, PARNAME44, PARVAL44, PARNAME45, PARVAL45, PARNAME46, PARVAL46, PARNAME47, PARVAL47, PARNAME48, PARVAL48, PARNAME49, PARVAL49, PARNAME50, PARVAL50) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38, PARVAL39, PARVAL40, PARVAL41, PARVAL42, PARVAL43, PARVAL44, PARVAL45, PARVAL46, PARVAL47, PARVAL48, PARVAL49, PARVAL50); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME26 == 26, "Error: named parameter #26 " #PARNAME26 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME27 == 27, "Error: named parameter #27 " #PARNAME27 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME28 == 28, "Error: named parameter #28 " #PARNAME28 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME29 == 29, "Error: named parameter #29 " #PARNAME29 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME30 == 30, "Error: named parameter #30 " #PARNAME30 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME31 == 31, "Error: named parameter #31 " #PARNAME31 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME32 == 32, "Error: named parameter #32 " #PARNAME32 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME33 == 33, "Error: named parameter #33 " #PARNAME33 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME34 == 34, "Error: named parameter #34 " #PARNAME34 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME35 == 35, "Error: named parameter #35 " #PARNAME35 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME36 == 36, "Error: named parameter #36 " #PARNAME36 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME37 == 37, "Error: named parameter #37 " #PARNAME37 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME38 == 38, "Error: named parameter #38 " #PARNAME38 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME39 == 39, "Error: named parameter #39 " #PARNAME39 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME40 == 40, "Error: named parameter #40 " #PARNAME40 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME41 == 41, "Error: named parameter #41 " #PARNAME41 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME42 == 42, "Error: named parameter #42 " #PARNAME42 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME43 == 43, "Error: named parameter #43 " #PARNAME43 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME44 == 44, "Error: named parameter #44 " #PARNAME44 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME45 == 45, "Error: named parameter #45 " #PARNAME45 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME46 == 46, "Error: named parameter #46 " #PARNAME46 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME47 == 47, "Error: named parameter #47 " #PARNAME47 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME48 == 48, "Error: named parameter #48 " #PARNAME48 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME49 == 49, "Error: named parameter #49 " #PARNAME49 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME50 == 50, "Error: named parameter #50 " #PARNAME50 " is a wrong name for the parameter.")

#else // #if !defined(DISABLE_NAMED_PARAMETERS)

  #define np1(FNNAME, PARNAME1, PARVAL1) FNNAME(PARVAL1)

  #define np2(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2) FNNAME(PARVAL1, PARVAL2)

  #define np3(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3) FNNAME(PARVAL1, PARVAL2, PARVAL3)

  #define np4(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4)

  #define np5(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5)

  #define np6(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6)

  #define np7(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7)

  #define np8(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8)

  #define np9(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9)

  #define np10(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10)

  #define np11(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11)

  #define np12(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12)

  #define np13(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13)

  #define np14(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14)

  #define np15(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15)

  #define np16(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16)

  #define np17(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17)

  #define np18(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18)

  #define np19(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19)

  #define np20(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20)

  #define np21(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21)

  #define np22(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22)

  #define np23(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23)

  #define np24(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24)

  #define np25(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25)

  #define np26(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26)

  #define np27(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27)

  #define np28(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28)

  #define np29(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29)

  #define np30(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30)

  #define np31(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31)

  #define np32(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32)

  #define np33(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33)

  #define np34(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34)

  #define np35(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35)

  #define np36(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36)

  #define np37(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37)

  #define np38(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38)

  #define np39(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38, PARNAME39, PARVAL39) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38, PARVAL39)

  #define np40(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38, PARNAME39, PARVAL39, PARNAME40, PARVAL40) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38, PARVAL39, PARVAL40)

  #define np41(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38, PARNAME39, PARVAL39, PARNAME40, PARVAL40, PARNAME41, PARVAL41) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38, PARVAL39, PARVAL40, PARVAL41)

  #define np42(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38, PARNAME39, PARVAL39, PARNAME40, PARVAL40, PARNAME41, PARVAL41, PARNAME42, PARVAL42) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38, PARVAL39, PARVAL40, PARVAL41, PARVAL42)

  #define np43(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38, PARNAME39, PARVAL39, PARNAME40, PARVAL40, PARNAME41, PARVAL41, PARNAME42, PARVAL42, PARNAME43, PARVAL43) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38, PARVAL39, PARVAL40, PARVAL41, PARVAL42, PARVAL43)

  #define np44(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38, PARNAME39, PARVAL39, PARNAME40, PARVAL40, PARNAME41, PARVAL41, PARNAME42, PARVAL42, PARNAME43, PARVAL43, PARNAME44, PARVAL44) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38, PARVAL39, PARVAL40, PARVAL41, PARVAL42, PARVAL43, PARVAL44)

  #define np45(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38, PARNAME39, PARVAL39, PARNAME40, PARVAL40, PARNAME41, PARVAL41, PARNAME42, PARVAL42, PARNAME43, PARVAL43, PARNAME44, PARVAL44, PARNAME45, PARVAL45) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38, PARVAL39, PARVAL40, PARVAL41, PARVAL42, PARVAL43, PARVAL44, PARVAL45)

  #define np46(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38, PARNAME39, PARVAL39, PARNAME40, PARVAL40, PARNAME41, PARVAL41, PARNAME42, PARVAL42, PARNAME43, PARVAL43, PARNAME44, PARVAL44, PARNAME45, PARVAL45, PARNAME46, PARVAL46) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38, PARVAL39, PARVAL40, PARVAL41, PARVAL42, PARVAL43, PARVAL44, PARVAL45, PARVAL46)

  #define np47(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38, PARNAME39, PARVAL39, PARNAME40, PARVAL40, PARNAME41, PARVAL41, PARNAME42, PARVAL42, PARNAME43, PARVAL43, PARNAME44, PARVAL44, PARNAME45, PARVAL45, PARNAME46, PARVAL46, PARNAME47, PARVAL47) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38, PARVAL39, PARVAL40, PARVAL41, PARVAL42, PARVAL43, PARVAL44, PARVAL45, PARVAL46, PARVAL47)

  #define np48(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38, PARNAME39, PARVAL39, PARNAME40, PARVAL40, PARNAME41, PARVAL41, PARNAME42, PARVAL42, PARNAME43, PARVAL43, PARNAME44, PARVAL44, PARNAME45, PARVAL45, PARNAME46, PARVAL46, PARNAME47, PARVAL47, PARNAME48, PARVAL48) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38, PARVAL39, PARVAL40, PARVAL41, PARVAL42, PARVAL43, PARVAL44, PARVAL45, PARVAL46, PARVAL47, PARVAL48)

  #define np49(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38, PARNAME39, PARVAL39, PARNAME40, PARVAL40, PARNAME41, PARVAL41, PARNAME42, PARVAL42, PARNAME43, PARVAL43, PARNAME44, PARVAL44, PARNAME45, PARVAL45, PARNAME46, PARVAL46, PARNAME47, PARVAL47, PARNAME48, PARVAL48, PARNAME49, PARVAL49) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38, PARVAL39, PARVAL40, PARVAL41, PARVAL42, PARVAL43, PARVAL44, PARVAL45, PARVAL46, PARVAL47, PARVAL48, PARVAL49)

  #define np50(FNNAME, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38, PARNAME39, PARVAL39, PARNAME40, PARVAL40, PARNAME41, PARVAL41, PARNAME42, PARVAL42, PARNAME43, PARVAL43, PARNAME44, PARVAL44, PARNAME45, PARVAL45, PARNAME46, PARVAL46, PARNAME47, PARVAL47, PARNAME48, PARVAL48, PARNAME49, PARVAL49, PARNAME50, PARVAL50) FNNAME(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38, PARVAL39, PARVAL40, PARVAL41, PARVAL42, PARVAL43, PARVAL44, PARVAL45, PARVAL46, PARVAL47, PARVAL48, PARVAL49, PARVAL50)

#endif // #if !defined(DISABLE_NAMED_PARAMETERS)

// npfp


#if !defined(DISABLE_NAMED_PARAMETERS)

  #define npfp1(FNNAME, FNPTR, PARNAME1, PARVAL1) FNPTR(PARVAL1); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter.")

  #define npfp2(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2) FNPTR(PARVAL1, PARVAL2); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter.")

  #define npfp3(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3) FNPTR(PARVAL1, PARVAL2, PARVAL3); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter.")

  #define npfp4(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter.")

  #define npfp5(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter.")

  #define npfp6(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter.")

  #define npfp7(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter.")

  #define npfp8(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter.")

  #define npfp9(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter.")

  #define npfp10(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter.")

  #define npfp11(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter.")

  #define npfp12(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter.")

  #define npfp13(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter.")

  #define npfp14(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter.")

  #define npfp15(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter.")

  #define npfp16(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter.")

  #define npfp17(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter.")

  #define npfp18(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter.")

  #define npfp19(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter.")

  #define npfp20(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter.")

  #define npfp21(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter.")

  #define npfp22(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter.")

  #define npfp23(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter.")

  #define npfp24(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter.")

  #define npfp25(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter.")

  #define npfp26(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME26 == 26, "Error: named parameter #26 " #PARNAME26 " is a wrong name for the parameter.")

  #define npfp27(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME26 == 26, "Error: named parameter #26 " #PARNAME26 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME27 == 27, "Error: named parameter #27 " #PARNAME27 " is a wrong name for the parameter.")

  #define npfp28(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME26 == 26, "Error: named parameter #26 " #PARNAME26 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME27 == 27, "Error: named parameter #27 " #PARNAME27 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME28 == 28, "Error: named parameter #28 " #PARNAME28 " is a wrong name for the parameter.")

  #define npfp29(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME26 == 26, "Error: named parameter #26 " #PARNAME26 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME27 == 27, "Error: named parameter #27 " #PARNAME27 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME28 == 28, "Error: named parameter #28 " #PARNAME28 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME29 == 29, "Error: named parameter #29 " #PARNAME29 " is a wrong name for the parameter.")

  #define npfp30(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME26 == 26, "Error: named parameter #26 " #PARNAME26 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME27 == 27, "Error: named parameter #27 " #PARNAME27 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME28 == 28, "Error: named parameter #28 " #PARNAME28 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME29 == 29, "Error: named parameter #29 " #PARNAME29 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME30 == 30, "Error: named parameter #30 " #PARNAME30 " is a wrong name for the parameter.")

  #define npfp31(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME26 == 26, "Error: named parameter #26 " #PARNAME26 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME27 == 27, "Error: named parameter #27 " #PARNAME27 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME28 == 28, "Error: named parameter #28 " #PARNAME28 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME29 == 29, "Error: named parameter #29 " #PARNAME29 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME30 == 30, "Error: named parameter #30 " #PARNAME30 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME31 == 31, "Error: named parameter #31 " #PARNAME31 " is a wrong name for the parameter.")

  #define npfp32(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME26 == 26, "Error: named parameter #26 " #PARNAME26 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME27 == 27, "Error: named parameter #27 " #PARNAME27 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME28 == 28, "Error: named parameter #28 " #PARNAME28 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME29 == 29, "Error: named parameter #29 " #PARNAME29 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME30 == 30, "Error: named parameter #30 " #PARNAME30 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME31 == 31, "Error: named parameter #31 " #PARNAME31 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME32 == 32, "Error: named parameter #32 " #PARNAME32 " is a wrong name for the parameter.")

  #define npfp33(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME26 == 26, "Error: named parameter #26 " #PARNAME26 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME27 == 27, "Error: named parameter #27 " #PARNAME27 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME28 == 28, "Error: named parameter #28 " #PARNAME28 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME29 == 29, "Error: named parameter #29 " #PARNAME29 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME30 == 30, "Error: named parameter #30 " #PARNAME30 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME31 == 31, "Error: named parameter #31 " #PARNAME31 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME32 == 32, "Error: named parameter #32 " #PARNAME32 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME33 == 33, "Error: named parameter #33 " #PARNAME33 " is a wrong name for the parameter.")

  #define npfp34(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME26 == 26, "Error: named parameter #26 " #PARNAME26 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME27 == 27, "Error: named parameter #27 " #PARNAME27 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME28 == 28, "Error: named parameter #28 " #PARNAME28 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME29 == 29, "Error: named parameter #29 " #PARNAME29 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME30 == 30, "Error: named parameter #30 " #PARNAME30 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME31 == 31, "Error: named parameter #31 " #PARNAME31 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME32 == 32, "Error: named parameter #32 " #PARNAME32 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME33 == 33, "Error: named parameter #33 " #PARNAME33 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME34 == 34, "Error: named parameter #34 " #PARNAME34 " is a wrong name for the parameter.")

  #define npfp35(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME26 == 26, "Error: named parameter #26 " #PARNAME26 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME27 == 27, "Error: named parameter #27 " #PARNAME27 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME28 == 28, "Error: named parameter #28 " #PARNAME28 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME29 == 29, "Error: named parameter #29 " #PARNAME29 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME30 == 30, "Error: named parameter #30 " #PARNAME30 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME31 == 31, "Error: named parameter #31 " #PARNAME31 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME32 == 32, "Error: named parameter #32 " #PARNAME32 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME33 == 33, "Error: named parameter #33 " #PARNAME33 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME34 == 34, "Error: named parameter #34 " #PARNAME34 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME35 == 35, "Error: named parameter #35 " #PARNAME35 " is a wrong name for the parameter.")

  #define npfp36(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME26 == 26, "Error: named parameter #26 " #PARNAME26 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME27 == 27, "Error: named parameter #27 " #PARNAME27 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME28 == 28, "Error: named parameter #28 " #PARNAME28 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME29 == 29, "Error: named parameter #29 " #PARNAME29 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME30 == 30, "Error: named parameter #30 " #PARNAME30 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME31 == 31, "Error: named parameter #31 " #PARNAME31 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME32 == 32, "Error: named parameter #32 " #PARNAME32 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME33 == 33, "Error: named parameter #33 " #PARNAME33 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME34 == 34, "Error: named parameter #34 " #PARNAME34 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME35 == 35, "Error: named parameter #35 " #PARNAME35 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME36 == 36, "Error: named parameter #36 " #PARNAME36 " is a wrong name for the parameter.")

  #define npfp37(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME26 == 26, "Error: named parameter #26 " #PARNAME26 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME27 == 27, "Error: named parameter #27 " #PARNAME27 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME28 == 28, "Error: named parameter #28 " #PARNAME28 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME29 == 29, "Error: named parameter #29 " #PARNAME29 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME30 == 30, "Error: named parameter #30 " #PARNAME30 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME31 == 31, "Error: named parameter #31 " #PARNAME31 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME32 == 32, "Error: named parameter #32 " #PARNAME32 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME33 == 33, "Error: named parameter #33 " #PARNAME33 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME34 == 34, "Error: named parameter #34 " #PARNAME34 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME35 == 35, "Error: named parameter #35 " #PARNAME35 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME36 == 36, "Error: named parameter #36 " #PARNAME36 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME37 == 37, "Error: named parameter #37 " #PARNAME37 " is a wrong name for the parameter.")

  #define npfp38(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME26 == 26, "Error: named parameter #26 " #PARNAME26 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME27 == 27, "Error: named parameter #27 " #PARNAME27 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME28 == 28, "Error: named parameter #28 " #PARNAME28 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME29 == 29, "Error: named parameter #29 " #PARNAME29 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME30 == 30, "Error: named parameter #30 " #PARNAME30 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME31 == 31, "Error: named parameter #31 " #PARNAME31 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME32 == 32, "Error: named parameter #32 " #PARNAME32 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME33 == 33, "Error: named parameter #33 " #PARNAME33 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME34 == 34, "Error: named parameter #34 " #PARNAME34 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME35 == 35, "Error: named parameter #35 " #PARNAME35 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME36 == 36, "Error: named parameter #36 " #PARNAME36 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME37 == 37, "Error: named parameter #37 " #PARNAME37 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME38 == 38, "Error: named parameter #38 " #PARNAME38 " is a wrong name for the parameter.")

  #define npfp39(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38, PARNAME39, PARVAL39) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38, PARVAL39); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME26 == 26, "Error: named parameter #26 " #PARNAME26 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME27 == 27, "Error: named parameter #27 " #PARNAME27 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME28 == 28, "Error: named parameter #28 " #PARNAME28 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME29 == 29, "Error: named parameter #29 " #PARNAME29 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME30 == 30, "Error: named parameter #30 " #PARNAME30 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME31 == 31, "Error: named parameter #31 " #PARNAME31 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME32 == 32, "Error: named parameter #32 " #PARNAME32 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME33 == 33, "Error: named parameter #33 " #PARNAME33 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME34 == 34, "Error: named parameter #34 " #PARNAME34 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME35 == 35, "Error: named parameter #35 " #PARNAME35 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME36 == 36, "Error: named parameter #36 " #PARNAME36 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME37 == 37, "Error: named parameter #37 " #PARNAME37 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME38 == 38, "Error: named parameter #38 " #PARNAME38 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME39 == 39, "Error: named parameter #39 " #PARNAME39 " is a wrong name for the parameter.")

  #define npfp40(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38, PARNAME39, PARVAL39, PARNAME40, PARVAL40) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38, PARVAL39, PARVAL40); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME26 == 26, "Error: named parameter #26 " #PARNAME26 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME27 == 27, "Error: named parameter #27 " #PARNAME27 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME28 == 28, "Error: named parameter #28 " #PARNAME28 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME29 == 29, "Error: named parameter #29 " #PARNAME29 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME30 == 30, "Error: named parameter #30 " #PARNAME30 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME31 == 31, "Error: named parameter #31 " #PARNAME31 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME32 == 32, "Error: named parameter #32 " #PARNAME32 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME33 == 33, "Error: named parameter #33 " #PARNAME33 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME34 == 34, "Error: named parameter #34 " #PARNAME34 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME35 == 35, "Error: named parameter #35 " #PARNAME35 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME36 == 36, "Error: named parameter #36 " #PARNAME36 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME37 == 37, "Error: named parameter #37 " #PARNAME37 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME38 == 38, "Error: named parameter #38 " #PARNAME38 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME39 == 39, "Error: named parameter #39 " #PARNAME39 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME40 == 40, "Error: named parameter #40 " #PARNAME40 " is a wrong name for the parameter.")

  #define npfp41(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38, PARNAME39, PARVAL39, PARNAME40, PARVAL40, PARNAME41, PARVAL41) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38, PARVAL39, PARVAL40, PARVAL41); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME26 == 26, "Error: named parameter #26 " #PARNAME26 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME27 == 27, "Error: named parameter #27 " #PARNAME27 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME28 == 28, "Error: named parameter #28 " #PARNAME28 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME29 == 29, "Error: named parameter #29 " #PARNAME29 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME30 == 30, "Error: named parameter #30 " #PARNAME30 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME31 == 31, "Error: named parameter #31 " #PARNAME31 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME32 == 32, "Error: named parameter #32 " #PARNAME32 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME33 == 33, "Error: named parameter #33 " #PARNAME33 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME34 == 34, "Error: named parameter #34 " #PARNAME34 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME35 == 35, "Error: named parameter #35 " #PARNAME35 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME36 == 36, "Error: named parameter #36 " #PARNAME36 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME37 == 37, "Error: named parameter #37 " #PARNAME37 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME38 == 38, "Error: named parameter #38 " #PARNAME38 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME39 == 39, "Error: named parameter #39 " #PARNAME39 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME40 == 40, "Error: named parameter #40 " #PARNAME40 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME41 == 41, "Error: named parameter #41 " #PARNAME41 " is a wrong name for the parameter.")

  #define npfp42(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38, PARNAME39, PARVAL39, PARNAME40, PARVAL40, PARNAME41, PARVAL41, PARNAME42, PARVAL42) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38, PARVAL39, PARVAL40, PARVAL41, PARVAL42); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME26 == 26, "Error: named parameter #26 " #PARNAME26 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME27 == 27, "Error: named parameter #27 " #PARNAME27 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME28 == 28, "Error: named parameter #28 " #PARNAME28 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME29 == 29, "Error: named parameter #29 " #PARNAME29 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME30 == 30, "Error: named parameter #30 " #PARNAME30 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME31 == 31, "Error: named parameter #31 " #PARNAME31 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME32 == 32, "Error: named parameter #32 " #PARNAME32 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME33 == 33, "Error: named parameter #33 " #PARNAME33 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME34 == 34, "Error: named parameter #34 " #PARNAME34 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME35 == 35, "Error: named parameter #35 " #PARNAME35 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME36 == 36, "Error: named parameter #36 " #PARNAME36 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME37 == 37, "Error: named parameter #37 " #PARNAME37 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME38 == 38, "Error: named parameter #38 " #PARNAME38 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME39 == 39, "Error: named parameter #39 " #PARNAME39 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME40 == 40, "Error: named parameter #40 " #PARNAME40 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME41 == 41, "Error: named parameter #41 " #PARNAME41 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME42 == 42, "Error: named parameter #42 " #PARNAME42 " is a wrong name for the parameter.")

  #define npfp43(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38, PARNAME39, PARVAL39, PARNAME40, PARVAL40, PARNAME41, PARVAL41, PARNAME42, PARVAL42, PARNAME43, PARVAL43) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38, PARVAL39, PARVAL40, PARVAL41, PARVAL42, PARVAL43); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME26 == 26, "Error: named parameter #26 " #PARNAME26 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME27 == 27, "Error: named parameter #27 " #PARNAME27 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME28 == 28, "Error: named parameter #28 " #PARNAME28 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME29 == 29, "Error: named parameter #29 " #PARNAME29 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME30 == 30, "Error: named parameter #30 " #PARNAME30 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME31 == 31, "Error: named parameter #31 " #PARNAME31 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME32 == 32, "Error: named parameter #32 " #PARNAME32 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME33 == 33, "Error: named parameter #33 " #PARNAME33 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME34 == 34, "Error: named parameter #34 " #PARNAME34 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME35 == 35, "Error: named parameter #35 " #PARNAME35 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME36 == 36, "Error: named parameter #36 " #PARNAME36 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME37 == 37, "Error: named parameter #37 " #PARNAME37 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME38 == 38, "Error: named parameter #38 " #PARNAME38 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME39 == 39, "Error: named parameter #39 " #PARNAME39 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME40 == 40, "Error: named parameter #40 " #PARNAME40 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME41 == 41, "Error: named parameter #41 " #PARNAME41 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME42 == 42, "Error: named parameter #42 " #PARNAME42 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME43 == 43, "Error: named parameter #43 " #PARNAME43 " is a wrong name for the parameter.")

  #define npfp44(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38, PARNAME39, PARVAL39, PARNAME40, PARVAL40, PARNAME41, PARVAL41, PARNAME42, PARVAL42, PARNAME43, PARVAL43, PARNAME44, PARVAL44) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38, PARVAL39, PARVAL40, PARVAL41, PARVAL42, PARVAL43, PARVAL44); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME26 == 26, "Error: named parameter #26 " #PARNAME26 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME27 == 27, "Error: named parameter #27 " #PARNAME27 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME28 == 28, "Error: named parameter #28 " #PARNAME28 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME29 == 29, "Error: named parameter #29 " #PARNAME29 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME30 == 30, "Error: named parameter #30 " #PARNAME30 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME31 == 31, "Error: named parameter #31 " #PARNAME31 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME32 == 32, "Error: named parameter #32 " #PARNAME32 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME33 == 33, "Error: named parameter #33 " #PARNAME33 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME34 == 34, "Error: named parameter #34 " #PARNAME34 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME35 == 35, "Error: named parameter #35 " #PARNAME35 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME36 == 36, "Error: named parameter #36 " #PARNAME36 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME37 == 37, "Error: named parameter #37 " #PARNAME37 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME38 == 38, "Error: named parameter #38 " #PARNAME38 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME39 == 39, "Error: named parameter #39 " #PARNAME39 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME40 == 40, "Error: named parameter #40 " #PARNAME40 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME41 == 41, "Error: named parameter #41 " #PARNAME41 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME42 == 42, "Error: named parameter #42 " #PARNAME42 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME43 == 43, "Error: named parameter #43 " #PARNAME43 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME44 == 44, "Error: named parameter #44 " #PARNAME44 " is a wrong name for the parameter.")

  #define npfp45(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38, PARNAME39, PARVAL39, PARNAME40, PARVAL40, PARNAME41, PARVAL41, PARNAME42, PARVAL42, PARNAME43, PARVAL43, PARNAME44, PARVAL44, PARNAME45, PARVAL45) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38, PARVAL39, PARVAL40, PARVAL41, PARVAL42, PARVAL43, PARVAL44, PARVAL45); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME26 == 26, "Error: named parameter #26 " #PARNAME26 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME27 == 27, "Error: named parameter #27 " #PARNAME27 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME28 == 28, "Error: named parameter #28 " #PARNAME28 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME29 == 29, "Error: named parameter #29 " #PARNAME29 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME30 == 30, "Error: named parameter #30 " #PARNAME30 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME31 == 31, "Error: named parameter #31 " #PARNAME31 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME32 == 32, "Error: named parameter #32 " #PARNAME32 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME33 == 33, "Error: named parameter #33 " #PARNAME33 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME34 == 34, "Error: named parameter #34 " #PARNAME34 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME35 == 35, "Error: named parameter #35 " #PARNAME35 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME36 == 36, "Error: named parameter #36 " #PARNAME36 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME37 == 37, "Error: named parameter #37 " #PARNAME37 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME38 == 38, "Error: named parameter #38 " #PARNAME38 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME39 == 39, "Error: named parameter #39 " #PARNAME39 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME40 == 40, "Error: named parameter #40 " #PARNAME40 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME41 == 41, "Error: named parameter #41 " #PARNAME41 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME42 == 42, "Error: named parameter #42 " #PARNAME42 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME43 == 43, "Error: named parameter #43 " #PARNAME43 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME44 == 44, "Error: named parameter #44 " #PARNAME44 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME45 == 45, "Error: named parameter #45 " #PARNAME45 " is a wrong name for the parameter.")

  #define npfp46(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38, PARNAME39, PARVAL39, PARNAME40, PARVAL40, PARNAME41, PARVAL41, PARNAME42, PARVAL42, PARNAME43, PARVAL43, PARNAME44, PARVAL44, PARNAME45, PARVAL45, PARNAME46, PARVAL46) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38, PARVAL39, PARVAL40, PARVAL41, PARVAL42, PARVAL43, PARVAL44, PARVAL45, PARVAL46); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME26 == 26, "Error: named parameter #26 " #PARNAME26 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME27 == 27, "Error: named parameter #27 " #PARNAME27 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME28 == 28, "Error: named parameter #28 " #PARNAME28 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME29 == 29, "Error: named parameter #29 " #PARNAME29 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME30 == 30, "Error: named parameter #30 " #PARNAME30 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME31 == 31, "Error: named parameter #31 " #PARNAME31 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME32 == 32, "Error: named parameter #32 " #PARNAME32 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME33 == 33, "Error: named parameter #33 " #PARNAME33 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME34 == 34, "Error: named parameter #34 " #PARNAME34 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME35 == 35, "Error: named parameter #35 " #PARNAME35 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME36 == 36, "Error: named parameter #36 " #PARNAME36 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME37 == 37, "Error: named parameter #37 " #PARNAME37 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME38 == 38, "Error: named parameter #38 " #PARNAME38 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME39 == 39, "Error: named parameter #39 " #PARNAME39 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME40 == 40, "Error: named parameter #40 " #PARNAME40 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME41 == 41, "Error: named parameter #41 " #PARNAME41 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME42 == 42, "Error: named parameter #42 " #PARNAME42 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME43 == 43, "Error: named parameter #43 " #PARNAME43 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME44 == 44, "Error: named parameter #44 " #PARNAME44 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME45 == 45, "Error: named parameter #45 " #PARNAME45 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME46 == 46, "Error: named parameter #46 " #PARNAME46 " is a wrong name for the parameter.")

  #define npfp47(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38, PARNAME39, PARVAL39, PARNAME40, PARVAL40, PARNAME41, PARVAL41, PARNAME42, PARVAL42, PARNAME43, PARVAL43, PARNAME44, PARVAL44, PARNAME45, PARVAL45, PARNAME46, PARVAL46, PARNAME47, PARVAL47) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38, PARVAL39, PARVAL40, PARVAL41, PARVAL42, PARVAL43, PARVAL44, PARVAL45, PARVAL46, PARVAL47); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME26 == 26, "Error: named parameter #26 " #PARNAME26 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME27 == 27, "Error: named parameter #27 " #PARNAME27 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME28 == 28, "Error: named parameter #28 " #PARNAME28 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME29 == 29, "Error: named parameter #29 " #PARNAME29 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME30 == 30, "Error: named parameter #30 " #PARNAME30 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME31 == 31, "Error: named parameter #31 " #PARNAME31 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME32 == 32, "Error: named parameter #32 " #PARNAME32 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME33 == 33, "Error: named parameter #33 " #PARNAME33 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME34 == 34, "Error: named parameter #34 " #PARNAME34 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME35 == 35, "Error: named parameter #35 " #PARNAME35 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME36 == 36, "Error: named parameter #36 " #PARNAME36 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME37 == 37, "Error: named parameter #37 " #PARNAME37 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME38 == 38, "Error: named parameter #38 " #PARNAME38 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME39 == 39, "Error: named parameter #39 " #PARNAME39 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME40 == 40, "Error: named parameter #40 " #PARNAME40 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME41 == 41, "Error: named parameter #41 " #PARNAME41 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME42 == 42, "Error: named parameter #42 " #PARNAME42 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME43 == 43, "Error: named parameter #43 " #PARNAME43 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME44 == 44, "Error: named parameter #44 " #PARNAME44 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME45 == 45, "Error: named parameter #45 " #PARNAME45 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME46 == 46, "Error: named parameter #46 " #PARNAME46 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME47 == 47, "Error: named parameter #47 " #PARNAME47 " is a wrong name for the parameter.")

  #define npfp48(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38, PARNAME39, PARVAL39, PARNAME40, PARVAL40, PARNAME41, PARVAL41, PARNAME42, PARVAL42, PARNAME43, PARVAL43, PARNAME44, PARVAL44, PARNAME45, PARVAL45, PARNAME46, PARVAL46, PARNAME47, PARVAL47, PARNAME48, PARVAL48) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38, PARVAL39, PARVAL40, PARVAL41, PARVAL42, PARVAL43, PARVAL44, PARVAL45, PARVAL46, PARVAL47, PARVAL48); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME26 == 26, "Error: named parameter #26 " #PARNAME26 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME27 == 27, "Error: named parameter #27 " #PARNAME27 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME28 == 28, "Error: named parameter #28 " #PARNAME28 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME29 == 29, "Error: named parameter #29 " #PARNAME29 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME30 == 30, "Error: named parameter #30 " #PARNAME30 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME31 == 31, "Error: named parameter #31 " #PARNAME31 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME32 == 32, "Error: named parameter #32 " #PARNAME32 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME33 == 33, "Error: named parameter #33 " #PARNAME33 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME34 == 34, "Error: named parameter #34 " #PARNAME34 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME35 == 35, "Error: named parameter #35 " #PARNAME35 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME36 == 36, "Error: named parameter #36 " #PARNAME36 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME37 == 37, "Error: named parameter #37 " #PARNAME37 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME38 == 38, "Error: named parameter #38 " #PARNAME38 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME39 == 39, "Error: named parameter #39 " #PARNAME39 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME40 == 40, "Error: named parameter #40 " #PARNAME40 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME41 == 41, "Error: named parameter #41 " #PARNAME41 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME42 == 42, "Error: named parameter #42 " #PARNAME42 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME43 == 43, "Error: named parameter #43 " #PARNAME43 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME44 == 44, "Error: named parameter #44 " #PARNAME44 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME45 == 45, "Error: named parameter #45 " #PARNAME45 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME46 == 46, "Error: named parameter #46 " #PARNAME46 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME47 == 47, "Error: named parameter #47 " #PARNAME47 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME48 == 48, "Error: named parameter #48 " #PARNAME48 " is a wrong name for the parameter.")

  #define npfp49(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38, PARNAME39, PARVAL39, PARNAME40, PARVAL40, PARNAME41, PARVAL41, PARNAME42, PARVAL42, PARNAME43, PARVAL43, PARNAME44, PARVAL44, PARNAME45, PARVAL45, PARNAME46, PARVAL46, PARNAME47, PARVAL47, PARNAME48, PARVAL48, PARNAME49, PARVAL49) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38, PARVAL39, PARVAL40, PARVAL41, PARVAL42, PARVAL43, PARVAL44, PARVAL45, PARVAL46, PARVAL47, PARVAL48, PARVAL49); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME26 == 26, "Error: named parameter #26 " #PARNAME26 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME27 == 27, "Error: named parameter #27 " #PARNAME27 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME28 == 28, "Error: named parameter #28 " #PARNAME28 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME29 == 29, "Error: named parameter #29 " #PARNAME29 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME30 == 30, "Error: named parameter #30 " #PARNAME30 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME31 == 31, "Error: named parameter #31 " #PARNAME31 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME32 == 32, "Error: named parameter #32 " #PARNAME32 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME33 == 33, "Error: named parameter #33 " #PARNAME33 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME34 == 34, "Error: named parameter #34 " #PARNAME34 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME35 == 35, "Error: named parameter #35 " #PARNAME35 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME36 == 36, "Error: named parameter #36 " #PARNAME36 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME37 == 37, "Error: named parameter #37 " #PARNAME37 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME38 == 38, "Error: named parameter #38 " #PARNAME38 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME39 == 39, "Error: named parameter #39 " #PARNAME39 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME40 == 40, "Error: named parameter #40 " #PARNAME40 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME41 == 41, "Error: named parameter #41 " #PARNAME41 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME42 == 42, "Error: named parameter #42 " #PARNAME42 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME43 == 43, "Error: named parameter #43 " #PARNAME43 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME44 == 44, "Error: named parameter #44 " #PARNAME44 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME45 == 45, "Error: named parameter #45 " #PARNAME45 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME46 == 46, "Error: named parameter #46 " #PARNAME46 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME47 == 47, "Error: named parameter #47 " #PARNAME47 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME48 == 48, "Error: named parameter #48 " #PARNAME48 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME49 == 49, "Error: named parameter #49 " #PARNAME49 " is a wrong name for the parameter.")

  #define npfp50(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38, PARNAME39, PARVAL39, PARNAME40, PARVAL40, PARNAME41, PARVAL41, PARNAME42, PARVAL42, PARNAME43, PARVAL43, PARNAME44, PARVAL44, PARNAME45, PARVAL45, PARNAME46, PARVAL46, PARNAME47, PARVAL47, PARNAME48, PARVAL48, PARNAME49, PARVAL49, PARNAME50, PARVAL50) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38, PARVAL39, PARVAL40, PARVAL41, PARVAL42, PARVAL43, PARVAL44, PARVAL45, PARVAL46, PARVAL47, PARVAL48, PARVAL49, PARVAL50); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME1 == 1, "Error: named parameter #1 " #PARNAME1 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME2 == 2, "Error: named parameter #2 " #PARNAME2 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME3 == 3, "Error: named parameter #3 " #PARNAME3 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME4 == 4, "Error: named parameter #4 " #PARNAME4 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME5 == 5, "Error: named parameter #5 " #PARNAME5 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME6 == 6, "Error: named parameter #6 " #PARNAME6 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME7 == 7, "Error: named parameter #7 " #PARNAME7 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME8 == 8, "Error: named parameter #8 " #PARNAME8 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME9 == 9, "Error: named parameter #9 " #PARNAME9 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME10 == 10, "Error: named parameter #10 " #PARNAME10 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME11 == 11, "Error: named parameter #11 " #PARNAME11 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME12 == 12, "Error: named parameter #12 " #PARNAME12 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME13 == 13, "Error: named parameter #13 " #PARNAME13 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME14 == 14, "Error: named parameter #14 " #PARNAME14 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME15 == 15, "Error: named parameter #15 " #PARNAME15 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME16 == 16, "Error: named parameter #16 " #PARNAME16 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME17 == 17, "Error: named parameter #17 " #PARNAME17 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME18 == 18, "Error: named parameter #18 " #PARNAME18 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME19 == 19, "Error: named parameter #19 " #PARNAME19 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME20 == 20, "Error: named parameter #20 " #PARNAME20 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME21 == 21, "Error: named parameter #21 " #PARNAME21 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME22 == 22, "Error: named parameter #22 " #PARNAME22 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME23 == 23, "Error: named parameter #23 " #PARNAME23 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME24 == 24, "Error: named parameter #24 " #PARNAME24 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME25 == 25, "Error: named parameter #25 " #PARNAME25 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME26 == 26, "Error: named parameter #26 " #PARNAME26 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME27 == 27, "Error: named parameter #27 " #PARNAME27 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME28 == 28, "Error: named parameter #28 " #PARNAME28 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME29 == 29, "Error: named parameter #29 " #PARNAME29 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME30 == 30, "Error: named parameter #30 " #PARNAME30 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME31 == 31, "Error: named parameter #31 " #PARNAME31 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME32 == 32, "Error: named parameter #32 " #PARNAME32 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME33 == 33, "Error: named parameter #33 " #PARNAME33 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME34 == 34, "Error: named parameter #34 " #PARNAME34 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME35 == 35, "Error: named parameter #35 " #PARNAME35 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME36 == 36, "Error: named parameter #36 " #PARNAME36 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME37 == 37, "Error: named parameter #37 " #PARNAME37 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME38 == 38, "Error: named parameter #38 " #PARNAME38 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME39 == 39, "Error: named parameter #39 " #PARNAME39 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME40 == 40, "Error: named parameter #40 " #PARNAME40 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME41 == 41, "Error: named parameter #41 " #PARNAME41 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME42 == 42, "Error: named parameter #42 " #PARNAME42 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME43 == 43, "Error: named parameter #43 " #PARNAME43 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME44 == 44, "Error: named parameter #44 " #PARNAME44 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME45 == 45, "Error: named parameter #45 " #PARNAME45 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME46 == 46, "Error: named parameter #46 " #PARNAME46 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME47 == 47, "Error: named parameter #47 " #PARNAME47 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME48 == 48, "Error: named parameter #48 " #PARNAME48 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME49 == 49, "Error: named parameter #49 " #PARNAME49 " is a wrong name for the parameter."); \
    static_assert(_named_parameter_##FNNAME##_##PARNAME50 == 50, "Error: named parameter #50 " #PARNAME50 " is a wrong name for the parameter.")

#else // #if !defined(DISABLE_NAMED_PARAMETERS)

  #define npfp1(FNNAME, FNPTR, PARNAME1, PARVAL1) FNPTR(PARVAL1)

  #define npfp2(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2) FNPTR(PARVAL1, PARVAL2)

  #define npfp3(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3) FNPTR(PARVAL1, PARVAL2, PARVAL3)

  #define npfp4(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4)

  #define npfp5(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5)

  #define npfp6(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6)

  #define npfp7(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7)

  #define npfp8(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8)

  #define npfp9(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9)

  #define npfp10(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10)

  #define npfp11(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11)

  #define npfp12(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12)

  #define npfp13(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13)

  #define npfp14(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14)

  #define npfp15(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15)

  #define npfp16(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16)

  #define npfp17(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17)

  #define npfp18(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18)

  #define npfp19(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19)

  #define npfp20(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20)

  #define npfp21(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21)

  #define npfp22(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22)

  #define npfp23(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23)

  #define npfp24(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24)

  #define npfp25(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25)

  #define npfp26(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26)

  #define npfp27(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27)

  #define npfp28(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28)

  #define npfp29(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29)

  #define npfp30(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30)

  #define npfp31(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31)

  #define npfp32(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32)

  #define npfp33(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33)

  #define npfp34(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34)

  #define npfp35(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35)

  #define npfp36(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36)

  #define npfp37(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37)

  #define npfp38(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38)

  #define npfp39(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38, PARNAME39, PARVAL39) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38, PARVAL39)

  #define npfp40(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38, PARNAME39, PARVAL39, PARNAME40, PARVAL40) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38, PARVAL39, PARVAL40)

  #define npfp41(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38, PARNAME39, PARVAL39, PARNAME40, PARVAL40, PARNAME41, PARVAL41) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38, PARVAL39, PARVAL40, PARVAL41)

  #define npfp42(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38, PARNAME39, PARVAL39, PARNAME40, PARVAL40, PARNAME41, PARVAL41, PARNAME42, PARVAL42) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38, PARVAL39, PARVAL40, PARVAL41, PARVAL42)

  #define npfp43(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38, PARNAME39, PARVAL39, PARNAME40, PARVAL40, PARNAME41, PARVAL41, PARNAME42, PARVAL42, PARNAME43, PARVAL43) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38, PARVAL39, PARVAL40, PARVAL41, PARVAL42, PARVAL43)

  #define npfp44(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38, PARNAME39, PARVAL39, PARNAME40, PARVAL40, PARNAME41, PARVAL41, PARNAME42, PARVAL42, PARNAME43, PARVAL43, PARNAME44, PARVAL44) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38, PARVAL39, PARVAL40, PARVAL41, PARVAL42, PARVAL43, PARVAL44)

  #define npfp45(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38, PARNAME39, PARVAL39, PARNAME40, PARVAL40, PARNAME41, PARVAL41, PARNAME42, PARVAL42, PARNAME43, PARVAL43, PARNAME44, PARVAL44, PARNAME45, PARVAL45) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38, PARVAL39, PARVAL40, PARVAL41, PARVAL42, PARVAL43, PARVAL44, PARVAL45)

  #define npfp46(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38, PARNAME39, PARVAL39, PARNAME40, PARVAL40, PARNAME41, PARVAL41, PARNAME42, PARVAL42, PARNAME43, PARVAL43, PARNAME44, PARVAL44, PARNAME45, PARVAL45, PARNAME46, PARVAL46) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38, PARVAL39, PARVAL40, PARVAL41, PARVAL42, PARVAL43, PARVAL44, PARVAL45, PARVAL46)

  #define npfp47(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38, PARNAME39, PARVAL39, PARNAME40, PARVAL40, PARNAME41, PARVAL41, PARNAME42, PARVAL42, PARNAME43, PARVAL43, PARNAME44, PARVAL44, PARNAME45, PARVAL45, PARNAME46, PARVAL46, PARNAME47, PARVAL47) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38, PARVAL39, PARVAL40, PARVAL41, PARVAL42, PARVAL43, PARVAL44, PARVAL45, PARVAL46, PARVAL47)

  #define npfp48(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38, PARNAME39, PARVAL39, PARNAME40, PARVAL40, PARNAME41, PARVAL41, PARNAME42, PARVAL42, PARNAME43, PARVAL43, PARNAME44, PARVAL44, PARNAME45, PARVAL45, PARNAME46, PARVAL46, PARNAME47, PARVAL47, PARNAME48, PARVAL48) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38, PARVAL39, PARVAL40, PARVAL41, PARVAL42, PARVAL43, PARVAL44, PARVAL45, PARVAL46, PARVAL47, PARVAL48)

  #define npfp49(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38, PARNAME39, PARVAL39, PARNAME40, PARVAL40, PARNAME41, PARVAL41, PARNAME42, PARVAL42, PARNAME43, PARVAL43, PARNAME44, PARVAL44, PARNAME45, PARVAL45, PARNAME46, PARVAL46, PARNAME47, PARVAL47, PARNAME48, PARVAL48, PARNAME49, PARVAL49) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38, PARVAL39, PARVAL40, PARVAL41, PARVAL42, PARVAL43, PARVAL44, PARVAL45, PARVAL46, PARVAL47, PARVAL48, PARVAL49)

  #define npfp50(FNNAME, FNPTR, PARNAME1, PARVAL1, PARNAME2, PARVAL2, PARNAME3, PARVAL3, PARNAME4, PARVAL4, PARNAME5, PARVAL5, PARNAME6, PARVAL6, PARNAME7, PARVAL7, PARNAME8, PARVAL8, PARNAME9, PARVAL9, PARNAME10, PARVAL10, PARNAME11, PARVAL11, PARNAME12, PARVAL12, PARNAME13, PARVAL13, PARNAME14, PARVAL14, PARNAME15, PARVAL15, PARNAME16, PARVAL16, PARNAME17, PARVAL17, PARNAME18, PARVAL18, PARNAME19, PARVAL19, PARNAME20, PARVAL20, PARNAME21, PARVAL21, PARNAME22, PARVAL22, PARNAME23, PARVAL23, PARNAME24, PARVAL24, PARNAME25, PARVAL25, PARNAME26, PARVAL26, PARNAME27, PARVAL27, PARNAME28, PARVAL28, PARNAME29, PARVAL29, PARNAME30, PARVAL30, PARNAME31, PARVAL31, PARNAME32, PARVAL32, PARNAME33, PARVAL33, PARNAME34, PARVAL34, PARNAME35, PARVAL35, PARNAME36, PARVAL36, PARNAME37, PARVAL37, PARNAME38, PARVAL38, PARNAME39, PARVAL39, PARNAME40, PARVAL40, PARNAME41, PARVAL41, PARNAME42, PARVAL42, PARNAME43, PARVAL43, PARNAME44, PARVAL44, PARNAME45, PARVAL45, PARNAME46, PARVAL46, PARNAME47, PARVAL47, PARNAME48, PARVAL48, PARNAME49, PARVAL49, PARNAME50, PARVAL50) FNPTR(PARVAL1, PARVAL2, PARVAL3, PARVAL4, PARVAL5, PARVAL6, PARVAL7, PARVAL8, PARVAL9, PARVAL10, PARVAL11, PARVAL12, PARVAL13, PARVAL14, PARVAL15, PARVAL16, PARVAL17, PARVAL18, PARVAL19, PARVAL20, PARVAL21, PARVAL22, PARVAL23, PARVAL24, PARVAL25, PARVAL26, PARVAL27, PARVAL28, PARVAL29, PARVAL30, PARVAL31, PARVAL32, PARVAL33, PARVAL34, PARVAL35, PARVAL36, PARVAL37, PARVAL38, PARVAL39, PARVAL40, PARVAL41, PARVAL42, PARVAL43, PARVAL44, PARVAL45, PARVAL46, PARVAL47, PARVAL48, PARVAL49, PARVAL50)

#endif // #if !defined(DISABLE_NAMED_PARAMETERS)
