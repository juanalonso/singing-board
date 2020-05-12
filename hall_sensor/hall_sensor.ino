#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthKarplusStrong  string;        //xy=254,425
AudioOutputI2S           output;           //xy=581,430
AudioConnection          patchCord1(string, 0, output, 0);
AudioConnection          patchCord2(string, 0, output, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=79,180
// GUItool: end automatically generated code



#include <Bounce.h>

#define NUMNOTES   7
#define MIDIOFFSET 12

int digitalPins[] = {0, 1, 2, 3, 4, 5, 9, 14, 16, 17, 22, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33};
int notes[NUMNOTES] = {10, 8, 7, 5, 3, 2, 0};

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



void setup() {

  Serial.begin(9600);

  AudioMemory(160);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.75);

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
      digitalWrite(LED_PIN, LOW);
    }

    if (butNote[f].fallingEdge()) {

      float period = butToHz(MIDIOFFSET + notes[f]);
      string.noteOn(period, 0.75);

      digitalWrite(LED_PIN, HIGH);
      break;
    }
  }

  butChange.update();

  if (butChange.risingEdge()) {
  }

  if (butChange.fallingEdge()) {
  }

}


float butToHz(int butValue) {

  float hz = 110.0 * pow(1.059463094359295, butValue);
  float period = 1000.0 / (float)hz;

  Serial.print("Midi: ");
  Serial.print(butValue + MIDIOFFSET);
  Serial.print(" Period: ");
  Serial.print(hz);
  Serial.print("Hz / ");
  Serial.print(period);
  Serial.println("ms");
  return hz;
}
