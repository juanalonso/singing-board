#include "synth_pulsetrain.h"
#include <Bounce.h>




#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthPulsetrain          impulse;       //xy=728.77783203125,230.55556742350262
AudioEffectEnvelope      env_impulse;      //xy=891.4445190429688,230.4444122314453
AudioFilterBiquad        biquad1;        //xy=1103.1112060546875,142.22219848632812
AudioFilterBiquad        biquad2;        //xy=1103.1112060546875,202.22219848632812
AudioFilterBiquad        biquad3;        //xy=1103.1112060546875,262.2221984863281
AudioFilterBiquad        biquad4;        //xy=1103.1112060546875,322.2221984863281
AudioMixer4              mix_filter;     //xy=1290.888916015625,229.9999542236328
AudioSynthWaveformSine   lfo;            //xy=1318.1111450195312,398.3333435058594
AudioEffectFreeverb      reverb;         //xy=1470.4444580078125,310.9999694824219
AudioEffectEnvelope      env_lfo;        //xy=1470.666748046875,399.4444580078125
AudioSynthWaveformDc     offset;         //xy=1472.666748046875,443.4444580078125
AudioMixer4              mix_lfo;        //xy=1653.666748046875,418.4444580078125
AudioMixer4              mix_reverb;     //xy=1661.5557861328125,248.00001525878906
AudioEffectMultiply      tremolo;        //xy=1832.444580078125,255.11105346679688
AudioOutputI2S           i2s;            //xy=1985.8890380859375,255.11106872558594
AudioConnection          patchCord1(impulse, env_impulse);
AudioConnection          patchCord2(env_impulse, biquad1);
AudioConnection          patchCord3(env_impulse, biquad2);
AudioConnection          patchCord4(env_impulse, biquad3);
AudioConnection          patchCord5(env_impulse, biquad4);
AudioConnection          patchCord6(biquad1, 0, mix_filter, 0);
AudioConnection          patchCord7(biquad2, 0, mix_filter, 1);
AudioConnection          patchCord8(biquad3, 0, mix_filter, 2);
AudioConnection          patchCord9(biquad4, 0, mix_filter, 3);
AudioConnection          patchCord10(mix_filter, 0, mix_reverb, 0);
AudioConnection          patchCord11(mix_filter, reverb);
AudioConnection          patchCord12(lfo, env_lfo);
AudioConnection          patchCord13(reverb, 0, mix_reverb, 1);
AudioConnection          patchCord14(env_lfo, 0, mix_lfo, 0);
AudioConnection          patchCord15(offset, 0, mix_lfo, 1);
AudioConnection          patchCord16(mix_lfo, 0, tremolo, 1);
AudioConnection          patchCord17(mix_reverb, 0, tremolo, 0);
AudioConnection          patchCord18(tremolo, 0, i2s, 0);
AudioConnection          patchCord19(tremolo, 0, i2s, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=1976.6668395996094,409.99993896484375
// GUItool: end automatically generated code




//OLIVIA
double Coefficcients[20][5] = {
  //A
  {1.0, 0.0, 0.0, -1.9619, 0.9915},
  {1.0, 0.0, 0.0, -1.9204, 0.9901},
  {1.0, 0.0, 0.0, -1.7174, 0.9844},
  {1.0, 0.0, 0.0, -1.3346, 0.9830},
  //E
  {1.0, 0.0, 0.0, -1.9847, 0.9943},
  {1.0, 0.0, 0.0, -1.8579, 0.9887},
  {1.0, 0.0, 0.0, -1.7480, 0.9859},
  {1.0, 0.0, 0.0, -1.3789, 0.9830},
  //I
  {1.0, 0.0, 0.0, -1.9912, 0.9943},
  {1.0, 0.0, 0.0, -1.8175, 0.9887},
  {1.0, 0.0, 0.0, -1.7318, 0.9859},
  {1.0, 0.0, 0.0, -1.4438, 0.9830},
  //O
  {1.0, 0.0, 0.0, -1.9854, 0.9943},
  {1.0, 0.0, 0.0, -1.9555, 0.9887},
  {1.0, 0.0, 0.0, -1.6965, 0.9859},
  {1.0, 0.0, 0.0, -1.4362, 0.9830},
  //U
  {1.0, 0.0, 0.0, -1.9922, 0.9943},
  {1.0, 0.0, 0.0, -1.9682, 0.9887},
  {1.0, 0.0, 0.0, -1.7448, 0.9859},
  {1.0, 0.0, 0.0, -1.4172, 0.9830},
};


//RANDOM GUY
//double Coefficcients[20][5] = {
//  //A
//  {1.0, 0.0, 0.0, -1.9842, 0.9915},
//  {1.0, 0.0, 0.0, -1.9682, 0.9901},
//  {1.0, 0.0, 0.0, -1.8833, 0.9844},
//  {1.0, 0.0, 0.0, -1.8634, 0.9830},
//  //E
//  {1.0, 0.0, 0.0, -1.9911, 0.9943},
//  {1.0, 0.0, 0.0, -1.9359, 0.9887},
//  {1.0, 0.0, 0.0, -1.8708, 0.9859},
//  {1.0, 0.0, 0.0, -1.8273, 0.9830},
//  //I
//  {1.0, 0.0, 0.0, -1.9902, 0.9915},
//  {1.0, 0.0, 0.0, -1.9258, 0.9873},
//  {1.0, 0.0, 0.0, -1.8511, 0.9859},
//  {1.0, 0.0, 0.0, -1.7987, 0.9830},
//  //O
//  {1.0, 0.0, 0.0, -1.9911, 0.9943},
//  {1.0, 0.0, 0.0, -1.9773, 0.9887},
//  {1.0, 0.0, 0.0, -1.8708, 0.9859},
//  {1.0, 0.0, 0.0, -1.8485, 0.9830},
//  //U
//  {1.0, 0.0, 0.0, -1.9918, 0.9943},
//  {1.0, 0.0, 0.0, -1.9814, 0.9887},
//  {1.0, 0.0, 0.0, -1.8708, 0.9859},
//  {1.0, 0.0, 0.0, -1.8407, 0.9830},
//};
#define NUMNOTES   7
#define MIDIOFFSET 58

int digitalPins[] = {0, 1, 2, 3, 4, 5, 9, 14, 16, 17, 22, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33};
float notes[NUMNOTES] = {0, 2, 3, 5, 7, 8, 10};

#define NOTE1_PIN   digitalPins[6]
#define NOTE2_PIN   digitalPins[5]
#define NOTE3_PIN   digitalPins[4]
#define NOTE4_PIN   digitalPins[3]
#define NOTE5_PIN   digitalPins[2]
#define NOTE6_PIN   digitalPins[1]
#define NOTE7_PIN   digitalPins[0]
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
  AudioMemory(80);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.75);
  setCoeffs(currentVowel);
  mix_filter.gain(0, 0.5);
  mix_filter.gain(1, 0.5);
  mix_filter.gain(2, 0.5);
  mix_filter.gain(3, 0.5);
  mix_reverb.gain(0, 0.9); // 90% dry
  mix_reverb.gain(1, 0.1); // 10% wet
  reverb.roomsize(0.6);
  reverb.damping(0.4);
  lfo.amplitude(0.1);
  lfo.frequency(4);
  offset.amplitude(0.5);
  env_lfo.delay(0);
  env_lfo.attack(2000);
  env_lfo.hold(0);
  env_lfo.decay(0);
  env_lfo.sustain(1);
  env_lfo.release(100);
  env_impulse.attack(15);
  env_impulse.release(150);


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

    //Note OFF
    if (butNote[f].risingEdge()) {
      env_impulse.noteOff();
      env_lfo.noteOff();
      digitalWrite(LED_PIN, LOW);
    }

    //Note ON
    if (butNote[f].fallingEdge()) {
      float freq = midiToFreq(MIDIOFFSET + notes[f]);
      impulse.frequency(freq);
      impulse.amplitude(1);
      env_lfo.noteOn();
      env_impulse.noteOn();
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


float midiToFreq(float midiVal) {

  float hz = 440.0 * pow(2, (midiVal - 69.0) / 12.0);
  float period = 1000.0 / hz;

  Serial.print("Midi: ");
  Serial.print(midiVal);
  Serial.print("     Freq: ");
  Serial.print(hz);
  Serial.print("hz    Period: ");
  Serial.print(period);
  Serial.println("ms");
  return hz;
}


void setCoeffs(int v) {
  biquad1.setCoefficients(0, Coefficcients[v * 4 + 0]);
  biquad2.setCoefficients(0, Coefficcients[v * 4 + 1]);
  biquad3.setCoefficients(0, Coefficcients[v * 4 + 2]);
  biquad4.setCoefficients(0, Coefficcients[v * 4 + 3]);
}
