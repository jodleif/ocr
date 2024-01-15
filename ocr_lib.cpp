//
// Created by jo on 4/15/18.
//
#include "ocr_lib.h"
#include <memory>
#include <leptonica/allheaders.h>


ocr::ImageOcr::ImageOcr() : ImageOcr("eng")
{
};

ocr::ImageOcr::ImageOcr(const std::string_view lang)
{
    tess.Init(nullptr, lang.data(), tesseract::OEM_DEFAULT);
}

auto destroyPix = [](Pix* p) { pixDestroy(&p); };

auto open_image = [](const std::filesystem::path& path) -> std::unique_ptr<Pix, decltype(destroyPix)>
{
    if (path.empty())
    {
        return nullptr;
    }
    return {pixRead(path.c_str()), destroyPix};
};

std::optional<std::string> ocr::ImageOcr::extract_text_from_image(const std::filesystem::path& path)
{
    const auto image = open_image(path);
    if (!image)
    {
        return {};
    }
    tess.SetImage(image.get());
    tess.Recognize(nullptr);

    const auto text = std::unique_ptr<char>(tess.GetUTF8Text());
    auto res = std::string(text.get(), std::strlen(text.get()));

    tess.Clear();

    return std::make_optional(res);
}
