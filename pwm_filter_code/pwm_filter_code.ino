#include <Bounce.h>

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthWaveformDc     dc1;            //xy=139,238
AudioSynthWaveformPWM    pwm1;           //xy=244,72
AudioFilterBiquad        biquad1;        //xy=443,72
AudioFilterBiquad        biquad2;        //xy=443,147
AudioFilterBiquad        biquad3;        //xy=446,245
AudioMixer4              mixer1;         //xy=682,93
AudioOutputI2S           i2s1;           //xy=891,89
AudioConnection          patchCord1(dc1, pwm1);
AudioConnection          patchCord2(pwm1, biquad1);
AudioConnection          patchCord3(pwm1, biquad2);
AudioConnection          patchCord4(pwm1, biquad3);
AudioConnection          patchCord5(biquad1, 0, mixer1, 0);
AudioConnection          patchCord6(biquad2, 0, mixer1, 1);
AudioConnection          patchCord7(biquad3, 0, mixer1, 2);
AudioConnection          patchCord8(mixer1, 0, i2s1, 0);
AudioConnection          patchCord9(mixer1, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=710.25,206.5
// GUItool: end automatically generated code





Bounce button0 = Bounce(0, 15);
Bounce button1 = Bounce(1, 15);
Bounce button2 = Bounce(2, 15);

int a0history, a2history;



void setup() {

  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);

  //float initFilter = potToGain(analogRead(A0));
  float initFreq = potToFreq(analogRead(A2));

  Serial.begin(115200);

  AudioMemory(30);

  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);

  dc1.amplitude(-0.9);

  pwm1.amplitude(0.75);
  pwm1.frequency(initFreq);

  double coef1[5] = {1.0, 0.0, 0.0, -1.9842, 0.9915};
  biquad1.setCoefficients(0, coef1);
  
  double coef2[5] = {1.0, 0.0, 0.0, -1.9682, 0.9901};
  biquad2.setCoefficients(0, coef2);
  
  double coef3[5] = {1.0, 0.0, 0.0, -1.8833, 0.9844};
  biquad3.setCoefficients(0, coef3);

  mixer1.gain(0,0.5);
  mixer1.gain(1,0.5);
  mixer1.gain(2,0.5);

  delay(1000);

}



void loop() {

//  int a0 = analogRead(A0);
//  if (a0 > a0history + 15 || a0 < a0history - 15) {
//    //float gain = potToGain(a0history);
//    float freq = potToFreq(a0);
//    biquad1.setBandpass(0, freq, 0.707);
//    a0history = a0;
//  }

  int a2 = analogRead(A2);
  if (a2 > a2history + 15 || a2 < a2history - 15) {
    float freq = potToFreq(a2);
    pwm1.frequency(freq);
    a2history = a2;
  }

  button0.update();
  button1.update();
  button2.update();

  if (button0.fallingEdge()) {
    //playMem1.play(vowels[currentVowel]);
    //mixer1.gain(1, 0.99);
  }

  if (button0.risingEdge()) {
    //mixer1.gain(1, 0);
  }

  if (button1.fallingEdge()) {
    //    currentVowel++;
    //    if (currentVowel >= 6) {
    //      currentVowel = 0;
    //    }
    //    mixer1.gain(0, gains[currentVowel]);
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

float potToFreq(int potValue) {
  int midi = map(potValue, 0, 1023, 0, 24);
  float hz = 110.0 * pow(1.059463094359295, midi);
  float period = 1000.0 / (float)hz;
  Serial.print("Period: ");
  Serial.print(hz);
  Serial.print("Hz / ");
  Serial.print(period);
  Serial.println("ms");
  return hz;
}

float potToGain(int potValue) {
  float potGain = (float)map(potValue, 0, 1023, 0, 800) / 1000.0;
  Serial.print("Gain: ");
  Serial.println(potGain);
  return potGain;
}
