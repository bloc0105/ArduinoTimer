#include <Wire.h>
const uint8_t segPinsTens[7] = {PIN_PB3, PIN_PB4, PIN_PC3, PIN_PC1, PIN_PA7, PIN_PA5, PIN_PA1};
const uint8_t segPinsOnes[7] = {PIN_PA4, PIN_PC2, PIN_PC0, PIN_PB2, PIN_PA6, PIN_PA2, PIN_PA3};

// Mapping for common cathode display (High = ON)
// Index 0-9 = Numbers, 10-15 = A-F
const byte hexToSegments[] = {
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101, // 5
  0b01111101, // 6
  0b00000111, // 7
  0b01111111, // 8
  0b01101111, // 9
  0b01110111, // A (10)
  0b01111100, // b (11) - usually lowercase so it doesn't look like '8'
  0b00111001, // C (12)
  0b01011110, // d (13) - usually lowercase so it doesn't look like '0'
  0b01111001, // E (14)
  0b01110001  // F (15)
};

void setup() {
  Wire.begin();
  Wire.onReceive(receiveEvent);
  // 7-segment pins as outputs
  for (uint8_t i = 0; i < 7; i++) {
    pinMode(segPinsOnes[i], OUTPUT);
    pinMode(segPinsTens[i], OUTPUT);
  }
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  
  uint8_t tensDigit = Wire.read();
  uint8_t onesDigit = Wire.read();

  while(Wire.available()) // loop through all but the last
  {
    //clear the buffer, I never expect us to actually get here
    Wire.read();
  }

  uint8_t pattern = hexToSegments[tensDigit];
  for (uint8_t i = 0; i < 7; i++) {
    digitalWrite(segPinsTens[i], bitRead(pattern, i));
  }

  pattern = hexToSegments[onesDigit];
  for (uint8_t i = 0; i < 7; i++) {
    digitalWrite(segPinsOnes[i], bitRead(pattern, i));
  }


}

void loop()
{

}
