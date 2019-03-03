#ifndef CLYRA_ENUM_H
#define CLYRA_ENUM_H

// Resolves to the size of the macro varargs
#define VA_NARGS_IMPL(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, N, ...) N
#define VA_NARGS(...) VA_NARGS_IMPL(__VA_ARGS__, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)

// The _MAKE_ENUM_N macro implementations; defines an enum with given name and given options
// Used in the MAKE_ENUM macro below, in conjunction with the _MAKE_ENUM_REPRS_N macro
#define _MAKE_ENUM_1(enumName, opt1) \
  typedef enum { opt1 } enumName;

#define _MAKE_ENUM_2(enumName, opt1, opt2) \
  typedef enum { opt1, opt2 } enumName;

#define _MAKE_ENUM_3(enumName, opt1, opt2, opt3) \
  typedef enum { opt1, opt2, opt3 } enumName;

#define _MAKE_ENUM_4(enumName, opt1, opt2, opt3, opt4) \
  typedef enum { opt1, opt2, opt3, opt4 } enumName;

#define _MAKE_ENUM_5(enumName, opt1, opt2, opt3, opt4, opt5) \
  typedef enum { opt1, opt2, opt3, opt4, opt5 } enumName;

#define _MAKE_ENUM_6(enumName, opt1, opt2, opt3, opt4, opt5, opt6) \
  typedef enum { opt1, opt2, opt3, opt4, opt5, opt6 } enumName;

#define _MAKE_ENUM_7(enumName, opt1, opt2, opt3, opt4, opt5, opt6, opt7) \
  typedef enum { opt1, opt2, opt3, opt4, opt5, opt6, opt7 } enumName;

#define _MAKE_ENUM_8(enumName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8) \
  typedef enum { opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8 } enumName;

#define _MAKE_ENUM_9(enumName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9) \
  typedef enum { opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9 } enumName;

#define _MAKE_ENUM_10(enumName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10) \
  typedef enum { opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10 } enumName;

#define _MAKE_ENUM_11(enumName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11) \
  typedef enum { opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11 } enumName;

#define _MAKE_ENUM_12(enumName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12) \
  typedef enum { opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12 } enumName;

#define _MAKE_ENUM_13(enumName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13) \
  typedef enum { opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13 } enumName;

#define _MAKE_ENUM_14(enumName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14) \
  typedef enum { opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14 } enumName;

#define _MAKE_ENUM_15(enumName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15) \
  typedef enum { opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15 } enumName;

#define _MAKE_ENUM_16(enumName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16) \
  typedef enum { opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16 } enumName;

#define _MAKE_ENUM_17(enumName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17) \
  typedef enum { opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17 } enumName;

#define _MAKE_ENUM_18(enumName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18) \
  typedef enum { opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18 } enumName;

#define _MAKE_ENUM_19(enumName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19) \
  typedef enum { opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19 } enumName;

#define _MAKE_ENUM_20(enumName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20) \
  typedef enum { opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20 } enumName;

#define _MAKE_ENUM_21(enumName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21) \
  typedef enum { opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21 } enumName;

#define _MAKE_ENUM_22(enumName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22) \
  typedef enum { opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22 } enumName;

#define _MAKE_ENUM_23(enumName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23) \
  typedef enum { opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23 } enumName;

#define _MAKE_ENUM_24(enumName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24) \
  typedef enum { opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24 } enumName;

#define _MAKE_ENUM_25(enumName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24, opt25) \
  typedef enum { opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24, opt25 } enumName;

#define _MAKE_ENUM_26(enumName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24, opt25, opt26) \
  typedef enum { opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24, opt25, opt26 } enumName;

#define _MAKE_ENUM_27(enumName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24, opt25, opt26, opt27) \
  typedef enum { opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24, opt25, opt26, opt27 } enumName;

#define _MAKE_ENUM_28(enumName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24, opt25, opt26, opt27, opt28) \
  typedef enum { opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24, opt25, opt26, opt27, opt28 } enumName;

#define _MAKE_ENUM_29(enumName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24, opt25, opt26, opt27, opt28, opt29) \
  typedef enum { opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24, opt25, opt26, opt27, opt28, opt29 } enumName;

