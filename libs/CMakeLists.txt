find_package(hwy CONFIG REQUIRED)

add_library(lib
    include/lib/lib.h
    lib.cpp
    mandelbrot_dynamic.h
    mandelbrot_dynamic.cpp
    mandelbrot_static.h
    mandelbrot_static.cpp
)
target_include_directories(lib PUBLIC include)
# This is needed so that the highway foreach_target.h mechanism works on ubuntu.
target_include_directories(lib PRIVATE .)
target_folder(lib "Libraries")
target_link_libraries(lib PUBLIC hwy::hwy)
