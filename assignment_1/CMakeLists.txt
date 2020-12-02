cmake_minimum_required(VERSION 3.0.0)
project(rpl_1 VERSION 0.1.0)

include(CTest)
enable_testing()

# include files
include_directories( ./include ./src )

# target
add_executable(rpl_1 ./src/main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
