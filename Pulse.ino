
#define TAIL_SIZE 5
/*
class Pulse {
  public:
    Pulse (uint16_t iPos, uint8_t iR, uint8_t iG, uint8_t iB) {
      pos = iPos;
      r = iR;
      g = iG;
      b = iB;
    }

    bool done () {
      return pos > (NUMPIXELS + TAIL_SIZE);
    }

    void update () {
      pos++;
    }

    void draw () {
      uint32_t color = 0xFF0000;
      uint8_t mr = r;
      uint8_t mg = g;
      uint8_t mb = b;
      for (int i = pos  ;  i >= max(pos - TAIL_SIZE, 0)  ;  i--) {
        if (i < NUMPIXELS)
          strip.setPixelColor (i, color);
        // still red but less bright
        mr = max (mr-20, 0);
        mg = max (mg-20, 0);
        mb = max (mb-20, 0);
        color = (mr << 16) | (mg << 8) | (mb);
      }
    }
    
   private:
    uint16_t pos;
    uint8_t r, g, b;
}
*/
