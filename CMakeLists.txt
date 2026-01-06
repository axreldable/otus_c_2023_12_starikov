cmake_minimum_required(VERSION 3.27)
project(otus_c_2023_12_starikov C)

set(CMAKE_C_STANDARD 11)

add_executable(test main.c
        "hw01_rarjpeg/rarjpeg.c"
        "hw01_rarjpeg/rarjpeg.h"
)
