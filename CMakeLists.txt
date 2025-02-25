cmake_minimum_required(VERSION 3.23)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(cxx_standard_17)
include(target_folder)
include(vs_startup_project)

set(HIGHWAY_FRACTAL_PROJECT_NAME "Highway Fractal" CACHE "STRING" "Default project name for IDEs")
project("${HIGHWAY_FRACTAL_PROJECT_NAME}" CXX)

include(CTest)

add_subdirectory(libs)
add_subdirectory(tools)

vs_startup_project(fractal)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
