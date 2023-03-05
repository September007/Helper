#include <Helper/color_transform/internal/matrix.h>
#include <cassert>
#include <algorithm>
#include <Helper/glm/matrix.hpp>
#define EXPECT_EQ(l, r) assert((l) == (r))
#define CEXPECT_EQ(l, r) static_assert((l) == (r))
using namespace CT;

template<typename Elem,const Vec<Elem> v[4]>
void ConstEval()
{
    constexpr Vec<Elem> v0 = v[0],v1=v[1], v2=v[2], v3=v[3];
    constexpr Vec       v1a2 = v1 + v2, v2a1 = v2 + v1;
    constexpr Vec       v2mi1 = v2 - v1;
    constexpr Vec       v2mu1 = v2 * v1;

    constexpr Vec v3d2 = v3 / v2;
    CEXPECT_EQ(v1a2, v3);
    CEXPECT_EQ(v1a2, v2a1);
    CEXPECT_EQ(v2mi1, v1);
    CEXPECT_EQ(v2mu1, v2);
    CEXPECT_EQ(v3d2, v1);
}

using VI=Vec<int>;
using VVI=Vec<Vec<int>>;

int main()
{
    static constexpr Vec<int> VI4[]{Vec<int>::Init(0), Vec<int>::Init(1), Vec<int>::Init(2), Vec<int>::Init(3)};
    ConstEval<int, VI4>();
    static constexpr Vec<Vec<int>> VVI4[]{Vec<Vec<int>>::Init(Vec<int>::Init(0)), Vec<Vec<int>>::Init(Vec<int>::Init(1)), Vec<Vec<int>>::Init(Vec<int>::Init(2)), Vec<Vec<int>>::Init(Vec<int>::Init(3))};
    static constexpr Mat<int> M[4] = {VVI4[0],VVI4[1],VVI4[2],VVI4[3]};
    static constexpr Vec<Vec<int>> VVI4M[]={M[0],M[1],M[2],M[3]};
    ConstEval<Vec<int>,VVI4M>();

    Mat<int> e = {
        VI{1, 0, 0, 0},
        VI{0, 1, 0, 0},
        VI{0, 0, 1, 0},
        VI{0, 0, 0, 1}
    };
    Mat<int> 
}
