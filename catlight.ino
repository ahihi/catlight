int redPin = 3;
int greenPin = 6;
int bluePin = 5;

//uncomment this line if using a Common Anode LED
#define COMMON_ANODE

//#define DEBUG

#include "Color.cpp"
#include "ColorStepper.cpp"

ColorStepper cs(80000);

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  #ifdef DEBUG
  Serial.begin(115200);
  #endif
}

void loop() {
  cs.step();
  Color color = cs.getColor();
  setLedColor(color);
}

void setLedColor(Color color) {
  int red = color.r;
  int green = color.g;
  int blue = color.b;

  #ifdef COMMON_ANODE
  red = 255 - red;
  green = 255 - green;
  blue = 255 - blue;
  #endif

  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

