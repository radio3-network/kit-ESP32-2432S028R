#include <Arduino.h>
#include <WiFi.h>
#include "AudioFileSourceSD.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"

AudioFileSourceSD *file;
AudioGeneratorMP3 *mp3;
AudioFileSourceSD *source;
AudioOutputI2S *out;
AudioFileSourceID3 *id3;
SPIClass SDSPI(VSPI);

void setup() {
  Serial.begin(115200);
  Serial.println();
  WiFi.mode(WIFI_OFF);
  delay(500);

  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH);
  SDSPI.begin(18, 19, 23); // SDSPI.begin(SCLK, MISO, MOSI);
  SDSPI.setFrequency(1000000);
  SD.begin(5, SDSPI);
}

void playMP3(char *filename) {
  file = new AudioFileSourceSD(filename);
  id3 = new AudioFileSourceID3(file);
  out = new AudioOutputI2S(0, 2, 8, -1); // Output to builtInDAC
  out->SetOutputModeMono(true);
  out->SetGain(1.0);
  mp3 = new AudioGeneratorMP3();
  mp3->begin(id3, out);
  while (mp3->isRunning()) {
    if (!mp3->loop()) mp3->stop();
  }
}


void loop() {
  playMP3("/Olsen-Banden.mp3");
  delay(100);
}
