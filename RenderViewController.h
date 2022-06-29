#import <Cocoa/Cocoa.h>
#import "RenderView.h"
#import "Core/Vector3.hpp"
#import "Core/Renderer.hpp"

NS_ASSUME_NONNULL_BEGIN

@interface RenderViewController : NSViewController

@property (strong) IBOutlet RenderView *_Nullable renderView;
@property Renderer *_Nullable renderer;
@property Vector3 *_Nullable *_Nullable buffer;

- (void)updateViewForSample:(int)currentSample;

@end

NS_ASSUME_NONNULL_END
