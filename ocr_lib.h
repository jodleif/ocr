#pragma once
#include <optional>
#include <filesystem>
#include <string>
#include <memory>

namespace tesseract
{
    class TessBaseAPI;
}

namespace ocr
{
    class ImageOcr
    {
        std::unique_ptr<tesseract::TessBaseAPI> tess;

    public:
        ImageOcr();

        ~ImageOcr();
        explicit ImageOcr(const std::string_view lang);
        std::optional<std::string> extract_text_from_image(const std::filesystem::path& path);
        std::optional<std::string> extract_text_from_bmp(const std::vector<std::uint8_t> & bmp_data);
    };
}
