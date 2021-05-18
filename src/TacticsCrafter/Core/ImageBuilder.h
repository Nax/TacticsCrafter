#ifndef TC_IMAGE_BUILDER_H
#define TC_IMAGE_BUILDER_H

#include <cstdio>
#include <QObject>

class Changeset;
class ImageBuilder : public QObject
{
    Q_OBJECT

public:
    ImageBuilder();
    ~ImageBuilder();

    void setInput(const char* path);
    void setOutput(const char* path);
    void apply(const Changeset& changes);
    void finish();

signals:
    void progress(int);

private:
    std::FILE*      _fileIn;
    std::FILE*      _fileOut;
};

#endif /* TC_IMAGE_BUILDER_H */
