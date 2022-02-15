#import <string>
#import <Foundation/Foundation.h>

std::string ltcImplGetDataPath(void)
{
    NSString* path = [[[NSBundle mainBundle] resourceURL] path];
    std::string str = [path UTF8String];

    return str + "/data";
}
