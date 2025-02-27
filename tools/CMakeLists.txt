find_package(OpenImageIO CONFIG REQUIRED)

target_compile_options(OpenImageIO::OpenImageIO INTERFACE $<$<CXX_COMPILER_ID:MSVC>:/utf-8>)

add_executable(fractal main.cpp)
target_link_libraries(fractal PUBLIC lib)
target_folder(fractal "Tools")
target_link_libraries(fractal PRIVATE OpenImageIO::OpenImageIO)
