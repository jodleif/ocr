#pragma once
#include <optional>
#include <filesystem>
#include <string>
#include <tesseract/baseapi.h>

namespace ocr {
    class ImageOcr {
        tesseract::TessBaseAPI tess;
    public:
        ImageOcr();
        explicit ImageOcr(const std::string_view lang);
        std::optional<std::string> extract_text_from_image(const std::filesystem::path& path);
    };
}
