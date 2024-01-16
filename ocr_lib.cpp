//
// Created by jo on 4/15/18.
//
#include "ocr_lib.h"

#include <cassert>
#include <memory>
#include <leptonica/allheaders.h>
#include <tesseract/baseapi.h>

ocr::ImageOcr::ImageOcr() : ImageOcr("eng")
{
}

ocr::ImageOcr::~ImageOcr()
{
    tess->End();
}


const tesseract::PageSegMode mode = tesseract::PSM_AUTO_OSD;

ocr::ImageOcr::ImageOcr(const std::string_view lang) : tess(std::make_unique<tesseract::TessBaseAPI>())
{
    tess->Init(nullptr, lang.data(), tesseract::OEM_DEFAULT);
    tess->SetPageSegMode(mode);
}

auto destroyPix = [](auto* p) { pixDestroy(&p); };

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
    assert(tess->GetPageSegMode() == mode);
    tess->SetImage(image.get());
    tess->Recognize(nullptr);

    const auto text = std::unique_ptr<char>(tess->GetUTF8Text());
    auto res = std::string(text.get(), std::strlen(text.get()));

    tess->Clear();

    return std::make_optional(res);
}

std::unique_ptr<PIX, decltype(destroyPix)> make_pix_from_bmp(const std::vector<std::uint8_t>& image)
{
    const l_uint8* data = const_cast<l_uint8*>(image.data());
    auto* pixy = pixReadMemBmp(data, image.size());
    return {pixy, destroyPix};
}

std::optional<std::string> ocr::ImageOcr::extract_text_from_bmp(const std::vector<std::uint8_t>& bmp_data)
{
    auto image = make_pix_from_bmp(bmp_data);
    if (!image)
    {
        return {};
    }

    assert(tess->GetPageSegMode() == mode);
    tess->SetImage(image.get());
    tess->Recognize(nullptr);

    const auto text = std::unique_ptr<char>(tess->GetUTF8Text());
    auto res = std::string(text.get(), std::strlen(text.get()));

    tess->Clear();

    return std::make_optional(res);
}
