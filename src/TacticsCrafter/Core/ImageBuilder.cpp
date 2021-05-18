#include <TacticsCrafter/Core/ImageBuilder.h>
#include <TacticsCrafter/Core/Changeset.h>

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

std::uint32_t fileOffsetFromAddr(std::uint32_t addr)
{
    if (addr >= 0x08804000 && addr < 0x08804000 + 3833648)
    {
        return EBOOT_OFFSET + 0x54 + (addr - 0x08804000);
    }
    return 0;
}

void fileWriteChange(std::FILE* out, Changeset::Change change)
{
    std::uint32_t addr;

    addr = fileOffsetFromAddr(change.addr);
    if (!addr)
        return;
    std::fseek(out, addr, SEEK_SET);
    switch (change.type)
    {
    case Changeset::ChangeType::None:
        break;
    case Changeset::ChangeType::Write8:
        std::fwrite(&change.u8, 1, 1, out);
        break;
    case Changeset::ChangeType::Write16:
        std::fwrite(&change.u16, 2, 1, out);
        break;
    case Changeset::ChangeType::Write32:
        std::fwrite(&change.u32, 4, 1, out);
        break;
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
    emit progress(750);

    /* Apply the changes */
    for (const auto& c : changes)
        fileWriteChange(_fileOut, c);

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
