#include <lib/lib.h>

#include <OpenImageIO/imageio.h>

#include <vector>
#include <iostream>

OIIO_NAMESPACE_USING

int main()
{
    const int width = 800;
    const int height = 600;
    const int max_iterations = 1000;
    std::vector<int> output;

    // Compute the Mandelbrot set
    compute_mandelbrot_set(width, height, max_iterations, output);

    // Convert the output to an image buffer
    std::vector<unsigned char> image(width * height * 3);
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            int index = y * width + x;
            int value = output[index];
            unsigned char color = static_cast<unsigned char>(255 * value / max_iterations);
            image[3 * index + 0] = color; // Red
            image[3 * index + 1] = color; // Green
            image[3 * index + 2] = color; // Blue
        }
    }

    // Write the image to a PNG file
    const char* filename = "mandelbrot.png";
    std::unique_ptr<ImageOutput> out = ImageOutput::create(filename);
    if (!out)
    {
        std::cerr << "Could not create output image for " << filename << "\n";
        return 1;
    }

    ImageSpec spec(width, height, 3, TypeDesc::UINT8);
    out->open(filename, spec);
    out->write_image(TypeDesc::UINT8, image.data());
    out->close();

    std::cout << "Mandelbrot set image saved to " << filename << "\n";

    return 0;
}
