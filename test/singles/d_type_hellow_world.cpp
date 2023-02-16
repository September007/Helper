
#include <Helper/project/project.h>
#include <Helper/d_type/_d_type.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <Helper/stb_image/stb_image_write.h>

#define DT_ASSET_ROOT   CMAKE_SOURCE_DIR "/extern/D-Type/"
#define DT_ASSET(asset) DT_ASSET_ROOT asset

#include <core-cpp/dtype_l1.h>
// D-Type CPP Engine
#include "core-cpp/dtengine.cpp"

#include <vector>

#include <cstring>
#include <functional>
#define DEC_ATTR(type, name, def) type name = def;
static struct EXPORT_ATTR_INTERFACE
{
    DEC_ATTR(int32_t, BaseFontWidth, 20);
    DEC_ATTR(int32_t, BaseFontHeight, 20);
    DEC_ATTR(int32_t, BaseDrawOffsetX, 0);
    DEC_ATTR(int32_t, BaseDrawOffsetY, 0);

} attr;
#define DEC_ATTR(type, name, def)                                                                                                          \
    extern "C" decltype(attr.name) *Get##name()                                                                                            \
    {                                                                                                                                      \
        return &attr.name;                                                                                                                 \
    }

DEC_ATTR(int32_t, BaseFontWidth, 20);
DEC_ATTR(int32_t, BaseFontHeight, 20);
DEC_ATTR(int32_t, BaseDrawOffsetX, 0);
DEC_ATTR(int32_t, BaseDrawOffsetY, 0);

void my_font_error_func(const DT_CHAR *error_message, void *user_param)
{
    printf("Error message returned by D-Type Engine: %s\r\n", error_message);
}

