#pragma once
#include <Helper/project/symbol.h>
#include <Helper/log/log.h>
#include <memory>
#include <stdint.h>
#include <exception>
Helper_API const unsigned char *get_file_dtype_inf_();
Helper_API const unsigned int   get_file_dtype_inf_size_();

Helper_API_CLS struct Image
{
    uint8_t *mem;
    int      cx, cy;
    // may only support n-bytes
    int          bitCountPerPixel;
    static Image CreateImage(int cx, int cy, int bitCountPerPixel, uint8_t *mem = 0)
    {
        Image ret{mem, cx, cy, bitCountPerPixel};
        if (ret.mem == 0)
        {
            auto pitch = ((cy * bitCountPerPixel + 31) & ~31) >> 3;
            ret.mem = new uint8_t(cx * pitch);
        }
        return ret;
    }
    static void Release(Image *i)
    {
        delete i->mem;
    }
};

Helper_API_CLS struct ImageInnerRect
{
    Image                *image; // not hold this resource
    int                   x, y;
    int                   w, h;
    static ImageInnerRect CreateImageInsideRect(int x, int y, int w, int h, Image *image)
    {
        /* check input params */
        if (image == 0 || image->cx <= x + w || image->cy <= y + h)
        {
            DefaultLogger()->error("CreateImageInsideRect use invalid params ");
            return ImageInnerRect();
        }
        ImageInnerRect ret{image, x, y, w, h};
        return ret;
    }
    static ImageInnerRect CreateImageInnerRect(Image *image)
    {
        return ImageInnerRect{image, 0, 0, image->cx, image->cy};
    }
};

struct DT_Context;
Helper_API std::shared_ptr<DT_Context> CreateDT_Context();
/* render a character in the rect, return 0 if success */
Helper_API int DrawChar(int32_t c, int x, int y, uint8_t *rgb_mem, int mem_w, int mem_h, std::shared_ptr<DT_Context> dtc);

Helper_API int DrawChars(const int32_t *cs, int chars_count, int start_x, int start_y, int font_w, int font_h, uint8_t *rgb_mem, int mem_w,
    int mem_h, std::shared_ptr<DT_Context> dtc);