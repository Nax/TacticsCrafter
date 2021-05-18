#include <TacticsCrafter/Core/ImageBuilder.h>

#define EBOOT_SIZE      (3835392)
#define BOOT_SIZE       (3835044)
#define EBOOT_OFFSET    (32 * 0x800)
#define BOOT_OFFSET     (130480 * 0x800)

namespace
{

void fileCopy(std::FILE* out, std::FILE* in)
{
    char buffer[16384];

    for (;;)
    {
        std::size_t len = std::fread(buffer, 1, sizeof(buffer), in);
            if (len == 0)
                break;
        std::fwrite(buffer, len, 1, out);
    }
}

void fileCopyRange(std::FILE* out, std::FILE* in, std::size_t outOffset, std::size_t inOffset, std::size_t len)
{
    char buffer[4096];

    std::fseek(out, (long)outOffset, SEEK_SET);
    std::fseek(in, (long)inOffset, SEEK_SET);

    while (len)
    {
        std::size_t l = len;
        if (l > sizeof(buffer))
            l = sizeof(buffer);

        l = std::fread(buffer, 1, l, in);
        std::fwrite(buffer, l, 1, out);
        len -= l;
    }
}

}

ImageBuilder::ImageBuilder()
: _fileIn{}
, _fileOut{}
{
}

ImageBuilder::~ImageBuilder()
{
    finish();
}

void ImageBuilder::setInput(const char* path)
{
    if (_fileIn)
        std::fclose(_fileIn);
    _fileIn = std::fopen(path, "rb");
}

void ImageBuilder::setOutput(const char* path)
{
    if (_fileOut)
        std::fclose(_fileOut);
    _fileOut = std::fopen(path, "wb");
}

void ImageBuilder::apply(const Changeset& changes)
{
    emit progress(0);

    if (!_fileIn || !_fileOut)
        return;

    /* Copy the whole ISO */
    fileCopy(_fileOut, _fileIn);
    emit progress(500);

    /* Decrypt the ISO */
    fileCopyRange(_fileOut, _fileIn, EBOOT_OFFSET, BOOT_OFFSET, BOOT_SIZE);
    emit progress(1000);
}

void ImageBuilder::finish()
{
    if (_fileIn)
        std::fclose(_fileIn);
    if (_fileOut)
        std::fclose(_fileOut);

    _fileIn = nullptr;
    _fileOut = nullptr;
}
