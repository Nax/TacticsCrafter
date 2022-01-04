#include <stdio.h>
#include <libtactics/tactics.h>

static void usage(const char* prog)
{
    printf("usage: %s <out.iso> <in.iso> <project.tcproj>\n", prog);
}

static int run(const char* pathOut, const char* pathIn, const char* pathProj)
{
    LTC_Context* ctx;

    ctx = ltcCreateContext(pathProj);
    ltcRunPipeline(ctx);
    ltcBuildImage(ctx, pathOut, pathIn, NULL, NULL);
    ltcDestroyContext(ctx);

    return 0;
}

int main(int argc, char** argv)
{
    if (argc != 4)
    {
        usage(argv[0]);
        return 1;
    }

    return run(argv[1], argv[2], argv[3]);
}
