#ifndef NOISELERP_CPP
#define NOISELERP_CPP

#include <stdint.h>

#include "LFSR.cpp"

class NoiseLerp {
  public:
    NoiseLerp(uint32_t seed, uint32_t duration):
      lfsr(seed)
    {
      this->k = 0;
      this->y = 0;
      this->y0 = 0;
      this->y1 = 0;
      this->duration = duration;
      this->compute();
    }

    void step() {
      this->k = (this->k + 1) % this->duration;
      this->compute();
    }

    uint32_t value() {
      return this->y;
    }
  
  private:
    LFSR lfsr;
    uint32_t k, y, y0, y1;
    uint32_t duration;

    void compute() {
      if(this->k == 0) {
        this->y0 = this->y1;
        this->y1 = this->lfsr.next_byte();
      }  

      uint32_t kBlend = this->k * 255 / this->duration;
      this->y = ((255 - kBlend) * this->y0 + kBlend * this->y1) / 255;
    }
};

#endif
