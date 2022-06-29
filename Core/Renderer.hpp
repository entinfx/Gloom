#ifndef Renderer_hpp
#define Renderer_hpp

#include <iostream>
#include "Vector3.hpp"
#include "Ray.hpp"
#include "Camera.hpp"
#include "Material.hpp"
#include "HitRecord.hpp"
#include "Renderer.hpp"
#include "../Hitable/Hitable.hpp"

class Renderer {
    Vector3 **buffer;
    int width;
    int height;
    int spp;
    int rayBounce;
    Vector3 color(const Ray &r, Hitable *scene, int depth) const;
public:
    Renderer(Vector3 **buffer, int width, int height, int spp, int rayBounce): buffer(buffer),
                                                                               width(width),
                                                                               height(height),
                                                                               spp(spp),
                                                                               rayBounce(rayBounce) {};
    void render(Hitable *scene, Camera *camera, void(^completion)(int currentSample)) const;
};

inline void Renderer::render(Hitable *scene, Camera *camera, void(^completion)(int currentSample)) const {
    for (int s = 0; s < this->spp; s++) {
        std::cout << "SPP: " << s + 1 << "/" << this->spp;
        Vector3 dofOffset = randomInUnitDisk();
        clock_t begin = clock();
        for (int j = this->height - 1; j >= 0; j--) {
            for (int i = 0; i < this->width; i++) {
                // Get ray for u, v
                float u = (float(i) + drand48()) / float(this->width);
                float v = (float(j) + drand48()) / float(this->height);
                Ray ray = camera->getRay(u, v, dofOffset);
                // Get color for ray, add to buffer
                this->buffer[i][j] = (s > 0) ? this->buffer[i][j] + color(ray, scene, 0) : color(ray, scene, 0);
            }
        }
        clock_t end = clock();
        double timePassed = double(end - begin) / CLOCKS_PER_SEC;
        std::cout << ", Time: " << timePassed << "s." << std::endl;

        /* Perform action on completion */
        completion(s + 1);
    }
}

inline Vector3 Renderer::color(const Ray &r, Hitable *scene, int depth) const {
    HitRecord hitRecord;
    // Get hit record of closest hit for ray
    if (scene->hit(r, 0.001, MAXFLOAT, hitRecord)) {
        Ray scattered;
        Vector3 attenuation;
        // Get light emittance
        Vector3 emitted = hitRecord.material->emitted();
        // Get material's scattered ray for current ray and hit record
        if (depth < this->rayBounce && hitRecord.material->scatter(r, hitRecord, attenuation, scattered)) {
            /* The Rendering Equation */
            // L0 = Le + ∫(f * Li * cos(Ø) * dw), where:
            // L0(x,w0) - pixel color at hit point x, ray 0 direction w0
            // Le(x,w0) - emitted radiance at hit point x, ray 0 direction w0
            // f(x,wi->w0) - BRDF at hit point x (attenuation)
            // Li(x,wi) - radiance at hit point x, ray i direction wi

            // Shoot scattered rays recursively until a light is hit
            return emitted + attenuation * color(scattered, scene, depth + 1);
        } else {
            // End of recursion: light was hit, return emitted radiance
            return emitted;
        }
    } else {
        // End of recursion: ray didn't hit anything - return BG color
        return Vector3(0.0, 0.0, 0.0);
    }
}


#endif
