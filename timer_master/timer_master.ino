#include <Wire.h>

volatile uint16_t timerDiplayNumbers[6] = {0,0,0,0,0,0};
volatile bool timerEnabled[6] = {false,false,false,false,false,false};

volatile bool masterClockEnabled = false;


const uint8_t displayAddresses[6][2] = {
 {10,11},
 {20,21},
 {30,31},
 {40,41},
 {50,51},
 {60,61}};

const uint8_t enablePin[6] = {0,1,2,3,4,5};
const uint8_t masterEnable = 11;
const uint8_t masterReset = 15;
const uint8_t masterEnabled = 14;


volatile unsigned long currentTime = 0;
volatile unsigned long timeDiff = 0;

void setup() {
  Wire.begin();

  pinMode(masterEnable, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(masterEnable), toggleMasterEnable, CHANGE);

  pinMode(masterReset, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(masterReset), resetToZero, FALLING);

  pinMode(masterEnabled, OUTPUT);

  for (uint8_t timerCounter = 0; timerCounter < 6; ++ timerCounter) {

    pinMode(enablePin[timerCounter], INPUT_PULLUP);
  }
attachInterrupt(enablePin[0],toggleEnableZero, FALLING);
attachInterrupt(enablePin[1],toggleEnableOne, FALLING);
attachInterrupt(enablePin[2],toggleEnableTwo, FALLING);
attachInterrupt(enablePin[3],toggleEnableThree, FALLING);
attachInterrupt(enablePin[4],toggleEnableFour, FALLING);
attachInterrupt(enablePin[5],toggleEnableFive, FALLING);


  updateDisplay();  
}


void resetToZero() {
  for (uint8_t timerCounter = 0; timerCounter < 6; ++timerCounter) {
        timerDiplayNumbers[timerCounter] = 0;
  }
  updateDisplay();
}

void updateDisplay() {

  for (uint8_t timerCounter = 0; timerCounter < 6; ++timerCounter) {

    uint8_t onesDigit = timerDiplayNumbers[timerCounter] % 10;
    uint8_t tensDigit = (timerDiplayNumbers[timerCounter] - onesDigit) / 10;

  Wire.beginTransmission(displayAddresses[timerCounter][0]);
  Wire.write(onesDigit);
  Wire.endTransmission();

  Wire.beginTransmission(displayAddresses[timerCounter][1]);
  Wire.write(tensDigit);
  Wire.endTransmission();

  }

}

void toggleEnableZero() {

  timerEnabled[0] = !timerEnabled[0];

}

void toggleEnableOne() {
  timerEnabled[1] = !timerEnabled[1];
}

void toggleEnableTwo() {
  timerEnabled[2] = !timerEnabled[2];
}

void toggleEnableThree() {
  timerEnabled[3] = !timerEnabled[3];
}

void toggleEnableFour() {
  timerEnabled[4] = !timerEnabled[4];
}

void toggleEnableFive() {
  timerEnabled[5] = !timerEnabled[5];
}

void toggleMasterEnable() {

  masterClockEnabled = digitalRead(masterEnable);
  
  if (masterClockEnabled) {
    currentTime = millis() - timeDiff;
  }
  else {
    timeDiff = millis() - currentTime;
  }


  digitalWrite(masterEnabled, masterClockEnabled); 
}

void loop() {
  unsigned long tempTime = millis();
  unsigned int divideToMinutes = 360;

  
  if (masterClockEnabled && tempTime - currentTime >= divideToMinutes * 1000) {
    for (uint8_t timerCounter = 0; timerCounter < 6; ++timerCounter) {
      if (timerEnabled[timerCounter]) {
        timerDiplayNumbers[timerCounter]++;
      }
    }
  updateDisplay();
  currentTime = tempTime;
  }
}