cmake_minimum_required(VERSION 3.9)
project(ocr_lib)

set(CMAKE_CXX_STANDARD 17)

set(HEADERS "ocr_lib.h")
set(SRC "ocr_lib.cpp")
add_library(ocr_lib ${HEADERS} ${SRC})
add_executable(ocr "main.cpp")

target_link_libraries(ocr_lib tesseract lept)
target_link_libraries(ocr ocr_lib)