#define _MAKE_ENUM_30(enumName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24, opt25, opt26, opt27, opt28, opt29, opt30) \
  typedef enum { opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24, opt25, opt26, opt27, opt28, opt29, opt30 } enumName;

#define _MAKE_ENUM_31(enumName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24, opt25, opt26, opt27, opt28, opt29, opt30, opt31) \
  typedef enum { opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24, opt25, opt26, opt27, opt28, opt29, opt30, opt31 } enumName;

#define _MAKE_ENUM_32(enumName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24, opt25, opt26, opt27, opt28, opt29, opt30, opt31, opt32) \
  typedef enum { opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24, opt25, opt26, opt27, opt28, opt29, opt30, opt31, opt32 } enumName;

#define _MAKE_ENUM_33(enumName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24, opt25, opt26, opt27, opt28, opt29, opt30, opt31, opt32, opt33) \
  typedef enum { opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24, opt25, opt26, opt27, opt28, opt29, opt30, opt31, opt32, opt33 } enumName;

#define _MAKE_ENUM_34(enumName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24, opt25, opt26, opt27, opt28, opt29, opt30, opt31, opt32, opt33, opt34) \
  typedef enum { opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24, opt25, opt26, opt27, opt28, opt29, opt30, opt31, opt32, opt33, opt34 } enumName;

#define _MAKE_ENUM_35(enumName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24, opt25, opt26, opt27, opt28, opt29, opt30, opt31, opt32, opt33, opt34, opt35) \
  typedef enum { opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24, opt25, opt26, opt27, opt28, opt29, opt30, opt31, opt32, opt33, opt34, opt35 } enumName;

#define _MAKE_ENUM_36(enumName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24, opt25, opt26, opt27, opt28, opt29, opt30, opt31, opt32, opt33, opt34, opt35, opt36) \
  typedef enum { opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24, opt25, opt26, opt27, opt28, opt29, opt30, opt31, opt32, opt33, opt34, opt35, opt36 } enumName;

#define _MAKE_ENUM_37(enumName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24, opt25, opt26, opt27, opt28, opt29, opt30, opt31, opt32, opt33, opt34, opt35, opt36, opt37) \
  typedef enum { opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24, opt25, opt26, opt27, opt28, opt29, opt30, opt31, opt32, opt33, opt34, opt35, opt36, opt37 } enumName;

#define _MAKE_ENUM_38(enumName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24, opt25, opt26, opt27, opt28, opt29, opt30, opt31, opt32, opt33, opt34, opt35, opt36, opt37, opt38) \
  typedef enum { opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24, opt25, opt26, opt27, opt28, opt29, opt30, opt31, opt32, opt33, opt34, opt35, opt36, opt37, opt38 } enumName;

#define _MAKE_ENUM_39(enumName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24, opt25, opt26, opt27, opt28, opt29, opt30, opt31, opt32, opt33, opt34, opt35, opt36, opt37, opt38, opt39) \
  typedef enum { opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24, opt25, opt26, opt27, opt28, opt29, opt30, opt31, opt32, opt33, opt34, opt35, opt36, opt37, opt38, opt39 } enumName;

#define _MAKE_ENUM_40(enumName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24, opt25, opt26, opt27, opt28, opt29, opt30, opt31, opt32, opt33, opt34, opt35, opt36, opt37, opt38, opt39, opt40) \
  typedef enum { opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24, opt25, opt26, opt27, opt28, opt29, opt30, opt31, opt32, opt33, opt34, opt35, opt36, opt37, opt38, opt39, opt40 } enumName;


