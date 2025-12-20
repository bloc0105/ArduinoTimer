// 7-segment pins (a b c d e f g) — change if your wiring is different

#include <Wire.h>
const uint8_t segPins[7] = {16, 15, 14, 4, 3, 2, 1};
const uint8_t enablePin = 6;
const uint8_t enableIndicator = 5;
const uint8_t carrybitIndicator = 11;
const uint8_t masterReset = 0;

volatile bool enabled = false;


const uint8_t hexPatterns[10] = {
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101, // 5
  0b01111101, // 6
  0b00000111, // 7
  0b01111111, // 8
  0b01100111, // 9
  
};

volatile unsigned long totalTime = 0;  
volatile unsigned long startTime = 0;
volatile unsigned long currentTime = 0;

void setup() {
  Wire.begin();
  // 7-segment pins as outputs
  for (uint8_t i = 0; i < 7; i++) {
    pinMode(segPins[i], OUTPUT);
  }

  pinMode(enableIndicator, OUTPUT);
  pinMode(carrybitIndicator, OUTPUT);

  pinMode(enablePin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(enablePin), toggleEnable, FALLING);

  pinMode(masterReset, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(masterReset), resetToZero, FALLING);

  updateDisplay(0);  
}

void toggleEnable() {
  enabled = !enabled;
  if (enabled) {
    startTime = millis();
  }
  else {
    totalTime += (millis() - startTime);

  }  
}

void resetToZero() {
  totalTime = 0;
  enabled = false;
  digitalWrite(carrybitIndicator, LOW);
  updateDisplay(0);
}

void updateDisplay(unsigned long inputTime) {
  int count  = inputTime % 10;
  uint8_t pattern = hexPatterns[count];
  for (uint8_t i = 0; i < 7; i++) {
    digitalWrite(segPins[i], bitRead(pattern, i));
  }
  digitalWrite(enableIndicator, enabled);

  digitalWrite(carrybitIndicator, !(inputTime % 10) && inputTime != 0);
  Wire.beginTransmission(4);
  Wire.write(count);
  Wire.endTransmission();
}

void loop() {
  int divideToMinutes = 2;
  unsigned long tempTime = 0;
  if (enabled) {
  tempTime = totalTime + (millis() - startTime);
  }
  else {
    tempTime = totalTime;
  }
  updateDisplay(tempTime / 1000 / divideToMinutes);

}