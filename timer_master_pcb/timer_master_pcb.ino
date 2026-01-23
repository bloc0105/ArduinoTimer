#include <Wire.h>

volatile uint16_t timerDiplayNumbers[6] = {0,0,0,0,0,0};

volatile uint16_t masterTimerNumber = 0;

volatile bool timerEnabled[6] = {false,false,false,false,false,false};

volatile bool masterClockEnabled = false;

const uint8_t masterdisplayAddress = 10;

const uint8_t displayAddresses[6] = {9,8,7,6,5,4};

const uint8_t enablePin[6] = {PIN_PB5,PIN_PB4,PIN_PC3,PIN_PA7,PIN_PC2,PIN_PC1};
const uint8_t displayPin[6] = {PIN_PA1,PIN_PA6,PIN_PA5,PIN_PA4,PIN_PA3,PIN_PA2};
const uint8_t masterEnable = PIN_PB3;
const uint8_t masterReset = PIN_PC0;
const uint8_t masterEnabled = PIN_PB2;

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
    pinMode(displayPin[timerCounter], OUTPUT);
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

    uint8_t onesDigit = masterTimerNumber % 10;
    uint8_t tensDigit = (masterTimerNumber - onesDigit) / 10;

  Wire.beginTransmission(masterdisplayAddress);
  Wire.write(tensDigit);
  Wire.write(onesDigit);
  Wire.endTransmission();

  for (uint8_t timerCounter = 0; timerCounter < 6; ++timerCounter) {

    onesDigit = timerDiplayNumbers[timerCounter] % 10;
    tensDigit = (timerDiplayNumbers[timerCounter] - onesDigit) / 10;
    
    Wire.beginTransmission(displayAddresses[timerCounter]);
    Wire.write(tensDigit);
    Wire.write(onesDigit);
    Wire.endTransmission();

  }

}

void toggleEnableZero() {

  timerEnabled[0] = !timerEnabled[0];
  digitalWrite(displayPin[0],timerEnabled[0]);

}

void toggleEnableOne() {
  timerEnabled[1] = !timerEnabled[1];
  digitalWrite(displayPin[1],timerEnabled[1]);
}

void toggleEnableTwo() {
  timerEnabled[2] = !timerEnabled[2];
  digitalWrite(displayPin[2],timerEnabled[2]);
}

void toggleEnableThree() {
  timerEnabled[3] = !timerEnabled[3];
  digitalWrite(displayPin[3],timerEnabled[3]);
}

void toggleEnableFour() {
  timerEnabled[4] = !timerEnabled[4];
  digitalWrite(displayPin[4],timerEnabled[4]);
}

void toggleEnableFive() {
  timerEnabled[5] = !timerEnabled[5];
  digitalWrite(displayPin[5],timerEnabled[5]);
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
  unsigned int divideToMinutes = 2;

  
  if (masterClockEnabled && tempTime - currentTime >= divideToMinutes * 1000) {
    masterTimerNumber++;
    for (uint8_t timerCounter = 0; timerCounter < 6; ++timerCounter) {
      if (timerEnabled[timerCounter]) {
        timerDiplayNumbers[timerCounter]++;
      }
    }
  updateDisplay();
  currentTime = tempTime;
  }
}