#pragma once

/* rgb to yuv standard */
/*
Kg = 1- Kr - Kb

Y = Kr*R + Kg*G + Kb*B
V = (R-Y)/(1-Kr) = R - G * Kg/(1-Kr) - B * Kb/(1-Kr)
U = (B-Y)/(1-Kb) = - R * Kr/(1-Kb) - G * Kg/(1-Kb) + B

R = Y + V*(1-Kr)
G = Y - U*(1-Kb)*Kb/Kg - V*(1-Kr)*Kr/Kg
B = Y + U*(1-Kb)
*/

typedef float Factor;

/* color standard */
struct CS_BT601
{
    constexpr static Factor Rf = 0.299f;
    constexpr static Factor Gf = 0.587f;
    constexpr static Factor Bf = 0.114f;
    constexpr static Factor RGB2YUV_CM[3][3] = {
        {0.299,     0.587,     0.114    },
        {-0.168736, -0.331264, 0.5      },
        {0.5,       -0.418688, -0.081312}
    };
    constexpr static Factor YUV2RGB_CM[3][3] = {
        {1., 0.,        1.042    },
        {1., -0.344136, -0.714136},
        {1., 1.772,     0        }
    };
};
struct BT709
{
    constexpr static Factor Rf = 0.2126f;
    constexpr static Factor Gf = 0.7152f;
    constexpr static Factor Bf = 0.0722f;
    constexpr static Factor RGB2YUV_CM[3][3] = {
        {0.2126,    0.7152,    0.0722   },
        {-0.114572, -0.385428, 0.5      },
        {0.5,       -0.454153, -0.045847}
    };
    constexpr static Factor YUV2RGB_CM[3][3] = {
        {1., 0.,        1.5748   },
        {1,  -0.187324, -0.468124},
        {1., 1.8556,    0.        }
    };
};
template<typename ColorStandard=CS_BT601>
struct Generated_CM:ColorStandard {
    constexpr static Factor RGB2YUV_CM[3][3]={
        {Rf,Gf,Bf},
        {-Rf/(1-Bf)/2,-Gf/(1-Bf)/2,1./2},
        {1./2,-Gf/(1-Rf)/2,-Bf/(1-Rf)/2}
    };
};