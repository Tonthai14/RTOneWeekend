#include <iostream>

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "material.h"
#include "rtweekend.h"
#include "sphere.h"

hittable_list random_scene() {
  hittable_list world;

  auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
  world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      auto choose_mat = random_double();
      point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

      if ((center - point3(4, 0.2, 0)).length() > 0.9) {
        shared_ptr<material> material;
        
        if (choose_mat < 0.8) { // Diffuse
          auto albedo = color::random() * color::random();
          material = make_shared<lambertian>(albedo);
        }
        else if (choose_mat < 0.95) { // Metal
          auto albedo = color::random(0.5, 1);
          auto fuzz = random_double(0, 0.5);
          material = make_shared<metal>(albedo, fuzz);
        }
        else { // Glass
          material = make_shared<dielectric>(1.5);
        }

        world.add(make_shared<sphere>(center, 0.2, material));
      }
    }
  }

  auto material1 = make_shared<dielectric>(1.5);
  world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

  auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
  world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

  auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
  world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

  return world;
}

color ray_color(const ray& r, const hittable& world, int depth) {
  if (depth <= 0) // If ray bounce limit exceeded, no more light gathered
    return color(0, 0, 0);

  hit_record rec;
  // 0.001 for shadow acne, i.e. rays reflecting close to t = 0 but not exactly
  if (world.hit(r, 0.001, infinity, rec)) {
    // // Diffuse light bounches off in a random direction
    // // So, we calculate a random vector within a unit sphere distance from the object's surface
    // // Unit sphere center is at P + N with P being surface hit
    // point3 target = rec.p + rec.normal + random_unit_vector();

    ray scattered;
    color attenuation;
    if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
      return attenuation * ray_color(scattered, world, depth - 1);

    return color(0, 0, 0);
    // // Random vector points towards direction of ray origin
    // return 0.5 * ray_color(ray(rec.p, target - rec.p), world, depth - 1);
  }

  vec3 unit_direction = unit_vector(r.direction());
  auto t = 0.5 * (unit_direction.y() + 1.0);

  return (1.0 - t) * color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

int main() {
  const auto ASPECT_RATIO = 16.0 / 9.0;
  const int IMG_WIDTH = 1200;
  const int IMG_HEIGHT = static_cast<int>(IMG_WIDTH / ASPECT_RATIO);
  const int samples_per_pixel = 10;
  const int max_depth = 50;

  // World
  auto world = random_scene();

  // Camera
  point3 lookfrom(13, 2, 3);
  point3 lookat(0, 0, 0);
  vec3 viewup(0, 1, 0);
  auto focus_dist = 10.0;
  auto aperture = 0.1;

  camera camera(lookfrom, lookat, viewup, 20, ASPECT_RATIO, aperture, focus_dist);

  // Defining PPM image parameters
  std::cout << "P3\n" << IMG_WIDTH << " " << IMG_HEIGHT << "\n255\n";

  // We want to start from bottom left corner, so we start at the highest index in the y-direction
  for (int row = IMG_HEIGHT - 1; row >= 0; --row) {
    std::cerr << "\rScanlines remaining: " << row << " " << std::flush;
    for (int col = 0; col < IMG_WIDTH; ++col) { 
      color pixel_color(0, 0, 0);
      for (int sample = 0; sample < samples_per_pixel; sample++) {
        // For given pixel, cast a random ray within that pixel for however many samples
        auto u = (double(col) + random_double()) / (IMG_WIDTH - 1);
        auto v = (double(row) + random_double()) / (IMG_HEIGHT - 1);

        ray r = camera.get_ray(u, v);
        pixel_color += ray_color(r, world, max_depth);
      }
      write_color(std::cout, pixel_color, samples_per_pixel);
    }
  }

  std::cerr << "\nDone.\n";
}