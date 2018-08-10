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

    Color add(Color other) {
      return Color(
        this->r + other.r,
        this->g + other.g,
        this->b + other.b
      );
    }

    Color multiply(uint16_t m) {
      uint32_t m32 = static_cast<uint32_t>(m);
      return Color(
        m32 * this->r / 255,
        m32 * this->g / 255,
        m32 * this->b / 255
      );
    }

    Color blend(Color other, uint16_t amount) {
      return Color(
        blend(this->r, other.r, amount),
        blend(this->g, other.g, amount),
        blend(this->b, other.b, amount)
      );
    }

  private:
    static uint16_t blend(uint16_t a, uint16_t b, uint16_t amount) {
      return ((255 - amount) * a + amount * b) / 255;
    }
};

#endif