void draw(int width = 600, int height = 400)
{
    DT_DTENGINE engine;

    DT_MDC          dc_mem;
    DT_TYPE_ATTRIBS type = {0, 0, 0, 0, 0, {{100, 100, 0, 0, 0}}};

    auto dtc = CreateDT_Context();
    engine = dtc->engine;

    /* Create a 640x480-pixel 24-bpp memory surface */
    dc_mem.w = width;
    dc_mem.h = height, dc_mem.l = 3 * dc_mem.w * dc_mem.h;
    if ((dc_mem.m = (DT_UBYTE *)malloc(dc_mem.l)) == DV_NULL)
        exit(0);
    memset(dc_mem.m, 200, dc_mem.l);
    dtFontSetErrorCallback(dtc->engine, my_font_error_func, DV_NULL);
    /* Redirect all D-Type output to that surface and define clipping rect */
    dtOutputSetAsMDC(dtc->engine, DV_FORMAT_24, 0, &dc_mem, 0, 0, width, height);

    /* Select type */
    dtTypesetterSetTypeAttribs(dtc->engine, &type, 0);
    DT_BMP pa;
    pa.w = dc_mem.w;
    pa.h = dc_mem.h;
    pa.l = dc_mem.l;
    pa.x = 0;
    pa.y = 0;
    TX_ATTR_SKEW_HOR;
    int x = *GetBaseDrawOffsetX() - 100, y = *GetBaseDrawOffsetY() + 100;
    DV_EFFECT_EDGE_100_NE;
    int  i = 0;
    auto test = [&](DT_STYLE_ATTRIBS *p, DT_ID_ULONG c = 'a')
    {
        dtOutputSetStyleAttribs(dtc->engine, p, 0);
        dtCharDoOutput(dtc->engine, c, x += 100, y, 0, &pa);
    };
    // font analyze
    wchar_t const CHAR = L'标';
    DT_UWORD      font_x, font_y, char_x, char_y;
    DT_RECT_SWORD font_extent, char_extent;
    auto          findex = dtFontGetGlyphIndex(dtc->engine, dtc->cur_font, CHAR);
    auto          fa_ret = dtFontGetMetrics(dtc->engine, dtc->cur_font, &font_x, &font_y, &font_extent);
    auto          ca_ret = dtFontGetMetrics(dtc->engine, findex, &char_x, &char_y, &char_extent);

    // clang-format off
    std::function<void()> funcs[] = {[&]
        { // 0  RGBA
            test(new DT_STYLE_ATTRIBS{{0, 0},{0, 0, 255},0, DV_NULL},L'红');
            test(new DT_STYLE_ATTRIBS{{0, 0},{0, 255, 0},0, DV_NULL},L'绿');
            test(new DT_STYLE_ATTRIBS{{0, 0},{255, 0, 0},0, DV_NULL},L'蓝');
            test(new DT_STYLE_ATTRIBS{{0, 0},{255, 0, 0,128},0, DV_NULL},L'浅');
            test(new DT_STYLE_ATTRIBS{{0, 0},{0, 0, 0, 0},0, DV_NULL},L'黑');},
        [&]
        { // type:size
            DT_TYPE_ATTRIBS type{0, 0, 0, 0, 0, DT_TRANSFORM{
                DT_TRANSFORM_PARAMS_L{.size_h = 50, .size_v = 50, .skew_h = 0, .skew_v = 0, .rotation = 0}
                }};
            dtTypesetterSetTypeAttribs(dtc->engine, &type, 0);
            dtCharDoOutput(dtc->engine, L'标', x += 100, y, 0, &pa);

            type.transform.params.size_h *= 2;
            dtTypesetterSetTypeAttribs(dtc->engine, &type, 0);
            dtCharDoOutput(dtc->engine, L'胖', x += 100, y, 0, &pa);

            type.transform.params.size_v *= 2;
            dtTypesetterSetTypeAttribs(dtc->engine, &type, 0);
            dtCharDoOutput(dtc->engine, L'壮', x += 100, y, 0, &pa);

            type.transform.params.rotation = 30;
            dtTypesetterSetTypeAttribs(dtc->engine, &type, 0);
            dtCharDoOutput(dtc->engine, L'倒', x += 100, y, 0, &pa);

            type.transform.params.skew_h = 30;
            dtTypesetterSetTypeAttribs(dtc->engine, &type, 0);
            dtCharDoOutput(dtc->engine, L'扭', x += 100, y, 0, &pa);

            type.transform.params.skew_v = 30;
            dtTypesetterSetTypeAttribs(dtc->engine, &type, 0);
            dtCharDoOutput(dtc->engine, L'额', x += 100, y, 0, &pa);

        },
        [&]
        { // type:position not working
            DT_TYPE_ATTRIBS type{
                0, 0, 0, 0, 0, DT_TRANSFORM{DT_TRANSFORM_PARAMS_L{.size_h = 50, .size_v = 50, .skew_h = 0, .skew_v = 0, .rotation = 0}}};
            dtTypesetterSetTypeAttribs(dtc->engine, &type, 0);
            dtCharDoOutput(dtc->engine, 'a', x += 100, y, 0, &pa);

        },
        [&]
        { // outline not working
            DT_STYLE_EFFECTS style1 = {1, {0}, {120, 50, 230, 0}, DV_NULL};
            DT_STYLE_EFFECTS style2 = {1, {0}, {7, 100, 250, 0}, DV_NULL};
            DT_STYLE_EFFECTS style3 = {1, {0}, {89, 167, 234, 0}, DV_NULL};
            DT_STYLE_EFFECTS style4 = {1, {0}, {20, 90, 200, 0}, DV_NULL};
            DT_TYPE_EFFECTS_L type1 = {dtFontFind(dtc->engine, "F1040_INVISB_NS0"), 0, 0, {{100, 100, 0, 0, 25}}, {0, 0, 0, 0, 0, DV_SCALE_100}};
	
	        dtOutputSetStyleEffects(dtc->engine,&style1,0);
            // clang-format on

            dtOutputSetStyleEffects(dtc->engine, &style1, 0);
            dtCharDoOutput(dtc->engine, L'轮', x += 100, y, 0, &pa);

            dtOutputSetStyleEffects(dtc->engine, &style2, 0);
            dtCharDoOutput(dtc->engine, L'廓', x += 100, y, 0, &pa);

            dtOutputSetStyleEffects(dtc->engine, &style3, 0);
            dtCharDoOutput(dtc->engine, L'设', x += 100, y, 0, &pa);

            dtOutputSetStyleEffects(dtc->engine, &style4, 0);
            dtCharDoOutput(dtc->engine, L'置', x += 100, y, 0, &pa);

        },
        [&]{
            //DT_STYLE_ATTRIBS;
            dtCharDoOutput(dtc->engine, L'M', x += 100, y, 0, &pa);
            DT_STYLE_EFFECTS style = {3, {DV_EFFECT_EDGE_100_NE, 100, 10}, {0, 255, 0, 0}, DV_NULL};
            dtOutputSetStyleEffects(dtc->engine, &style, 0);
            dtCharDoOutput(dtc->engine, L'M', x += 100, y, 0, &pa);
            dtCharDoOutput(dtc->engine, L'M', x += 100, y, 0, &pa);
        }
        };
    // dtCharDoOutput(dtc->engine, 'a', x+=100, y, 0, &pa);

    // dtOutputSetStyleAttribs(dtc->engine, new DT_STYLE_ATTRIBS {
    //     {0, 0},
    //     {"000000ff"}},
    //     0, DV_NULL
    // },0);
    // dtCharDoOutput(dtc->engine, 'a', x+=100, y, 0, &pa);

    // auto sz_dtFontGetCounter= dtFontGetCounter(dtc->engine);
    // auto fontIndex0=dtFontTest(dtc->engine,0);
    // auto fontIndexByName= dtFontFind(dtc->engine,"Moto");
    // auto fontIndexByNameInvalid= dtFontFind(dtc->engine,"Invalid");
    // constexpr int32_t chars[] = {'a', 'b', 'c', 'd', 0x5B57, 0x5E55, 0x6E32, 0x67D3, '1', '2', '3', '4'};
    // auto              retDrawChars = DrawChars(chars, sizeof(chars) / 4, 0, 100, 100, 100, dc_mem.m, dc_mem.w, dc_mem.h, dtc);
    // DrawChar('c',100,100,dc_mem.m, dc_mem.w, dc_mem.h, dtc);
    while (i != sizeof(funcs) / sizeof(funcs[0]))
    {
        funcs[i++]();
        x = *GetBaseDrawOffsetX() - 100;
        y += 100;
    }

    stbi_write_png("./d_typ_hellow_world_out.png", width, height, 3, dc_mem.m + (width * 3 * (height - 1)), -width * 3);

    /* Free memory surface */
    free(dc_mem.m);

    /* And destroy D-Type Engine */
    dtEngineExt(dtc->engine);
}

int main()
{
    // DefaultLogger()->info("something went wrong");
    DefaultLogger()->set_level(spdlog::level::level_enum::debug);
    draw(800, 800);
}