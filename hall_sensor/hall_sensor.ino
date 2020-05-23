#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioPlayMemory          impulse;        //xy=144,95
AudioMixer4              mix_delay_impulse; //xy=345,115
AudioEffectDelay         delay_impulse;  //xy=345,315
AudioFilterBiquad        biquad1;        //xy=588,115
AudioFilterBiquad        biquad2;        //xy=588,175
AudioFilterBiquad        biquad3;        //xy=588,235
AudioFilterBiquad        biquad4;        //xy=588,295
AudioMixer4              mix_filter;     //xy=785,134
AudioEffectFreeverb      reverb;         //xy=936,232
AudioMixer4              mix_reverb;     //xy=1092,153
AudioOutputI2S           i2s;            //xy=1289,146
AudioConnection          patchCord1(impulse, 0, mix_delay_impulse, 0);
AudioConnection          patchCord2(mix_delay_impulse, delay_impulse);
AudioConnection          patchCord3(mix_delay_impulse, biquad1);
AudioConnection          patchCord4(mix_delay_impulse, biquad2);
AudioConnection          patchCord5(mix_delay_impulse, biquad3);
AudioConnection          patchCord6(mix_delay_impulse, biquad4);
AudioConnection          patchCord7(delay_impulse, 0, mix_delay_impulse, 1);
AudioConnection          patchCord8(biquad1, 0, mix_filter, 0);
AudioConnection          patchCord9(biquad2, 0, mix_filter, 1);
AudioConnection          patchCord10(biquad3, 0, mix_filter, 2);
AudioConnection          patchCord11(biquad4, 0, mix_filter, 3);
AudioConnection          patchCord12(mix_filter, 0, mix_reverb, 0);
AudioConnection          patchCord13(mix_filter, reverb);
AudioConnection          patchCord14(reverb, 0, mix_reverb, 1);
AudioConnection          patchCord15(mix_reverb, 0, i2s, 0);
AudioConnection          patchCord16(mix_reverb, 0, i2s, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=1278,212
// GUItool: end automatically generated code



#include <Bounce.h>
#include "AudioSampleImpulse_x2.h"
//#include "Coefficcients.h"

double Coefficcients[20][5] = {
  //A
  {1.0, 0.0, 0.0, -1.9842, 0.9915},
  {1.0, 0.0, 0.0, -1.9682, 0.9901},
  {1.0, 0.0, 0.0, -1.8833, 0.9844},
  {1.0, 0.0, 0.0, -1.8634, 0.9830},
  //E
  {1.0, 0.0, 0.0, -1.9911, 0.9943},
  {1.0, 0.0, 0.0, -1.9359, 0.9887},
  {1.0, 0.0, 0.0, -1.8708, 0.9859},
  {1.0, 0.0, 0.0, -1.8273, 0.9830},
  //I
  {1.0, 0.0, 0.0, -1.9902, 0.9915},
  {1.0, 0.0, 0.0, -1.9258, 0.9873},
  {1.0, 0.0, 0.0, -1.8511, 0.9859},
  {1.0, 0.0, 0.0, -1.7987, 0.9830},
  //O
  {1.0, 0.0, 0.0, -1.9911, 0.9943},
  {1.0, 0.0, 0.0, -1.9773, 0.9887},
  {1.0, 0.0, 0.0, -1.8708, 0.9859},
  {1.0, 0.0, 0.0, -1.8485, 0.9830},
  //U
  {1.0, 0.0, 0.0, -1.9918, 0.9943},
  {1.0, 0.0, 0.0, -1.9814, 0.9887},
  {1.0, 0.0, 0.0, -1.8708, 0.9859},
  {1.0, 0.0, 0.0, -1.8407, 0.9830},
};

#define NUMNOTES   7
#define MIDIOFFSET 60

int digitalPins[] = {0, 1, 2, 3, 4, 5, 9, 14, 16, 17, 22, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33};
//float notes[NUMNOTES] = {0, 2, 4, 5, 7, 9, 11};
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
elapsedMillis last_time;



void setup() {

  //Vars init
  currentVowel = 0;
  last_time = millis();


  //Serial init
  Serial.begin(9600);


  //Teensy audio init
  AudioMemory(160);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.85);
  mix_delay_impulse.gain(0, 0.025);
  mix_delay_impulse.gain(1, 0);
  setCoeffs(currentVowel);
  mix_filter.gain(0, 0.1);
  mix_filter.gain(1, 0.1);
  mix_filter.gain(2, 0.1);
  mix_filter.gain(3, 0.1);
  mix_reverb.gain(0, 0.9); // 90% dry
  mix_reverb.gain(1, 0.1); // 10% wet
  reverb.roomsize(0.6);
  reverb.damping(0.2);

  //Hardware init
  for (int f = 0; f < NUMNOTES; f++) {
    pinMode(digitalPins[f], INPUT_PULLUP);
  }
  pinMode(CHANGE_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  delay(1000);

}



void loop() {

  for (int f = 0; f < NUMNOTES ; f++) {

    butNote[f].update();

    if (butNote[f].risingEdge()) {
      mix_delay_impulse.gain(1, 0.7);
      digitalWrite(LED_PIN, LOW);
    }

    if (butNote[f].fallingEdge()) {

//      Serial.print("f=");
//      Serial.print(f);
//      Serial.print("     notes[f]=");
//      Serial.println(MIDIOFFSET + notes[f]);
      float period = butToPeriod(MIDIOFFSET + notes[f]);
      delay_impulse.delay(0, period);
      mix_delay_impulse.gain(1, 0.99);
      impulse.play(AudioSampleImpulse_x2);

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

  if (last_time > 5000) {
    Serial.print("Proc = ");
    Serial.print(AudioProcessorUsage());
    Serial.print(" (");
    Serial.print(AudioProcessorUsageMax());
    Serial.print("),  Mem = ");
    Serial.print(AudioMemoryUsage());
    Serial.print(" (");
    Serial.print(AudioMemoryUsageMax());
    Serial.println(")");
    last_time = 0;
  }

}



float butToPeriod(float midiVal) {

  float hz = 440.0 * pow(2, (midiVal - 69.0) / 12.0);
  float period = 1000.0 / hz;

  Serial.print("Midi: ");
  Serial.print(midiVal);
  Serial.print("     Freq: ");
  Serial.print(hz);
  Serial.print("hz    Period: ");
  Serial.print(period);
  Serial.println("ms");
  return period;
}

//
//float potToPeriod(int potValue) {
//  int midi = map(potValue, 0, 1023, 0, 24);
//  float hz = 110.0 * pow(1.059463094359295, midi);
//  float period = 10000.0 / (float)hz;
//  Serial.print("Period: ");
//  Serial.print(hz);
//  Serial.print("Hz / ");
//  Serial.print(period);
//  Serial.println("ms");
//  return period;
//}



void setCoeffs(int v) {
  biquad1.setCoefficients(0, Coefficcients[v * 4 + 0]);
  biquad2.setCoefficients(0, Coefficcients[v * 4 + 1]);
  biquad3.setCoefficients(0, Coefficcients[v * 4 + 2]);
  biquad4.setCoefficients(0, Coefficcients[v * 4 + 3]);
}
