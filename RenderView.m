#import "RenderView.h"

@implementation RenderView

- (void)drawRect:(NSRect)dirtyRect {
    [super drawRect:dirtyRect];
    /* Image parameters */
    int width = (int)self.frame.size.width;
    int height = (int)self.frame.size.height;
    int bitsPerComponent = 8;
    int bitsPerPixel = 32;
    int bytesPerRow = 4 * width;
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    CGBitmapInfo bitmapInfo = kCGImageAlphaNoneSkipFirst | kCGBitmapByteOrder32Little;
    CGDataProviderRef dataProvider = CGDataProviderCreateWithData(nil, _buffer, width * height, nil);
    void *decodeArray = nil;
    bool shouldInterpolate = false;
    CGColorRenderingIntent renderingIntent = kCGRenderingIntentDefault;
    CGImageRef image = CGImageCreate(width,
                                     height,
                                     bitsPerComponent,
                                     bitsPerPixel,
                                     bytesPerRow,
                                     colorSpace,
                                     bitmapInfo,
                                     dataProvider,
                                     decodeArray,
                                     shouldInterpolate,
                                     renderingIntent);
    CGContextRef context = [[NSGraphicsContext currentContext] graphicsPort];
    /* Draw image */
    CGContextDrawImage(context, CGRectMake(0, 0, width, height), image);
    /* Release */
    CGColorSpaceRelease(colorSpace);
    CGDataProviderRelease(dataProvider);
    CGImageRelease(image);
}

@end
