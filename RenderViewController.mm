#import "RenderViewController.h"
#import <iostream>
#import "Core/Vector3.hpp"
#import "Core/Camera.hpp"
#import "Core/Material.hpp"
#import "Core/Renderer.hpp"
#import "Hitable/Hitable.hpp"
#import "Hitable/HitableList.hpp"
#import "RayIntersection/Sphere.hpp"
#import "Materials/Metal.hpp"
#import "Materials/Lambertian.hpp"
#import "Materials/Glossy.hpp"
#import "Materials/DiffuseLight.hpp"
#import "Materials/Dielectric.hpp"

@implementation RenderViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    /* Image parameters */
    int width = 960;
    int height = 400;
    int spp = 800;
    int rayBounce = 50;
    
    /* Window parameters */
    [self setPreferredContentSize:NSMakeSize(width, height)];
    [_renderView setFrameSize:NSMakeSize(width, height)];
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        /* Scene */
//        Hitable *list[13];
//        list[0] = new Sphere(Vector3(0, -1000, -1), 1000, new Lambertian(Vector3(0.15, 0.26, 0.6))); // floor
//        list[1] = new Sphere(Vector3(0, 0, -10000 - 2.25), 10000, new Lambertian(Vector3(0.15, 0.26, 0.6))); // back wall
//        list[2] = new Sphere(Vector3(-10000 - 2.5, 0, -1), 10000, new Lambertian(Vector3(0.15, 0.26, 0.6))); // left wall
//        list[3] = new Sphere(Vector3(0, 0 + 0.5, -1), 0.5, new Glossy(Vector3(1, 0.2, 0.4))); // middle Sphere
//        list[4] = new Sphere(Vector3(1.2, 0 + 0.28, -0.7), 0.28, new DiffuseLight(Vector3(0, 5.1, 2.7))); // right light
//        list[5] = new Sphere(Vector3(1.2, 0 + 0.3, -0.7), 0.3, new Dielectric(Vector3(0.96, 0.96, 0.98), 1.5)); // right glass
//        list[6] = new Sphere(Vector3(-1.2, 0 + 0.5, -0.7), 0.5, new DiffuseLight(Vector3(22, 22, 22))); // left light
//        list[7] = new Sphere(Vector3(2.0, 0 + 0.3, -0.7), 0.3, new Dielectric(Vector3(0.96, 0.96, 0.98), 1.5)); // right glass
//        list[8] = new Sphere(Vector3(2.0, 0 + 0.28, -0.7), 0.28, new DiffuseLight(Vector3(5.1, 0.0, 2.7))); // right light
//        list[9] = new Sphere(Vector3(0.45, 0 + 0.3, -0.2), 0.3, new Dielectric(Vector3(0.96, 0.96, 0.98), 1.5));
//        list[10] = new Sphere(Vector3(-0.45, 0 + 0.25, -0.25), 0.25, new Glossy(Vector3(0.2, 1.0, 0.55)));
//        list[11] = new Sphere(Vector3(0, 5 + 30, 1), 30, new DiffuseLight(Vector3(1.8, 1.3, 2.2))); // top light
//        list[12] = new Sphere(Vector3(-1.0, 0 + 0.35, 0.5), 0.35, new Glossy(Vector3(1.0, 0.2, 0.55))); // middle Sphere
//        Hitable *scene = new HitableList(list, 13);
        
        Hitable *list[11];
        list[0] = new Sphere(Vector3(0, -1000, -1), 1000, new Lambertian(Vector3(0.15, 0.26, 0.6))); // floor
        list[1] = new Sphere(Vector3(0, 0, -10000 - 2.25), 10000, new Lambertian(Vector3(0.15, 0.26, 0.6))); // back wall
        list[2] = new Sphere(Vector3(-10000 - 2.5, 0, -1), 10000, new Lambertian(Vector3(0.15, 0.26, 0.6))); // left wall
        list[3] = new Sphere(Vector3(-1.2, 0 + 0.45, -0.7), 0.45, new Glossy(Vector3(0.7, 0.1, 0.25))); // left Glossy
        list[4] = new Sphere(Vector3(0, 0 + 0.5, -1), 0.5, new Glossy(Vector3(1, 0.2, 0.4))); // middle Glossy
        list[5] = new Sphere(Vector3(1.2, 0 + 0.3, -0.7), 0.3, new Metal(Vector3(0.75, 0.75, 0.75), 0.0)); // right Metal
        list[6] = new Sphere(Vector3(2.0, 0 + 0.3, -0.7), 0.3, new Glossy(Vector3(0.25, 0.45, 0.65))); // right Glossy
        list[7] = new Sphere(Vector3(0.45, 0 + 0.3, -0.2), 0.3, new Dielectric(Vector3(0.96, 0.96, 0.98), 1.5)); // front right glass
        list[8] = new Sphere(Vector3(-0.45, 0 + 0.25, -0.25), 0.25, new Glossy(Vector3(0.2, 1.0, 0.55))); // front left Glossy
        list[9] = new Sphere(Vector3(30, 20, 0), 30, new DiffuseLight(Vector3(2.2, 2.0, 3.3))); // right light
        list[10] = new Sphere(Vector3(-1.0, 0 + 0.35, 0.5), 0.35, new Glossy(Vector3(1.0, 0.2, 0.55))); // front left Glossy
        Hitable *scene = new HitableList(list, 11);
        
        /* Camera */
        Vector3 lookFrom(0, 1.5, 3);
        Vector3 lookAt(0, 0.5, -1);
        Vector3 vUp(0, 1, 0);
        float vFov = 40;
        float distanceToFocus = (lookFrom - lookAt).length();
        float aperture = 0.25;
        Camera *camera = new Camera(lookFrom, lookAt, vUp, vFov, float(width) / float(height), aperture, distanceToFocus);
        
        /* Set up buffers for Renderer and RenderView */
        self->_buffer = new Vector3*[width];
        for (int i = 0; i < width; i++) self->_buffer[i] = new Vector3[height];
        self->_renderView.buffer = (unsigned int *)malloc(width * height * sizeof(unsigned int));
        
        /* Renderer */
        self->_renderer = new Renderer(self->_buffer, width, height, spp, rayBounce);
        
        /* Render to buffer and update view continuously */
        self->_renderer->render(scene, camera, ^(int currentSample) {
            dispatch_async(dispatch_get_main_queue(), ^{
                [self updateViewForSample:currentSample];
            });
        });
    });
}

