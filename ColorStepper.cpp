#ifndef COLORSTEPPER_CPP
#define COLORSTEPPER_CPP

#include <stdint.h>

#include "Color.cpp"

class ColorStepper {
  public:
    ColorStepper(long stageDuration) {
      this->stageDuration = stageDuration;
      this->stageCount = sizeof(this->stages) / sizeof(Color);
      this->period = this->stageCount * this->stageDuration;
    }

    void step() {
      this->k = (this->k + 1) % period;
    }

    uint16_t getTime() {
      return (this->k % this->stageDuration) * 255 / this->stageDuration;
    }

    Color getColor() {
      uint32_t stage0 = this->getStage();
      uint32_t stage1 = (stage0 + 1) % this->stageCount;
      uint16_t time = this->getTime();
      
      Color color = this->stages[stage0].blend(this->stages[stage1], time);

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
    uint32_t k = 0;
    Color stages[6] = {
      Color(255, 0, 0),
      Color(255, 255, 0),
      Color(0, 255, 0),
      Color(0, 255, 255),
      Color(0, 0, 255),
      Color(255, 0, 255)
    };
    uint32_t stageDuration;
    uint32_t stageCount;
    uint32_t period;

    uint16_t getStage() {
      return this->k / this->stageDuration;
    }
};

#endif
