#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
  public:
    point3 center;
    double radius;
    shared_ptr<material> mat_ptr;

    sphere() {}
    sphere(point3 cen, double r, shared_ptr<material> m) 
      : center(cen), radius(r), mat_ptr(m) {};

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    /*
   * Based on the quadratic form ax^2 + bx + c = 0
   * Where ray-sphere intersection can be represented as:
   * b*b(t^2) + 2b*(A - C) + (A - C)*(A - c)-r^2 = 0
   * --> b = ray direction (Screen coordinates)
   * --> A = ray origin (Camera coordinates)
   * --> C = sphere center
   * --> r = sphere radius
   * --> t = unknown to solve for
   * --> * represents the dot product between two variables
   * Then, we solve for the discriminant to determine the number of roots
   * D = b^2 - 4ac,
   * D > 0 means equation has 2 roots (Intersects front of sphere and exits through back)
   * D = 0 means equation has 1 root
   * D < 0 means equation has 0 roots (No real roots)
   */
  vec3 circle_dist = r.origin() - center;
  // Vector dot product with itself is just squared length of the vector
  auto a = r.direction().length_squared(); 
  // Since b has a factor of 2, we can reduce the quadratic equation to
  // (-b +- sqrt(b^2 - ac)) / a
  auto b = dot(circle_dist, r.direction());
  auto c = circle_dist.length_squared() - radius*radius;

  auto discriminant = b*b - a*c;
  if (discriminant < 0) return false;

  auto sqrt_dis = sqrt(discriminant);
  auto root = (-b - sqrt_dis) / a;
  // Finding nearest root within acceptable range
  if (root < t_min || root > t_max) {
    root = (-b + sqrt_dis) / a;
    if (root < t_min || root > t_max)
      return false;
  }

  rec.t = root;
  // Point where ray intersects the sphere
  rec.p = r.at(rec.t);
  // Divided by radius to turn into unit vector
  vec3 outward_normal = (rec.p - center) / radius;
  // Determine front face
  rec.set_face_normal(r, outward_normal);
  rec.mat_ptr = mat_ptr;

  return true;
}

#endif