- (void)updateViewForSample:(int)currentSample {
    int width = (int)_renderView.frame.size.width;
    int height = (int)_renderView.frame.size.height;
    for (int j = height - 1; j >= 0; j--) {
        for (int i = 0; i < width; i++) {
            /* Average buffer */
            Vector3 color = _buffer[i][j] / currentSample;
            /* Clipping */
            if (color.r() > 1) color = Vector3(1, color.g(), color.b());
            if (color.g() > 1) color = Vector3(color.r(), 1, color.b());
            if (color.b() > 1) color = Vector3(color.r(), color.g(), 1);
            /* Gamma correction */
            // Human eye color perception is non-linear, correction
            // must be applied: encodedColor = linearColor^(gamma), where
            // gamma = 1/2.2 for human eye (use gamma = 2.2 to decode back into linear).
            // Lower gamma means brighter image (color rises faster).
            color = Vector3(sqrt(color.r()), sqrt(color.g()), sqrt(color.b()));
            // Convert to 8 bit per channel
            int r = int(255.99 * color.r());
            int g = int(255.99 * color.g());
            int b = int(255.99 * color.b());
            // Render
            _renderView.buffer[i + width * (height - j - 1)] = r << 16 | g << 8 | b;
        }
    }
    [_renderView setNeedsDisplay:YES];
}

@end
