#pragma once
#include <boost/describe.hpp>
#include <spdlog/fmt/bundled/format.h> /* this comes from spdlog,so dont need import fmt again */
#include <type_traits>

/* this comes from https://www.boost.org/doc/libs/master/libs/describe/doc/html/describe.html#example_fmtlib_enum_formatter */

template<class T>
struct fmt::formatter<T, char, std::enable_if_t<boost::describe::has_describe_enumerators<T>::value>>
{
private:
    using U = std::underlying_type_t<T>;

    fmt::formatter<fmt::string_view, char> sf_;
    fmt::formatter<U, char>                nf_;

public:
    constexpr auto parse(format_parse_context &ctx)
    {
        auto i1 = sf_.parse(ctx);
        auto i2 = nf_.parse(ctx);

        if (i1 != i2)
        {
            ctx.error_handler().on_error("invalid format");
        }

        return i1;
    }

    auto format(T const &t, format_context &ctx) const
    {
        char const *s = boost::describe::enum_to_string(t, 0);

        if (s)
        {
            return sf_.format(s, ctx);
        }
        else
        {
            return nf_.format(static_cast<U>(t), ctx);
        }
    }
};
