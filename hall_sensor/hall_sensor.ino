#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioPlayMemory          impulse;        //xy=70,51
AudioMixer4              mix_delay_impulse; //xy=271,71
AudioEffectDelay         delay_impulse;  //xy=271,271
AudioFilterBiquad        biquad1;        //xy=514,71
AudioFilterBiquad        biquad2;        //xy=514,131
AudioFilterBiquad        biquad3;        //xy=514,191
AudioFilterBiquad        biquad4;        //xy=514,251
AudioMixer4              mix_filter;     //xy=711,90
AudioEffectChorus        chorus;         //xy=849,90
AudioEffectFreeverb      reverb;      //xy=960,251
AudioMixer4              mix_reverb;         //xy=1076,109
AudioOutputI2S           i2s;            //xy=1260,101
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
AudioConnection          patchCord12(mix_filter, chorus);
AudioConnection          patchCord13(chorus, 0, mix_reverb, 0);
AudioConnection          patchCord14(chorus, reverb);
AudioConnection          patchCord15(reverb, 0, mix_reverb, 1);
AudioConnection          patchCord16(mix_reverb, 0, i2s, 0);
AudioConnection          patchCord17(mix_reverb, 0, i2s, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=1251,175
// GUItool: end automatically generated code




#define CHORUS_DELAY_LENGTH (16*AUDIO_BLOCK_SAMPLES)
short delayline[CHORUS_DELAY_LENGTH];

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
elapsedMillis last_time;



void setup() {

  //Vars init
  currentVowel = 1;
  last_time = millis();


  //Serial init
  Serial.begin(9600);


  //Teensy audio init
  AudioMemory(160);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  mix_delay_impulse.gain(0, 0.5);
  mix_delay_impulse.gain(1, 0);
  setCoeffs(currentVowel);
  mix_filter.gain(0, 0.25);
  mix_filter.gain(1, 0.25);
  mix_filter.gain(2, 0.25);
  mix_filter.gain(3, 0.25);
  if (!chorus.begin(delayline, CHORUS_DELAY_LENGTH, 2)) {
    Serial.println("AudioEffectChorus - begin failed");
    while (1);
  }
  chorus.voices(0);
  mix_reverb.gain(0, 0.5); // 50% wet
  mix_reverb.gain(1, 0.5); // 50% dry
  reverb.roomsize(0.8);
  reverb.damping(0.1);

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
      mix_delay_impulse.gain(1, 0.7);
      digitalWrite(LED_PIN, LOW);
      chorus.voices(0);
    }

    if (butNote[f].fallingEdge()) {

      Serial.print("f=");
      Serial.print(f);
      Serial.print("     notes[f]=");
      Serial.println(MIDIOFFSET + notes[f]);
      float period = butToPeriod(MIDIOFFSET + notes[f]);
      delay_impulse.delay(0, period);
      mix_delay_impulse.gain(1, 0.99);
      impulse.play(AudioSampleImpulse);
      chorus.voices(2);

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
  Serial.print(" Period: ");
  Serial.print(hz);
  Serial.print("Hz / ");
  Serial.print(period);
  Serial.println("ms");
  return period;
}


float potToPeriod(int potValue) {
  int midi = map(potValue, 0, 1023, 0, 24);
  float hz = 110.0 * pow(1.059463094359295, midi);
  float period = 1000.0 / (float)hz;
  Serial.print("Period: ");
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
