#include <Helper/color_transform/color_standard.h>
#include <Helper/log/log.h>

#define fmt_str(x) #x ":{} "
#define ASSERT(x)                                                                                                                          \
    if ((!x))                                                                                                                              \
    DefaultLogger()->error("ASSERT({}) failed at {}:{}", #x,__FILE__,__LINE__);

int main()
{
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
        {
            ASSERT(std::fabs(CS_BT601::RGB2YUV_CM[i][j] - Generated_CM<CS_BT601>::RGB2YUV_CM[i][j]) < 0.001);
        }
}