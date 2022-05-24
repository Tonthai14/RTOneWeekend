#ifndef COLOR_H
#define COLOR_H

#include <iostream>

#include "vec3.h"

void write_color(std::ostream& out, color pixel_color, int samples_per_pixel) {
  const auto SCALE = 1.0 / samples_per_pixel;

  // Gamma=2.0 correction for more accurate color (1/gamma or simplify to sqrt)
  auto r = sqrt(pixel_color.x() * SCALE);
  auto g = sqrt(pixel_color.y() * SCALE);
  auto b = sqrt(pixel_color.z() * SCALE);

  // [0, 255] range for RGB values
  out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << " "
      << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << " "
      << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << "\n";
}

#endif 