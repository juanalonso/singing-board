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

#define NUMNOTES   3
#define MIDIOFFSET 12

int digitalPins[] = {0, 1, 2, 3, 4, 5, 9, 14, 16, 17, 22, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33};
int notes[NUMNOTES] = {0, 4, 7};

#define HALL_PIN1  digitalPins[7]
#define HALL_PIN2  digitalPins[8]
#define HALL_PIN3  digitalPins[9]
#define LED_PIN   LED_BUILTIN

Bounce butNote[NUMNOTES] = {
  Bounce(HALL_PIN1, 15),
  Bounce(HALL_PIN2, 15),
  Bounce(HALL_PIN3, 15)
};


void setup() {

  Serial.begin(9600);

  AudioMemory(160);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.75);

  for (int f = 0; f < NUMNOTES; f++) {
    pinMode(digitalPins[f + 7], INPUT_PULLUP);
  }
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

      float period = butToHz(MIDIOFFSET+notes[f]);
      string.noteOn(period, 0.75);

      digitalWrite(LED_PIN, HIGH);
      break;
    }
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
