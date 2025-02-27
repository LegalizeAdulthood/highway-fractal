#include "lib/lib.h"

#include <hwy/highway.h>

#include <vector>

using namespace hwy::HWY_NAMESPACE;

void compute_mandelbrot_set(int width, int height, int max_iterations, std::vector<int>& output)
{
    output.resize(width * height);
    const float x_min = -2.0f;
    const float x_max = 1.0f;
    const float y_min = -1.5f;
    const float y_max = 1.5f;
    const float x_scale = (x_max - x_min) / width;
    const float y_scale = (y_max - y_min) / height;

    const HWY_FULL(float) d; // Define the SIMD type
    const HWY_FULL(int32_t) di; // Define the SIMD type for integers

    for (int y = 0; y < height; ++y)
    {
        const float cy = y_min + y * y_scale;
        const auto cy_vec = Set(d, cy);
        for (int x = 0; x < width; x += Lanes(d))
        {
            // Initialize SIMD vectors for the x coordinates
            auto cx_vec = Zero(d);
            for (size_t lane = 0; lane < Lanes(d); ++lane)
            {
                cx_vec = InsertLane(cx_vec, lane, x_min + (x + lane) * x_scale);
            }
            auto zx_vec = cx_vec;
            auto zy_vec = cy_vec;
            const auto one{Set(di, 1)};
            auto iter_vec = one;

            for (int i = 0; i < max_iterations; ++i)
            {
                const auto zx2 = zx_vec * zx_vec;
                const auto zy2 = zy_vec * zy_vec;
                const auto magnitude2 = zx2 + zy2;

                const auto mask = magnitude2 < Set(d, 4.0f);
                if (AllFalse(d, mask))
                {
                    break;
                }

                iter_vec += IfThenElseZero(RebindMask(di, mask), one);
                const auto new_zy_vec = Set(d, 2.0f) * zx_vec * zy_vec + cy_vec;
                zx_vec = zx2 - zy2 + cx_vec;
                zy_vec = new_zy_vec;
            }

            // Store results
            Store(iter_vec, di, output.data() + y * width + x);
        }
    }
}
