#include "lib/lib.h"

#include "mandelbrot_dynamic.h"
#include "mandelbrot_static.h"

void mandelbrot(int width, int height, int max_iterations, std::vector<int> &output)
{
    static bool use_static{true};
    return use_static ? mandelbrot_static(width, height, max_iterations, output)
                      : mandelbrot_dynamic(width, height, max_iterations, output);
}
