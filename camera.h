#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

class camera {
  public:
    camera(point3 lookfrom, 
           point3 lookat, 
           vec3 viewup, 
           double vert_fov, // FOV in degrees
           double aspect_ratio,
           double aperture,
           double focus_dist) 
    {
      auto theta = degrees_to_radians(vert_fov);
      auto h = tan(theta / 2); // Height from z = 0 to top boundary of FOV

      auto viewport_height = 2.0 * h;
      auto viewport_width = aspect_ratio * viewport_height;
      const auto focal_length = 1.0; // 1 unit distance (z-direction) between camera and projection plane

      w = unit_vector(lookfrom - lookat); // -w being the look direction
      u = unit_vector(cross(viewup, w)); // vector pointing directly right
      v = cross(w, u); // vector pointing directly up 

      origin = lookfrom;
      horizontal = focus_dist * viewport_width * u;
      vertical = focus_dist * viewport_height * v;
      lower_left_corner = origin - horizontal/2 - vertical/2 - focus_dist*w;
    
      lens_radius = aperture / 2;
    }

    ray get_ray(double s, double t) const {
      vec3 camera_disk = lens_radius * random_in_unit_disk();
      vec3 offset = u*camera_disk.x() + v*camera_disk.y();

      return ray(
        origin + offset, 
        lower_left_corner + s*horizontal + t*vertical - origin - offset
      );
    }

  private:
    point3 origin;
    point3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;
    double lens_radius;
};

#endif