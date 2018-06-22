#include <iostream>
#include <filesystem>
#include "ocr_lib.h"

namespace {
void print_results(std::optional<std::string> str)
{
    if (str) {
        std::cout << *str << '\n';
    } else {
        std::cout << "Could not load image\n";
    }
}
}

int main(int argc, const char **argv)
{
    if (argc < 2) return 1;
    {
        std::error_code ec;
        auto path = std::filesystem::path(argv[1]);
        auto exists = std::filesystem::exists(path, ec);
        if (!exists && !ec) {
            std::cout << "File: " << path << " does not exist\n";
            return 1;
        }
    }
    ocr::ImageOcr img;

    auto str = img.read_image(argv[1]);
    print_results(str);
    return 0;
}