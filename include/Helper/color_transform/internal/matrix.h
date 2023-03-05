#pragma once
#if !defined(__cpp_consteval)
#    error "this need consteval"
#endif
#include <cstddef>

namespace CT {

template<typename Elem>
struct Vec
{
    Elem v[4];
    consteval Vec(Elem v0, Elem v1, Elem v2, Elem v3)
        : v{v0, v1, v2, v3}
    {}
    consteval Vec(Elem const e)
        : v{e, e, e, e}
    {}
    consteval Vec(const Vec &) = default;
    consteval Elem &operator[](size_t index)
    {
#ifdef DEBUG
        assert(index < 4);
#endif
        return v[index];
    }
    static consteval Vec Init(Elem const v)
    {
        return Vec<Elem>{v, v, v, v};
    }
    friend consteval Elem Sum(Vec v)
    {
        return v[0] + v[1] + v[2] + v[3];
    }
    friend consteval Vec operator+(Vec l, Vec r)
    {
        return {l.v[0] + r.v[0], l.v[1] + r.v[1], l.v[2] + r.v[2], l.v[3] + r.v[3]};
    }

    friend consteval Vec operator-(Vec l, Vec r)
    {
        return {l.v[0] - r.v[0], l.v[1] - r.v[1], l.v[2] - r.v[2], l.v[3] - r.v[3]};
    }

    friend consteval Vec operator*(Vec l, Vec r)
    {
        return {l.v[0] * r.v[0], l.v[1] * r.v[1], l.v[2] * r.v[2], l.v[3] * r.v[3]};
    }

    friend consteval Vec operator/(Vec l, Vec r)
    {
        return {l.v[0] / r.v[0], l.v[1] / r.v[1], l.v[2] / r.v[2], l.v[3] / r.v[3]};
    }
    friend consteval bool operator==(Vec l, Vec r)
    {
        return l.v[0] == r.v[0] && l.v[1] == r.v[1] && l.v[2] == r.v[2] && l.v[3] == r.v[3];
    }
    friend consteval bool operator!=(Vec l, Vec r)
    {
        return !(l == r);
    }
};

template<typename Elem>
struct Mat : public Vec<Vec<Elem>>
{
    using Vec<Vec<Elem>>::v;
    /* using Vec<Vec<Elem>>::Vec;  this wont let Mat::Mat(which inherit from Vec) Be consteval*/
    template<typename ...ARGS>
    consteval Mat(ARGS...ags):Vec<Vec<Elem>>(ags...){}
    consteval Mat Transpose()
    {
        return {
            {v[0][0], v[1][0], v[2][0], v[3][0]},
            {v[0][1], v[1][1], v[2][1], v[3][1]},
            {v[0][2], v[1][2], v[2][2], v[3][2]},
            {v[0][3], v[1][3], v[2][3], v[3][3]}
        };
    }
    friend consteval Mat       operator/(Mat l, Mat r) = delete;
    friend consteval Vec<Elem> Sum(Mat v) = delete;
    friend consteval Mat       operator*(Mat l, Mat r)
    {
        Mat t = r.Transpose();
        return {
            {Sum(l[0] * t[0]), Sum(l[0] * t[1]), Sum(l[0] * t[2]), Sum(l[0] * t[3])},
            {Sum(l[1] * t[0]), Sum(l[1] * t[1]), Sum(l[1] * t[2]), Sum(l[1] * t[3])},
            {Sum(l[2] * t[0]), Sum(l[2] * t[1]), Sum(l[2] * t[2]), Sum(l[2] * t[3])},
            {Sum(l[3] * t[0]), Sum(l[3] * t[1]), Sum(l[3] * t[2]), Sum(l[3] * t[3])}
        };
    }
};
} // namespace CT