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



#include <Bounce.h>
#include "AudioSampleImpulse.h"
//#include "Coefficcients.h"

double Coefficcients[25][5] = {
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

#define NUMNOTES   7
#define MIDIOFFSET 60

int digitalPins[] = {0, 1, 2, 3, 4, 5, 9, 14, 16, 17, 22, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33};
float notes[NUMNOTES] = {20.75, 14.6, 9.6, 5.3, 3.4, -0.38, -3.76};

#define NOTE1_PIN   digitalPins[0]
#define NOTE2_PIN   digitalPins[1]
#define NOTE3_PIN   digitalPins[2]
#define NOTE4_PIN   digitalPins[3]
#define NOTE5_PIN   digitalPins[4]
#define NOTE6_PIN   digitalPins[5]
#define NOTE7_PIN   digitalPins[6]
#define CHANGE_PIN  digitalPins[7]
#define LED_PIN     LED_BUILTIN

Bounce butNote[NUMNOTES] = {
  Bounce(NOTE1_PIN, 15),
  Bounce(NOTE2_PIN, 15),
  Bounce(NOTE3_PIN, 15),
  Bounce(NOTE4_PIN, 15),
  Bounce(NOTE5_PIN, 15),
  Bounce(NOTE6_PIN, 15),
  Bounce(NOTE7_PIN, 15)
};

Bounce butChange = Bounce(CHANGE_PIN, 15);
int currentVowel;



void setup() {

  //Vars init
  currentVowel = 1;


  //Serial init
  Serial.begin(9600);


  //Teensy audio init
  AudioMemory(160);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  mix_echo.gain(0, 0.5);
  mix_echo.gain(1, 0);
  setCoeffs(currentVowel);
  mix_filter.gain(0, 0.25);
  mix_filter.gain(1, 0.25);
  mix_filter.gain(2, 0.25);
  mix_filter.gain(3, 0.25);


  //Hardware init
  for (int f = 0; f < NUMNOTES; f++) {
    pinMode(digitalPins[f], INPUT_PULLUP);
  }
  pinMode(CHANGE_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  delay(1000);

}



void loop() {

  for (int f = NUMNOTES - 1; f >= 0 ; f--) {

    butNote[f].update();

    if (butNote[f].risingEdge()) {
      mix_echo.gain(1, 0.7);
      digitalWrite(LED_PIN, LOW);
    }

    if (butNote[f].fallingEdge()) {

      Serial.print("f=");
      Serial.print(f);
      Serial.print("     notes[f]=");
      Serial.println(MIDIOFFSET + notes[f]);
      float period = butToPeriod(MIDIOFFSET + notes[f]);
      echo.delay(0, period);
      mix_echo.gain(1, 0.99);
      impulse.play(AudioSampleImpulse);

      digitalWrite(LED_PIN, HIGH);
      break;
    }
  }

  butChange.update();

  if (butChange.risingEdge()) {
  }

  if (butChange.fallingEdge()) {
    currentVowel++;
    if (currentVowel >= 5) {
      currentVowel = 0;
    }
    setCoeffs(currentVowel);
    Serial.print("Current vowel: ");
    Serial.println(currentVowel);

  }

}



float butToPeriod(float midiVal) {

  float hz = 440.0 * pow(2,(midiVal-69.0)/12.0);
  float period = 1000.0 / hz;

  Serial.print("Midi: ");
  Serial.print(midiVal);
  Serial.print(" Period: ");
  Serial.print(hz);
  Serial.print("Hz / ");
  Serial.print(period);
  Serial.println("ms");
  return period;
}



void setCoeffs(int v) {
  biquad1.setCoefficients(0, Coefficcients[v * 5 + 0]);
  biquad2.setCoefficients(0, Coefficcients[v * 5 + 1]);
  biquad3.setCoefficients(0, Coefficcients[v * 5 + 2]);
  biquad4.setCoefficients(0, Coefficcients[v * 5 + 3]);
}
