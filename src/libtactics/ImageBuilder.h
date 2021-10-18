#ifndef TC_IMAGE_BUILDER_H
#define TC_IMAGE_BUILDER_H

#include <cstdio>
#include <libtactics/tactics.h>

class ImageBuilder
{
public:
    ImageBuilder();
    ~ImageBuilder();

    void setInput(const char* path);
    void setOutput(const char* path);
    void apply(LTC_Context* ctx, LTC_ProgressCallback cb, void* arg);
    void finish();

private:
    std::FILE*      _fileIn;
    std::FILE*      _fileOut;
};

#endif /* TC_IMAGE_BUILDER_H */
