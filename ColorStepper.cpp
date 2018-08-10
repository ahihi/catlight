#ifndef COLORSTEPPER_CPP
#define COLORSTEPPER_CPP

#include <stdint.h>

#include "Color.cpp"
#include "LFSR.cpp"

#define AVG_COUNT 128

class ColorStepper {
  public:
    ColorStepper(long colorDuration, long noiseDuration) {
      this->colorCount = sizeof(this->stages) / sizeof(Color);
      this->colorDuration = colorDuration;
      this->colorPeriod = this->colorCount * this->colorDuration;
      
      this->noiseDuration = noiseDuration;
      this->noisePeriod = this->noiseDuration;
    }

    void step() {
      if(this->noiseIx == 0) {
        this->noise0 = this->noise1;
        this->noise1 = this->lfsr.next_byte();  
      }

      this->colorIx = (this->colorIx + 1) % this->colorPeriod;
      this->noiseIx = (this->noiseIx + 1) % this->noisePeriod;
    }

    uint16_t getTime() {
      return (this->colorIx % this->colorDuration) * 255 / this->colorDuration;
    }

    Color getColor() {
      uint32_t stage0 = this->getStage();
      uint32_t stage1 = (stage0 + 1) % this->colorCount;
      uint16_t time = this->getTime();

      uint32_t noiseBlendAmount = this->noiseIx * 255 / this->noiseDuration;
      uint32_t noiseBlend = ((255 - noiseBlendAmount) * this->noise0 + noiseBlendAmount * this->noise1) / 255;
      uint16_t noise = 255 - (255 - noiseBlend) / 6;

      Color color = this->stages[stage0]
        .blend(this->stages[stage1], time)
        .multiply(noise);

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
    LFSR lfsr;
    uint32_t colorIx = 0;
    uint32_t colorCount;
    uint32_t colorDuration;
    uint32_t colorPeriod;
    
    uint32_t noiseIx = 0;
    uint32_t noiseDuration;
    uint32_t noisePeriod;
    uint32_t noise0 = 0;
    uint32_t noise1 = 0;
    
    uint16_t getStage() {
      return this->colorIx / this->colorDuration;
    }
};

#endif
