#include "mandelbrot_dynamic.h"

#ifndef HWY_TARGET_INCLUDE
#undef HWY_TARGET_INCLUDE
#endif
#define HWY_TARGET_INCLUDE "mandelbrot_dynamic.cpp"
#include <hwy/foreach_target.h>
#include <hwy/highway.h>

#include <iostream>
#include <vector>

namespace fractal
{
namespace HWY_NAMESPACE
{

static void mandelbrot_dynamic_impl(int width, int height, int max_iterations, std::vector<int> &output)
{
    std::cout << "Compiled for " << hwy::TargetName(HWY_TARGET) << "\n";
    output.resize(width * height);
    const float x_min = -2.0f;
    const float x_max = 1.0f;
    const float y_min = -1.5f;
    const float y_max = 1.5f;
    const float x_scale = (x_max - x_min) / width;
    const float y_scale = (y_max - y_min) / height;

    const HWY_FULL(float) d;
    const HWY_FULL(int32_t) di;
    using VecZ = decltype(Zero(d));
    using VecI = decltype(Zero(di));
    using VecMask = decltype(Zero(d) < Zero(d));
    const VecI one{Set(di, 1)};

    for (int y = 0; y < height; ++y)
    {
        const float cy = y_min + y * y_scale;
        const VecZ cy_vec = Set(d, cy);
        for (int x = 0; x < width; x += Lanes(d))
        {
            // Initialize SIMD vectors for the x coordinates
            VecZ cx_vec = MulAdd(Iota(d, x), Set(d, x_scale), Set(d, x_min));
            VecZ zx_vec = cx_vec;
            VecZ zy_vec = cy_vec;
            VecI iter_vec = one;

            for (int i = 0; i < max_iterations; ++i)
            {
                const VecZ zx2 = zx_vec * zx_vec;
                const VecZ zy2 = zy_vec * zy_vec;
                const VecZ magnitude2 = zx2 + zy2;

                const VecMask mask = magnitude2 < Set(d, 4.0f);
                if (AllFalse(d, mask))
                {
                    break;
                }

                iter_vec += IfThenElseZero(RebindMask(di, mask), one);
                const VecZ new_zy_vec = Set(d, 2.0f) * zx_vec * zy_vec + cy_vec;
                zx_vec = zx2 - zy2 + cx_vec;
                zy_vec = new_zy_vec;
            }

            // Store results
            Store(iter_vec, di, output.data() + y * width + x);
        }
    }
}

} // namespace HWY_NAMESPACE
} // namespace fractal

#if HWY_ONCE

namespace fractal
{

HWY_EXPORT(mandelbrot_dynamic_impl);

void dispatch(int width, int height, int max_iterations, std::vector<int> &output)
{
    return HWY_DYNAMIC_DISPATCH(mandelbrot_dynamic_impl)(width, height, max_iterations, output);
}

} // namespace fractal

void mandelbrot_dynamic(int width, int height, int max_iterations, std::vector<int> &output)
{
    return fractal::dispatch(width, height, max_iterations, output);
}

#endif
