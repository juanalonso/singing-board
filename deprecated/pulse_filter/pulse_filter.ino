#include <Bounce.h>
#include "AudioSampleImpulse.h"

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioPlayMemory          impulse;       //xy=77.5,80
AudioMixer4              mix_echo;         //xy=310,100
AudioEffectDelay         echo;         //xy=310,300
AudioFilterBiquad        biquad1;        //xy=553.5,100
AudioFilterBiquad        biquad2;        //xy=553.5,160
AudioFilterBiquad        biquad3;        //xy=553.5,220
AudioFilterBiquad        biquad4;        //xy=553.5,280
AudioMixer4              mix_filter;         //xy=790,120
AudioOutputI2S           i2s1;           //xy=970,120
AudioConnection          patchCord1(impulse, 0, mix_echo, 0);
AudioConnection          patchCord2(mix_echo, echo);
AudioConnection          patchCord3(mix_echo, biquad1);
AudioConnection          patchCord4(mix_echo, biquad2);
AudioConnection          patchCord5(mix_echo, biquad3);
AudioConnection          patchCord6(mix_echo, biquad4);
AudioConnection          patchCord7(echo, 0, mix_echo, 1);
AudioConnection          patchCord8(biquad1, 0, mix_filter, 0);
AudioConnection          patchCord9(biquad2, 0, mix_filter, 1);
AudioConnection          patchCord10(biquad3, 0, mix_filter, 2);
AudioConnection          patchCord11(biquad4, 0, mix_filter, 3);
AudioConnection          patchCord12(mix_filter, 0, i2s1, 0);
AudioConnection          patchCord13(mix_filter, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=79,180
// GUItool: end automatically generated code


#define NUMNOTES   6
#define MIDIOFFSET 0

//CAUTION:
//These digital pins also work as analog:  14 (A0), 16 (A2), 17 (A3) and 22 (A8).
int digitalPins[] = {0, 1, 2, 3, 4, 5, 9, 16, 17, 22, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33};

Bounce butNote[NUMNOTES] = {
  Bounce(digitalPins[1], 15),
  Bounce(digitalPins[2], 15),
  Bounce(digitalPins[3], 15),
  Bounce(digitalPins[4], 15),
  Bounce(digitalPins[5], 15),
  Bounce(digitalPins[6], 15)
};

Bounce butNext = Bounce(digitalPins[0], 15);


int a0history;


int currentVowel = 0;

double coef[][5] = {
  //A
  {1.0, 0.0, 0.0, -1.9842, 0.9915},
  {1.0, 0.0, 0.0, -1.9682, 0.9901},
  {1.0, 0.0, 0.0, -1.8833, 0.9844},
  {1.0, 0.0, 0.0, -1.8634, 0.9830},
  {1.0, 0.0, 0.0, -1.8314, 0.9816},
  //E
  {1.0, 0.0, 0.0, -1.9911, 0.9943},
  {1.0, 0.0, 0.0, -1.9359, 0.9887},
  {1.0, 0.0, 0.0, -1.8708, 0.9859},
  {1.0, 0.0, 0.0, -1.8273, 0.9830},
  {1.0, 0.0, 0.0, -1.7927, 0.9830},
  //I
  {1.0, 0.0, 0.0, -1.9902, 0.9915},
  {1.0, 0.0, 0.0, -1.9258, 0.9873},
  {1.0, 0.0, 0.0, -1.8511, 0.9859},
  {1.0, 0.0, 0.0, -1.7987, 0.9830},
  {1.0, 0.0, 0.0, -1.7627, 0.9830},
  //O
  {1.0, 0.0, 0.0, -1.9911, 0.9943},
  {1.0, 0.0, 0.0, -1.9773, 0.9887},
  {1.0, 0.0, 0.0, -1.8708, 0.9859},
  {1.0, 0.0, 0.0, -1.8485, 0.9830},
  {1.0, 0.0, 0.0, -1.8161, 0.9830},
  //U
  {1.0, 0.0, 0.0, -1.9918, 0.9943},
  {1.0, 0.0, 0.0, -1.9814, 0.9887},
  {1.0, 0.0, 0.0, -1.8708, 0.9859},
  {1.0, 0.0, 0.0, -1.8407, 0.9830},
  {1.0, 0.0, 0.0, -1.8104, 0.9830},
};



void setup() {

  for (int f = 0; f < NUMNOTES; f++) {
    pinMode(digitalPins[f + 1], INPUT_PULLUP);
  }
  pinMode(digitalPins[0], INPUT_PULLUP);

  a0history = analogRead(A0);
  float initVol = potToGain(a0history);

  Serial.begin(9600);

  AudioMemory(160);

  sgtl5000_1.enable();
  //sgtl5000_1.volume(initVol);
  sgtl5000_1.volume(0.5);

  mix_echo.gain(0, 0.5);
  mix_echo.gain(1, 0);

  //We will probably not need this
  //echo.delay(0, initPeriod);

  biquad1.setCoefficients(0, coef[currentVowel * 5 + 0]);
  biquad2.setCoefficients(0, coef[currentVowel * 5 + 1]);
  biquad3.setCoefficients(0, coef[currentVowel * 5 + 2]);
  biquad4.setCoefficients(0, coef[currentVowel * 5 + 3]);

  mix_filter.gain(0, 0.25);
  mix_filter.gain(1, 0.25);
  mix_filter.gain(2, 0.25);
  mix_filter.gain(3, 0.25);

  delay(1000);

}



void loop() {

  //Volume
  int a0 = analogRead(A0);
  if (a0 > a0history + 15 || a0 < a0history - 15) {
    float gain = potToGain(a0history);
    //sgtl5000_1.volume(gain);
    a0history = a0;
  }

  //Note selection
  for (int f = NUMNOTES - 1; f >= 0 ; f--) {

    butNote[f].update();

    if (butNote[f].risingEdge()) {
      mix_echo.gain(1, 0.7);
    }

    if (butNote[f].fallingEdge()) {

      mix_echo.gain(1, 0);
      float period = butToPeriod(f);
      echo.delay(0, period);

      //If we get strange results, swithc the following two lines
      impulse.play(AudioSampleImpulse);
      mix_echo.gain(1, 0.99);

      break;
    }
  }

  //Vowel selection
  butNext.update();

  if (butNext.fallingEdge()) {
    currentVowel++;
    if (currentVowel >= 5) {
      currentVowel = 0;
    }
    biquad1.setCoefficients(0, coef[currentVowel * 5 + 0]);
    biquad2.setCoefficients(0, coef[currentVowel * 5 + 1]);
    biquad3.setCoefficients(0, coef[currentVowel * 5 + 2]);
    biquad4.setCoefficients(0, coef[currentVowel * 5 + 3]);
    Serial.print("Current vowel: ");
    Serial.println(currentVowel);
  }

}


float butToPeriod(int butValue) {

  float hz = 110.0 * pow(1.059463094359295, butValue + MIDIOFFSET);
  float period = 1000.0 / (float)hz;

  Serial.print("Midi: ");
  Serial.print(butValue + MIDIOFFSET);
  Serial.print(" Period: ");
  Serial.print(hz);
  Serial.print("Hz / ");
  Serial.print(period);
  Serial.println("ms");
  return period;
}


float potToGain(int potValue) {
  float potGain = (float)map(potValue, 0, 1023, 0, 800) / 1000.0;
  Serial.print("Gain: ");
  Serial.println(potGain);
  return potGain;
}
