#ifndef COLOR_CPP
#define COLOR_CPP

#include <stdint.h>

class Color {
  public:
    uint16_t r, g, b;

    Color(uint16_t r, uint16_t g, uint16_t b) {
      this->r = r;
      this->g = g;
      this->b = b;
    }

    Color blend(Color other, uint16_t amount) {
      return Color(
        ((255 - amount) * this->r + amount * other.r) / 255,
        ((255 - amount) * this->g + amount * other.g) / 255,
        ((255 - amount) * this->b + amount * other.b) / 255
      );
    }
};

#endif
