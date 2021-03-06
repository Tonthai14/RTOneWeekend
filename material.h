#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "rtweekend.h"

struct hit_record;

class material {
  public:
    virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};

class lambertian : public material {
  public:
    color albedo;

    lambertian(const color& a) : albedo(a) {}

    virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
      auto scatter_direction = rec.normal + random_unit_vector();
      
      // Catch degenerate scatter direction (Random vector directly opposite of normal)
      if (scatter_direction.near_zero())
        scatter_direction = rec.normal;

      scattered = ray(rec.p, scatter_direction);
      attenuation = albedo;

      return true;
    }
};

class metal : public material {
  public:
    color albedo;
    double fuzz;

    metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
      vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
      scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere());
      attenuation = albedo;

      return dot(scattered.direction(), rec.normal) > 0;
    }
};

class dielectric : public material {
  public:
    double refraction_index;

    dielectric(double ir) : refraction_index(ir) {}

    virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
      // Always 1, glass surface absorbs no light
      attenuation = color(1.0, 1.0, 1.0);
      double refraction_ratio = rec.front_face ? (1.0/refraction_index) : refraction_index;

      vec3 unit_direction = unit_vector(r_in.direction());
      // sin(theta) = sqrt(1.0 - cos(theta)^2)
      // cos(theta) = R * n
      double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
      double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

      bool cant_refract = refraction_ratio * sin_theta > 1.0;
      
      vec3 direction;
      if (cant_refract)
        direction = reflect(unit_direction, rec.normal);
      else
        direction = refract(unit_direction, rec.normal, refraction_ratio); 

      scattered = ray(rec.p, direction);
      return true;
    }

  private:
    static double reflectance(double cosine, double reflect_index) {
      auto r0 = (1 - reflect_index) / (1 + reflect_index);
      r0 = r0 * r0;
      
      return r0 + (1 - r0)*pow((1 - cosine), 5);
    }
};

#endif