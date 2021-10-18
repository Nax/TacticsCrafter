#include <cstdint>
#include <libtactics/ImageBuilder.h>
#include <libtactics/Context.h>

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

void fileZeroRange(std::FILE* out, std::size_t offset, std::size_t len)
{
    char buffer[4096]{};

    std::fseek(out, (long)offset, SEEK_SET);

    while (len)
    {
        std::size_t l = len;
        if (l > sizeof(buffer))
            l = sizeof(buffer);

        std::fwrite(buffer, l, 1, out);
        len -= l;
    }
}

void write32(std::FILE* out, std::uint32_t offset, std::uint32_t value)
{
    std::fseek(out, (long)offset, SEEK_SET);
    std::fwrite(&value, 4, 1, out);
}

std::uint32_t swap32(std::uint32_t v)
{
    return ((v & 0xff) << 24)
        | ((v & 0xff00) << 8)
        | ((v & 0xff0000) >> 8)
        | ((v & 0xff000000) >> 24);
}

std::uint32_t fileOffsetFromAddr(std::uint32_t addr)
{
    if (addr >= 0x08804000 && addr < 0x08804000 + 3833648)
    {
        return EBOOT_OFFSET + 0x54 + (addr - 0x08804000);
    }
    if (addr >= 0x08804000 + 0xfe5200 && addr < 0x0a000000)
    {
        return 0xbc8000 + addr - (0x08804000 + 0xfe5200);
    }
    return 0;
}

void fileWriteChange(std::FILE* out, const Changeset::Change& change)
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
    case Changeset::ChangeType::Blob:
        std::fwrite(change.blob, change.blobSize, 1, out);
        break;
    }
}

void dummyCallback(void* arg, int progress)
{
    (void)arg;
    (void)progress;
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

void ImageBuilder::apply(LTC_Context* ctx, LTC_ProgressCallback cb, void* arg)
{
    if (!cb)
    {
        cb = &dummyCallback;
    }
    cb(arg, 0);

    if (!_fileIn || !_fileOut)
        return;

    /* Copy the whole ISO */
    fileCopy(_fileOut, _fileIn);
    cb(arg, 500);

    /* Decrypt the ISO */
    fileCopyRange(_fileOut, _fileIn, EBOOT_OFFSET, BOOT_OFFSET, BOOT_SIZE);
    fileZeroRange(_fileOut, EBOOT_OFFSET + BOOT_SIZE, EBOOT_SIZE - BOOT_SIZE);

    if (ctx->extraMemory)
    {
        /* Patch in the extra size */
        write32(_fileOut, EBOOT_OFFSET + 0x48, 0xfe5200 + ctx->extraMemory);
        write32(_fileOut, fileOffsetFromAddr(0x08a73560), ctx->extraMemory);

        /* Parasite the update file */
        write32(_fileOut, 0xd06a, ctx->extraMemory);
        write32(_fileOut, 0xd06a + 4, swap32(ctx->extraMemory));

        fileZeroRange(_fileOut, 0xbc8000, ctx->extraMemory);
    }

    cb(arg, 750);

    /* Apply the changes */
    for (const auto& c : ctx->changeset)
        fileWriteChange(_fileOut, c);

    cb(arg, 1000);
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

LTC_API void ltcBuildImage(LTC_Context* ctx, const char* dst, const char* src, LTC_ProgressCallback cb, void* arg)
{
    ImageBuilder builder;

    builder.setOutput(dst);
    builder.setInput(src);
    builder.apply(ctx, cb, arg);
}
