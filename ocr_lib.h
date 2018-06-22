
#include <memory>
#include <optional>
#include <string>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

namespace ocr {
    class ImageOcr {
        tesseract::TessBaseAPI tess;
    public:
        ImageOcr();
        std::optional<std::string> read_image(const char *path);
    };
}
