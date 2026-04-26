cmake_minimum_required(VERSION 3.20)
project(trynd)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)
file(GLOB SRC src/*.cpp)

add_executable(trynd ${SRC})
