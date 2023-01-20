#include <Helper/macro_util/macro_util.h>
#include <Helper/log/log.h>
#include <spdlog/fmt/bundled/compile.h>
#define EXPAND(x)     x
#define APPEND_ADD(x) x +
#define AA            APPEND_ADD
#define VC_0(...)
#define VC_1(m, x, ...)            EXPAND(m(x) VC_0(m, __VA_ARGS__))
#define VC_2(m, x, ...)            EXPAND(m(x) VC_1(m, __VA_ARGS__))
#define VC_3(m, x, ...)            EXPAND(m(x) VC_2(m, __VA_ARGS__))
#define VC_4(m, x, ...)            EXPAND(m(x) VC_3(m, __VA_ARGS__))
#define SKIP_4(a, s, d, f, x, ...) x
#define VC(m, ...)                 EXPAND(EXPAND(SKIP_4(__VA_ARGS__, VC_4, VC_3, VC_2, VC_1, VC_0))(m, __VA_ARGS__)
// template<template<int N> typename char file_name[N] >

#define LOG_ASSERT(...) HLOG_ASSERT(DefaultLogger(), __VA_ARGS__)
int main()
{
    auto pos_str = POS_STR;
    DefaultLogger()->info("pos_str : {}", pos_str);

    constexpr int  cm = COUNT_MACRO(1, 2, 3, s);
    LOG_ASSERT(cm == 4);

    constexpr auto va = VAR_CALL(AA, 1, 2, 3, 4) 0;
    LOG_ASSERT(va == 10);

    int            a = 1, b = 2;
    char           c = '3';
    auto           params_str = VAR_PARAMS_STR(a, b, c);
    LOG_ASSERT(params_str == "params[a:1,b:2,c:3,]");
}