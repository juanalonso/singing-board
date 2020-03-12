#include <Bounce.h>
#include "AudioSampleGrain_a.h"
#include "AudioSampleGrain_e.h"
#include "AudioSampleGrain_i.h"
#include "AudioSampleGrain_o.h"
#include "AudioSampleGrain_u.h"
#include "AudioSampleImpulse.h"

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioPlayMemory          playMem1;       //xy=107,195
AudioMixer4              mixer1;         //xy=776,209
AudioEffectDelay         delay1;         //xy=780,410
AudioOutputI2S           i2s1;           //xy=992,202
AudioConnection          patchCord1(playMem1, 0, mixer1, 0);
AudioConnection          patchCord2(mixer1, 0, i2s1, 0);
AudioConnection          patchCord3(mixer1, 0, i2s1, 1);
AudioConnection          patchCord4(mixer1, delay1);
AudioConnection          patchCord5(delay1, 0, mixer1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=547,519
// GUItool: end automatically generated code



Bounce button0 = Bounce(0, 15);
Bounce button1 = Bounce(1, 15);
Bounce button2 = Bounce(2, 15);

int a0history, a2history;



int currentVowel = 0;
const unsigned int *vowels[] = {
  AudioSampleGrain_a,
  AudioSampleGrain_e,
  AudioSampleGrain_i,
  AudioSampleGrain_o,
  AudioSampleGrain_u,
  AudioSampleImpulse,
};

const float gains[] = {
  0.75, 0.56, 0.75, 0.56, 0.40, 0.40
};



void setup() {

  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);

  float initGain = potToGain(analogRead(A0));
  float initPeriod = potToPeriod(analogRead(A2));

  Serial.begin(115200);

  AudioMemory(160);

  sgtl5000_1.enable();
  sgtl5000_1.volume(initGain);

  mixer1.gain(0, gains[currentVowel]); //Main gain
  mixer1.gain(1, 0);    //Delay gain, controled by button
  mixer1.gain(2, 0);

  delay1.delay(0, initPeriod);

  delay(1000);

}



void loop() {

  int a0 = analogRead(A0);
  if (a0 > a0history + 15 || a0 < a0history - 15) {
    float gain = potToGain(a0history);
    sgtl5000_1.volume(gain);
    a0history = a0;
  }

  int a2 = analogRead(A2);
  if (a2 > a2history + 15 || a2 < a2history - 15) {
    float period = potToPeriod(a2);
    delay1.delay(0, period);
    a2history = a2;
  }

  button0.update();
  button1.update();
  button2.update();

  if (button0.fallingEdge()) {
    playMem1.play(vowels[currentVowel]);
    mixer1.gain(1, 0.99);
  }

  if (button0.risingEdge()) {
    mixer1.gain(1, 0);
  }

  if (button1.fallingEdge()) {
    currentVowel++;
    if (currentVowel >= 6) {
      currentVowel = 0;
    }
    mixer1.gain(0, gains[currentVowel]);
  }


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

float potToGain(int potValue) {
  float potGain = (float)map(potValue, 0, 1023, 0, 800) / 1000.0;
  Serial.print("Gain: ");
  Serial.println(potGain);
  return potGain;
}
