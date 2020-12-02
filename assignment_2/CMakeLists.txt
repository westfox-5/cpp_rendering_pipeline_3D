cmake_minimum_required(VERSION 3.0.0)
project(rasterization_2 VERSION 0.1.0)

include_directories( ./src )


add_executable(rasterization_2 ./src/rasterization.cpp ./src/texture.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
