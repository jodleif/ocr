#include <iostream>
#include "ocr_lib.h"

namespace {
void print_results(std::optional<std::string> str) {
    if(str) {
        std::cout << *str << '\n';
    } else {
        std::cout << "Could not load image\n";
    }
}
}

int main()
{
    ocr::ImageOcr img;

    auto str = img.read_image("test.png");
    print_results(str);
    auto str2 = img.read_image("test2.png");
    print_results(str2);
    return 0;
}