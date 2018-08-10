#ifndef COLORSTEPPER_CPP
#define COLORSTEPPER_CPP

#include <stdint.h>

#include "Color.cpp"
#include "NoiseLerp.cpp"

class ColorStepper {
  public:
    ColorStepper(uint32_t colorDuration, uint32_t flicker1Duration, uint32_t flicker2Duration, uint32_t flicker3Duration) :
      flicker1(0xfeedfaceUL, flicker1Duration),
      flicker2(0xf4ccf4ccUL, flicker2Duration),
      flicker3(0xb00b00b1UL, flicker3Duration)
    {
      this->colorIx = 0;
      this->colorCount = sizeof(this->stages) / sizeof(Color);
      this->colorDuration = colorDuration;
      this->colorPeriod = this->colorCount * this->colorDuration;
    }

    void step() {
      this->colorIx = (this->colorIx + 1) % this->colorPeriod;
      this->flicker1.step();
      this->flicker2.step();
      this->flicker3.step();
    }

    uint16_t time() {
      return (this->colorIx % this->colorDuration) * 255 / this->colorDuration;
    }

    Color value() {
      uint32_t stage0 = this->getStage();
      uint32_t stage1 = (stage0 + 1) % this->colorCount;
      uint16_t time = this->time();

      uint32_t attenuation = 4;
      uint32_t rNoise = 255 - (255 - this->flicker1.value()) / attenuation;
      uint32_t gNoise = 255 - (255 - this->flicker2.value()) / attenuation;
      uint32_t bNoise = 255 - (255 - this->flicker3.value()) / attenuation;

      Color color = this->stages[stage0]
        .blend(this->stages[stage1], time)
        .multiply(Color(rNoise, gNoise, bNoise));

      #ifdef DEBUG
      Serial.print("stage0 = "); Serial.print(stage0);
      Serial.print(", stage1 = "); Serial.print(stage1);
      Serial.print(", time = "); Serial.print(time);
      Serial.print(", color = ("); Serial.print(color.r);
      Serial.print(", "); Serial.print(color.g);
      Serial.print(", "); Serial.print(color.b); Serial.print(")");
      Serial.println();
      #endif

      return color;
    }

  private:
    Color stages[6] = {
      Color(255, 0, 0),
      Color(255, 255, 0),
      Color(0, 255, 0),
      Color(0, 255, 255),
      Color(0, 0, 255),
      Color(255, 0, 255)
    };
    NoiseLerp flicker1, flicker2, flicker3;
    uint32_t colorIx;
    uint32_t colorCount;
    uint32_t colorDuration;
    uint32_t colorPeriod;
    
    uint16_t getStage() {
      return this->colorIx / this->colorDuration;
    }
};

#endif
