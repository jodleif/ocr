//
// Created by jo on 4/15/18.
//
#include "ocr_lib.h"

ocr::ImageOcr::ImageOcr()
{
    tess.Init(nullptr, "eng", tesseract::OEM_DEFAULT);
}

std::optional<std::string> ocr::ImageOcr::read_image(const char *path)
{
    if(__builtin_expect(!path, 1)) {
        return {};
    }
    auto destroyPix = [](Pix* p){ pixDestroy(&p);};
    auto image = std::unique_ptr<Pix, decltype(destroyPix)>(pixRead(path), destroyPix);
    if(!image) {
        return {};
    }
    tess.SetImage(image.get());
    tess.Recognize(nullptr);

    auto text = std::unique_ptr<char>(tess.GetUTF8Text());
    auto res = std::string(text.get(),strlen(text.get()));

    tess.Clear();

    return std::make_optional(res);
}