// The _MAKE_ENUM_REPRS_N macro implementations; defines a string array containing
// the enum string values. This must stay in the same order as the enum declaration itself,
// otherwise madness ensues. This convenience is the main motivation for this macro madness.
// Used in the MAKE_ENUM macro below, in conjunction with the _MAKE_ENUM_N macro above
#define _MAKE_ENUM_REPRS_1(strValuesName, opt1) \
  static const char* strValuesName[] = { #opt1 };

#define _MAKE_ENUM_REPRS_2(strValuesName, opt1, opt2) \
  static const char* strValuesName[] = { #opt1, #opt2 };

#define _MAKE_ENUM_REPRS_3(strValuesName, opt1, opt2, opt3) \
  static const char* strValuesName[] = { #opt1, #opt2, #opt3 };

#define _MAKE_ENUM_REPRS_4(strValuesName, opt1, opt2, opt3, opt4) \
  static const char* strValuesName[] = { #opt1, #opt2, #opt3, #opt4 };

#define _MAKE_ENUM_REPRS_5(strValuesName, opt1, opt2, opt3, opt4, opt5) \
  static const char* strValuesName[] = { #opt1, #opt2, #opt3, #opt4, #opt5 };

#define _MAKE_ENUM_REPRS_6(strValuesName, opt1, opt2, opt3, opt4, opt5, opt6) \
  static const char* strValuesName[] = { #opt1, #opt2, #opt3, #opt4, #opt5, #opt6 };

#define _MAKE_ENUM_REPRS_7(strValuesName, opt1, opt2, opt3, opt4, opt5, opt6, opt7) \
  static const char* strValuesName[] = { #opt1, #opt2, #opt3, #opt4, #opt5, #opt6, #opt7 };

#define _MAKE_ENUM_REPRS_8(strValuesName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8) \
  static const char* strValuesName[] = { #opt1, #opt2, #opt3, #opt4, #opt5, #opt6, #opt7, #opt8 };

#define _MAKE_ENUM_REPRS_9(strValuesName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9) \
  static const char* strValuesName[] = { #opt1, #opt2, #opt3, #opt4, #opt5, #opt6, #opt7, #opt8, #opt9 };

#define _MAKE_ENUM_REPRS_10(strValuesName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10) \
  static const char* strValuesName[] = { #opt1, #opt2, #opt3, #opt4, #opt5, #opt6, #opt7, #opt8, #opt9, #opt10 };

#define _MAKE_ENUM_REPRS_11(strValuesName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11) \
  static const char* strValuesName[] = { #opt1, #opt2, #opt3, #opt4, #opt5, #opt6, #opt7, #opt8, #opt9, #opt10, #opt11 };

#define _MAKE_ENUM_REPRS_12(strValuesName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12) \
  static const char* strValuesName[] = { #opt1, #opt2, #opt3, #opt4, #opt5, #opt6, #opt7, #opt8, #opt9, #opt10, #opt11, #opt12 };

#define _MAKE_ENUM_REPRS_13(strValuesName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13) \
  static const char* strValuesName[] = { #opt1, #opt2, #opt3, #opt4, #opt5, #opt6, #opt7, #opt8, #opt9, #opt10, #opt11, #opt12, #opt13 };

#define _MAKE_ENUM_REPRS_14(strValuesName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14) \
  static const char* strValuesName[] = { #opt1, #opt2, #opt3, #opt4, #opt5, #opt6, #opt7, #opt8, #opt9, #opt10, #opt11, #opt12, #opt13, #opt14 };

#define _MAKE_ENUM_REPRS_15(strValuesName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15) \
  static const char* strValuesName[] = { #opt1, #opt2, #opt3, #opt4, #opt5, #opt6, #opt7, #opt8, #opt9, #opt10, #opt11, #opt12, #opt13, #opt14, #opt15 };

#define _MAKE_ENUM_REPRS_16(strValuesName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16) \
  static const char* strValuesName[] = { #opt1, #opt2, #opt3, #opt4, #opt5, #opt6, #opt7, #opt8, #opt9, #opt10, #opt11, #opt12, #opt13, #opt14, #opt15, #opt16 };

#define _MAKE_ENUM_REPRS_17(strValuesName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17) \
  static const char* strValuesName[] = { #opt1, #opt2, #opt3, #opt4, #opt5, #opt6, #opt7, #opt8, #opt9, #opt10, #opt11, #opt12, #opt13, #opt14, #opt15, #opt16, #opt17 };

#define _MAKE_ENUM_REPRS_18(strValuesName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18) \
  static const char* strValuesName[] = { #opt1, #opt2, #opt3, #opt4, #opt5, #opt6, #opt7, #opt8, #opt9, #opt10, #opt11, #opt12, #opt13, #opt14, #opt15, #opt16, #opt17, #opt18 };

#define _MAKE_ENUM_REPRS_19(strValuesName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19) \
  static const char* strValuesName[] = { #opt1, #opt2, #opt3, #opt4, #opt5, #opt6, #opt7, #opt8, #opt9, #opt10, #opt11, #opt12, #opt13, #opt14, #opt15, #opt16, #opt17, #opt18, #opt19 };

#define _MAKE_ENUM_REPRS_20(strValuesName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20) \
  static const char* strValuesName[] = { #opt1, #opt2, #opt3, #opt4, #opt5, #opt6, #opt7, #opt8, #opt9, #opt10, #opt11, #opt12, #opt13, #opt14, #opt15, #opt16, #opt17, #opt18, #opt19, #opt20 };

#define _MAKE_ENUM_REPRS_21(strValuesName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21) \
  static const char* strValuesName[] = { #opt1, #opt2, #opt3, #opt4, #opt5, #opt6, #opt7, #opt8, #opt9, #opt10, #opt11, #opt12, #opt13, #opt14, #opt15, #opt16, #opt17, #opt18, #opt19, #opt20, #opt21 };

#define _MAKE_ENUM_REPRS_22(strValuesName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22) \
  static const char* strValuesName[] = { #opt1, #opt2, #opt3, #opt4, #opt5, #opt6, #opt7, #opt8, #opt9, #opt10, #opt11, #opt12, #opt13, #opt14, #opt15, #opt16, #opt17, #opt18, #opt19, #opt20, #opt21, #opt22 };

#define _MAKE_ENUM_REPRS_23(strValuesName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23) \
  static const char* strValuesName[] = { #opt1, #opt2, #opt3, #opt4, #opt5, #opt6, #opt7, #opt8, #opt9, #opt10, #opt11, #opt12, #opt13, #opt14, #opt15, #opt16, #opt17, #opt18, #opt19, #opt20, #opt21, #opt22, #opt23 };

#define _MAKE_ENUM_REPRS_24(strValuesName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24) \
  static const char* strValuesName[] = { #opt1, #opt2, #opt3, #opt4, #opt5, #opt6, #opt7, #opt8, #opt9, #opt10, #opt11, #opt12, #opt13, #opt14, #opt15, #opt16, #opt17, #opt18, #opt19, #opt20, #opt21, #opt22, #opt23, #opt24 };

#define _MAKE_ENUM_REPRS_25(strValuesName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24, opt25) \
  static const char* strValuesName[] = { #opt1, #opt2, #opt3, #opt4, #opt5, #opt6, #opt7, #opt8, #opt9, #opt10, #opt11, #opt12, #opt13, #opt14, #opt15, #opt16, #opt17, #opt18, #opt19, #opt20, #opt21, #opt22, #opt23, #opt24, #opt25 };

#define _MAKE_ENUM_REPRS_26(strValuesName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24, opt25, opt26) \
  static const char* strValuesName[] = { #opt1, #opt2, #opt3, #opt4, #opt5, #opt6, #opt7, #opt8, #opt9, #opt10, #opt11, #opt12, #opt13, #opt14, #opt15, #opt16, #opt17, #opt18, #opt19, #opt20, #opt21, #opt22, #opt23, #opt24, #opt25, #opt26 };

#define _MAKE_ENUM_REPRS_27(strValuesName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24, opt25, opt26, opt27) \
  static const char* strValuesName[] = { #opt1, #opt2, #opt3, #opt4, #opt5, #opt6, #opt7, #opt8, #opt9, #opt10, #opt11, #opt12, #opt13, #opt14, #opt15, #opt16, #opt17, #opt18, #opt19, #opt20, #opt21, #opt22, #opt23, #opt24, #opt25, #opt26, #opt27 };

#define _MAKE_ENUM_REPRS_28(strValuesName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24, opt25, opt26, opt27, opt28) \
  static const char* strValuesName[] = { #opt1, #opt2, #opt3, #opt4, #opt5, #opt6, #opt7, #opt8, #opt9, #opt10, #opt11, #opt12, #opt13, #opt14, #opt15, #opt16, #opt17, #opt18, #opt19, #opt20, #opt21, #opt22, #opt23, #opt24, #opt25, #opt26, #opt27, #opt28 };

#define _MAKE_ENUM_REPRS_29(strValuesName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24, opt25, opt26, opt27, opt28, opt29) \
  static const char* strValuesName[] = { #opt1, #opt2, #opt3, #opt4, #opt5, #opt6, #opt7, #opt8, #opt9, #opt10, #opt11, #opt12, #opt13, #opt14, #opt15, #opt16, #opt17, #opt18, #opt19, #opt20, #opt21, #opt22, #opt23, #opt24, #opt25, #opt26, #opt27, #opt28, #opt29 };

#define _MAKE_ENUM_REPRS_30(strValuesName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24, opt25, opt26, opt27, opt28, opt29, opt30) \
  static const char* strValuesName[] = { #opt1, #opt2, #opt3, #opt4, #opt5, #opt6, #opt7, #opt8, #opt9, #opt10, #opt11, #opt12, #opt13, #opt14, #opt15, #opt16, #opt17, #opt18, #opt19, #opt20, #opt21, #opt22, #opt23, #opt24, #opt25, #opt26, #opt27, #opt28, #opt29, #opt30 };

#define _MAKE_ENUM_REPRS_31(strValuesName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24, opt25, opt26, opt27, opt28, opt29, opt30, opt31) \
  static const char* strValuesName[] = { #opt1, #opt2, #opt3, #opt4, #opt5, #opt6, #opt7, #opt8, #opt9, #opt10, #opt11, #opt12, #opt13, #opt14, #opt15, #opt16, #opt17, #opt18, #opt19, #opt20, #opt21, #opt22, #opt23, #opt24, #opt25, #opt26, #opt27, #opt28, #opt29, #opt30, #opt31 };

#define _MAKE_ENUM_REPRS_32(strValuesName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24, opt25, opt26, opt27, opt28, opt29, opt30, opt31, opt32) \
  static const char* strValuesName[] = { #opt1, #opt2, #opt3, #opt4, #opt5, #opt6, #opt7, #opt8, #opt9, #opt10, #opt11, #opt12, #opt13, #opt14, #opt15, #opt16, #opt17, #opt18, #opt19, #opt20, #opt21, #opt22, #opt23, #opt24, #opt25, #opt26, #opt27, #opt28, #opt29, #opt30, #opt31, #opt32 };

#define _MAKE_ENUM_REPRS_33(strValuesName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24, opt25, opt26, opt27, opt28, opt29, opt30, opt31, opt32, opt33) \
  static const char* strValuesName[] = { #opt1, #opt2, #opt3, #opt4, #opt5, #opt6, #opt7, #opt8, #opt9, #opt10, #opt11, #opt12, #opt13, #opt14, #opt15, #opt16, #opt17, #opt18, #opt19, #opt20, #opt21, #opt22, #opt23, #opt24, #opt25, #opt26, #opt27, #opt28, #opt29, #opt30, #opt31, #opt32, #opt33 };

#define _MAKE_ENUM_REPRS_34(strValuesName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24, opt25, opt26, opt27, opt28, opt29, opt30, opt31, opt32, opt33, opt34) \
  static const char* strValuesName[] = { #opt1, #opt2, #opt3, #opt4, #opt5, #opt6, #opt7, #opt8, #opt9, #opt10, #opt11, #opt12, #opt13, #opt14, #opt15, #opt16, #opt17, #opt18, #opt19, #opt20, #opt21, #opt22, #opt23, #opt24, #opt25, #opt26, #opt27, #opt28, #opt29, #opt30, #opt31, #opt32, #opt33, #opt34 };

#define _MAKE_ENUM_REPRS_35(strValuesName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24, opt25, opt26, opt27, opt28, opt29, opt30, opt31, opt32, opt33, opt34, opt35) \
  static const char* strValuesName[] = { #opt1, #opt2, #opt3, #opt4, #opt5, #opt6, #opt7, #opt8, #opt9, #opt10, #opt11, #opt12, #opt13, #opt14, #opt15, #opt16, #opt17, #opt18, #opt19, #opt20, #opt21, #opt22, #opt23, #opt24, #opt25, #opt26, #opt27, #opt28, #opt29, #opt30, #opt31, #opt32, #opt33, #opt34, #opt35 };

#define _MAKE_ENUM_REPRS_36(strValuesName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24, opt25, opt26, opt27, opt28, opt29, opt30, opt31, opt32, opt33, opt34, opt35, opt36) \
  static const char* strValuesName[] = { #opt1, #opt2, #opt3, #opt4, #opt5, #opt6, #opt7, #opt8, #opt9, #opt10, #opt11, #opt12, #opt13, #opt14, #opt15, #opt16, #opt17, #opt18, #opt19, #opt20, #opt21, #opt22, #opt23, #opt24, #opt25, #opt26, #opt27, #opt28, #opt29, #opt30, #opt31, #opt32, #opt33, #opt34, #opt35, #opt36 };

#define _MAKE_ENUM_REPRS_37(strValuesName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24, opt25, opt26, opt27, opt28, opt29, opt30, opt31, opt32, opt33, opt34, opt35, opt36, opt37) \
  static const char* strValuesName[] = { #opt1, #opt2, #opt3, #opt4, #opt5, #opt6, #opt7, #opt8, #opt9, #opt10, #opt11, #opt12, #opt13, #opt14, #opt15, #opt16, #opt17, #opt18, #opt19, #opt20, #opt21, #opt22, #opt23, #opt24, #opt25, #opt26, #opt27, #opt28, #opt29, #opt30, #opt31, #opt32, #opt33, #opt34, #opt35, #opt36, #opt37 };

#define _MAKE_ENUM_REPRS_38(strValuesName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24, opt25, opt26, opt27, opt28, opt29, opt30, opt31, opt32, opt33, opt34, opt35, opt36, opt37, opt38) \
  static const char* strValuesName[] = { #opt1, #opt2, #opt3, #opt4, #opt5, #opt6, #opt7, #opt8, #opt9, #opt10, #opt11, #opt12, #opt13, #opt14, #opt15, #opt16, #opt17, #opt18, #opt19, #opt20, #opt21, #opt22, #opt23, #opt24, #opt25, #opt26, #opt27, #opt28, #opt29, #opt30, #opt31, #opt32, #opt33, #opt34, #opt35, #opt36, #opt37, #opt38 };

#define _MAKE_ENUM_REPRS_39(strValuesName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24, opt25, opt26, opt27, opt28, opt29, opt30, opt31, opt32, opt33, opt34, opt35, opt36, opt37, opt38, opt39) \
  static const char* strValuesName[] = { #opt1, #opt2, #opt3, #opt4, #opt5, #opt6, #opt7, #opt8, #opt9, #opt10, #opt11, #opt12, #opt13, #opt14, #opt15, #opt16, #opt17, #opt18, #opt19, #opt20, #opt21, #opt22, #opt23, #opt24, #opt25, #opt26, #opt27, #opt28, #opt29, #opt30, #opt31, #opt32, #opt33, #opt34, #opt35, #opt36, #opt37, #opt38, #opt39 };

#define _MAKE_ENUM_REPRS_40(strValuesName, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9, opt10, opt11, opt12, opt13, opt14, opt15, opt16, opt17, opt18, opt19, opt20, opt21, opt22, opt23, opt24, opt25, opt26, opt27, opt28, opt29, opt30, opt31, opt32, opt33, opt34, opt35, opt36, opt37, opt38, opt39, opt40) \
  static const char* strValuesName[] = { #opt1, #opt2, #opt3, #opt4, #opt5, #opt6, #opt7, #opt8, #opt9, #opt10, #opt11, #opt12, #opt13, #opt14, #opt15, #opt16, #opt17, #opt18, #opt19, #opt20, #opt21, #opt22, #opt23, #opt24, #opt25, #opt26, #opt27, #opt28, #opt29, #opt30, #opt31, #opt32, #opt33, #opt34, #opt35, #opt36, #opt37, #opt38, #opt39, #opt40 };

// The necessary indirection layer
#define _CONCAT(x, y) x ## _ ## y
#define CONCATTER(x, y) _CONCAT(x, y)

// Defines an enum with `enumName`, and string representation array with `strValuesName`, for
// the varargs of options. Defers to underlying implementations depending on number of options passed.
#define MAKE_ENUM(enumName, strValuesName, ...) \
  CONCATTER(_MAKE_ENUM, VA_NARGS(__VA_ARGS__))(enumName, __VA_ARGS__) \
  CONCATTER(_MAKE_ENUM_REPRS, VA_NARGS(__VA_ARGS__))(strValuesName, __VA_ARGS__)

#endif //CLYRA_ENUM_H
