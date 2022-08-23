#include <iostream>
#include <fstream>

#include "Core/Vector3.hpp"
#include "Core/Camera.hpp"
#include "Core/Material.hpp"
#include "Hitable/Hitable.hpp"
#include "Hitable/HitableList.hpp"
#include "RayIntersection/Sphere.hpp"
#include "Materials/Metal.hpp"
#include "Materials/Lambertian.hpp"
#include "Materials/Glossy.hpp"
#include "Materials/DiffuseLight.hpp"
#include "Materials/Dielectric.hpp"

Vector3 color(const Ray &r, Hitable *scene, int depth, int maxDepth) {
    HitRecord hitRecord;
    // Get hit record of closest hit for ray
    if (scene->hit(r, 0.001, MAXFLOAT, hitRecord)) {
        Ray scattered;
        Vector3 attenuation;
        // Get light emittance
        Vector3 emitted = hitRecord.material->emitted();
        // Get material's scattered ray for current ray and hit record
        if (depth < maxDepth && hitRecord.material->scatter(r, hitRecord, attenuation, scattered)) {
            /* The Rendering Equation */
            // L0 = Le + ∫(f * Li * cos(Ø) * dw), where:
            // L0(x,w0) - pixel color at hit point x, ray 0 direction w0
            // Le(x,w0) - emitted radiance at hit point x, ray 0 direction w0
            // f(x,wi->w0) - BRDF at hit point x (attenuation)
            // Li(x,wi) - radiance at hit point x, ray i direction wi

            // Shoot scattered rays recursively until a light is hit
            return emitted + attenuation * color(scattered, scene, depth + 1, maxDepth);
        } else {
            // End of recursion: light was hit, return emitted radiance
            return emitted;
        }
    } else {
        // End of recursion: ray didn't hit anything - return BG color
        return Vector3(0.0, 0.0, 0.0);
    }
}

int main() {
    /* Image parameters */
    const int width = 960;
    const int height = 400;
    const int spp = 800;
    const int rayBounce = 50;

    /* Scene */
    Material *wallMaterial = new Lambertian(Vector3(0.15, 0.26, 0.6));

    Hitable *list[11];
    list[0] = new Sphere(Vector3(0, -1000, -1), 1000, wallMaterial); // floor
    list[1] = new Sphere(Vector3(0, 0, -10000 - 2.25), 10000, wallMaterial); // back wall
    list[2] = new Sphere(Vector3(-10000 - 2.5, 0, -1), 10000, wallMaterial); // left wall
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

    /* Set up image buffer */
    Vector3 **buffer = new Vector3*[height];
    for (int i = 0; i < height; i++) buffer[i] = new Vector3[width];

    // TEMP PPM WRITER
    std::ofstream progressiveWriter("/home/entinfx/dev/gloom/Output/render.ppm", std::ifstream::trunc);
    progressiveWriter << "P3" << std::endl << width << " " << height << std::endl << 255 << std::endl;

    if (progressiveWriter) {
        for (int currentSample = 0; currentSample < spp; currentSample++) {
            std::cout << "SPP: " << currentSample + 1 << "/" << spp;

            Vector3 dofOffset = randomInUnitDisk();
            clock_t begin = clock();

            for (int line = height - 1; line >= 0; --line) {
                for (int pixel = 0; pixel < width; ++pixel) {
                    // Get ray for u, v
                    float u = (float(pixel) + drand48()) / float(width);
                    float v = (float(line) + drand48()) / float(height);
                    Ray ray = camera->getRay(u, v, dofOffset);
                    // Get color for ray, add to buffer
                    buffer[line][pixel] = (currentSample > 0) ? buffer[line][pixel] + color(ray, scene, 0, rayBounce) : color(ray, scene, 0, rayBounce);

                    /* Average buffer */
                    Vector3 color = buffer[line][pixel] / (currentSample + 1);

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
                    int r = static_cast<int>(255.999 * color.r());
                    int g = static_cast<int>(255.999 * color.g());
                    int b = static_cast<int>(255.999 * color.b());
                    // Render
                    // _renderView.buffer[i + width * (height - j - 1)] = r << 16 | g << 8 | b; // i and j might have to be swapped, careful
                    progressiveWriter << r << " " << g << " " << b << std::endl;
                }
            }

            clock_t end = clock();
            double timePassed = double(end - begin) / CLOCKS_PER_SEC;
            std::cout << ", Time: " << timePassed << "s." << std::endl;
        }

        progressiveWriter.close();
    } else {
        std::cout << "ERROR: std::ofstream failed." << std::endl;
    }
}
