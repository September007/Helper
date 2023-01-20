#pragma once
#ifdef _MSC_VER
#include <assert.h>
#    define __ASSERT assert
#else
#define __ASSERT fill-this


#endif
#ifdef Debug
#    define DebugArea(...) __VA_ARGS__
#else
#    define DebugArea(...)
#endif

#define ASSERT(...)            __ASSERT(__VA_ARGS__)

// varic call
// define AA(x) x+
// constexpr int vc = VAR_CALL(AA,1,2,3,4)0;// vc==10
#define VC_EXPAND(x)                  x
#define VAR_CALL_0(mac, ...)
#define VAR_CALL_1(mac, x, ...)  VC_EXPAND(mac(x) VAR_CALL_0(mac, __VA_ARGS__))
#define VAR_CALL_2(mac, x, ...)  VC_EXPAND(mac(x) VAR_CALL_1(mac, __VA_ARGS__))
#define VAR_CALL_3(mac, x, ...)  VC_EXPAND(mac(x) VAR_CALL_2(mac, __VA_ARGS__))
#define VAR_CALL_4(mac, x, ...)  VC_EXPAND(mac(x) VAR_CALL_3(mac, __VA_ARGS__))
#define VAR_CALL_5(mac, x, ...)  VC_EXPAND(mac(x) VAR_CALL_4(mac, __VA_ARGS__))
#define VAR_CALL_6(mac, x, ...)  VC_EXPAND(mac(x) VAR_CALL_5(mac, __VA_ARGS__))
#define VAR_CALL_7(mac, x, ...)  VC_EXPAND(mac(x) VAR_CALL_6(mac, __VA_ARGS__))
#define VAR_CALL_8(mac, x, ...)  VC_EXPAND(mac(x) VAR_CALL_7(mac, __VA_ARGS__))
#define VAR_CALL_9(mac, x, ...)  VC_EXPAND(mac(x) VAR_CALL_8(mac, __VA_ARGS__))
#define VAR_CALL_10(mac, x, ...) VC_EXPAND(mac(x) VAR_CALL_9(mac, __VA_ARGS__))

#define SKIP_10(s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, ...) s10
#define VAR_CALL(mac, ...)                                                                                                                 \
    VC_EXPAND(SKIP_10(__VA_ARGS__, VAR_CALL_10, VAR_CALL_9, VAR_CALL_8, VAR_CALL_7, VAR_CALL_6, VAR_CALL_5, VAR_CALL_4, VAR_CALL_3, VAR_CALL_2,      \
        VAR_CALL_1, VAR_CALL_0)                                                                                                       \
    (mac,__VA_ARGS__))

// count macro param number
// constexpr int cm = COUNT_MACRO(1,2,3,4) // 4
#define _AA(x) +1
#define COUNT_MACRO(...) (VAR_CALL(_AA,__VA_ARGS__))

// todo: varibale_length macro

// mpl:macro-param-len, l: this-macro-param-len, m:this-macro
#define VLM_L_M(mpl, l, m, ...) mpl == l ? m(__VA_ARGS__):
#define VLM_L_M_0(mpl,l,m)  VC_EXPAND(VLM_L_M(mpl,l,m) MACRO_WITH_THIS_LENGTH_PARAM_IS_NOT_FOUND)
#define VLM_L_M_1(mpl, l, m, ...)  VC_EXPAND(VLM(mpl, l, m) VLM_L_M_0(__VA_ARGS__))
#define VLM_L_M_2(mpl, l, m, ...)  VC_EXPAND(VLM(mpl, l, m) VLM_L_M_1(__VA_ARGS__))
#define VLM_L_M_3(mpl, l, m, ...)  VC_EXPAND(VLM(mpl, l, m) VLM_L_M_2(__VA_ARGS__))
#define VLM_L_M_4(mpl, l, m, ...)  VC_EXPAND(VLM(mpl, l, m) VLM_L_M_3(__VA_ARGS__))
#define VLM_L_M_5(mpl, l, m, ...)  VC_EXPAND(VLM(mpl, l, m) VLM_L_M_4(__VA_ARGS__))
#define VLM_L_M_6(mpl, l, m, ...)  VC_EXPAND(VLM(mpl, l, m) VLM_L_M_5(__VA_ARGS__))
#define VLM_L_M_7(mpl, l, m, ...)  VC_EXPAND(VLM(mpl, l, m) VLM_L_M_6(__VA_ARGS__))
#define VLM_L_M_8(mpl, l, m, ...)  VC_EXPAND(VLM(mpl, l, m) VLM_L_M_7(__VA_ARGS__))
#define VLM_L_M_9(mpl, l, m, ...)  VC_EXPAND(VLM(mpl, l, m) VLM_L_M_8(__VA_ARGS__))
#define VLM_L_M_10(mpl, l, m, ...) VC_EXPAND(VLM(mpl, l, m) VLM_L_M_9(__VA_ARGS__))

#define VLM_CALL(mpl,...) 