#ifndef TC_IMAGE_BUILDER_H
#define TC_IMAGE_BUILDER_H

#include <cstdio>

class Changeset;
class ImageBuilder
{
public:
    ImageBuilder();

    void setInput(const char* path);
    void setOutput(const char* path);
    void apply(const Changeset& changes);
    void finish();

private:
    std::FILE*  _fileIn;
    std::FILE*  _fileOut;
};

#endif /* TC_IMAGE_BUILDER_H */
