cmake_minimum_required(VERSION 3.20)
project(ocr_lib)

set(CMAKE_CXX_STANDARD 23)

#set(CMAKE_CXX_FLAGS "-std=c++23")
find_package( PkgConfig REQUIRED)

pkg_search_module( TESSERACT REQUIRED tesseract )
pkg_search_module( LEPTONICA REQUIRED lept )

include_directories( ${TESSERACT_INCLUDE_DIRS} )

#Unsure if this is MacOS weirdness:
cmake_path(GET LEPTONICA_INCLUDE_DIRS PARENT_PATH LEPTONICA_INCLUDE_PARENT_DIR)
#message("Leptonica libraries: ${LEPTONICA_INCLUDE_DIRS}")
#message("Leptonica libraries (parent?): ${LEPTONICA_INCLUDE_PARENT_DIR}")
include_directories( ${LEPTONICA_INCLUDE_PARENT_DIR} )

link_directories( ${TESSERACT_LIBRARY_DIRS} )
link_directories( ${LEPTONICA_LIBRARY_DIRS} )

set(HEADERS "ocr_lib.h")
set(SRC "ocr_lib.cpp")
add_library(ocr_lib STATIC ${HEADERS} ${SRC})
add_executable(ocr "main.cpp")
target_link_libraries(ocr_lib ${TESSERACT_LIBRARIES} ${LEPTONICA_LIBRARIES})
target_link_libraries(ocr ocr_lib)