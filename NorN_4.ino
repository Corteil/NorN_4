#include "SPI.h"
#include "Adafruit_WS2801.h"
#include <Adafruit_NeoPixel.h>
#include <Wire.h>



#define PIN 12
#define SLAVE_ADDRESS 0x04

Adafruit_NeoPixel bar = Adafruit_NeoPixel(16, PIN, NEO_GRB + NEO_KHZ800);

// i2c

int number = 0;

// Internal

uint8_t dataPin  = 9;
uint8_t clockPin = 8;
Adafruit_WS2801 strip = Adafruit_WS2801(1, dataPin, clockPin);

// Cylon settings

int wait_T=40; //This is the delay between moving back and forth and per pixel
int PixelCount=16; //Set this to the AMOUNT of Led's/Pixels you have or want to use on your strip And It can be used to tell where to Stop then return the eye at in the strip
int Pixel_Start_End=0; //Set this to where you want it to Start/End at
boolean UsingBar = true; //Set this to true If you are using the 8x1 Neopixel Bar Or you want to only use 3 leds for the scanner. 


void setup() {
  // put your setup code here, to run once:

  bar.begin();
  bar.setBrightness(64);
  bar.show(); // Initialize all pixels to 'off'
  strip.begin();

  // Update LED contents, to start they are all 'off'
  strip.show();

  // i2c start up

  Serial.begin(9600);         // start serial for output
  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);

  // define callbacks for i2c communication
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);

  Serial.println("Ready!");



}

void loop() {
  // put your main code here, to run repeatedly:
  selectColourPatten();
  delay(100);

  /*
  colorWipeStrip(Color(255, 255, 255), 1);
   colorWipeBar(bar.Color(0, 255, 0), 1);
   delay(500);
   colorWipeStrip(Color(0 ,0 ,0), 1);
   colorWipeBar(bar.Color(0, 0, 0), 1);
   delay(500);
   
   */

}

void colorWipeStrip(uint32_t c, uint8_t wait) {
  int i;

  for (i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
void colorWipeBar(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < bar.numPixels(); i++) {
    bar.setPixelColor(i, c);
    bar.show();
    delay(wait);
  }
}

void colorWipeBar2(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < bar.numPixels(); i++) {
    bar.setPixelColor(i, c);
    bar.show();
    delay(wait);
  }
}

void computing(){

  for (int i = 0; i < bar.numPixels(); i++) {
    int onOff = random(0,2);
    Serial.print(onOff);
    if (onOff) {
      bar.setPixelColor(i, bar.Color(255,255,255));
    }
    else {
      bar.setPixelColor(i, bar.Color(0,0,0));
    }
    bar.show();

  }
}



// Cylon scrobe
void cylonScrobe() {
  //Example: CylonEyeUp(Center_Dot_Color, Second_Dot_color, Third_Dot_color, wait_T, PixelCount, Pixel_Start_End);
  CylonEyeUp(bar.Color(175,0,0), bar.Color(25,0,0), bar.Color(10,0,0), wait_T, PixelCount, Pixel_Start_End);
  delay(wait_T);
  //Example: CylonEyeDown(Center_Dot_Color, Second_Dot_color, Third_Dot_color, wait_T, PixelCount, Pixel_Start_End);
  CylonEyeDown(bar.Color(175,0,0), bar.Color(25,0,0), bar.Color(10,0,0), wait_T, PixelCount, Pixel_Start_End);
  delay(wait_T);



}
void CylonEyeUp(uint32_t Co, uint32_t Ct, uint32_t Ctt, uint8_t Delay, int TotalPixels, int pStart) {
  for(int i=pStart; i<TotalPixels; i++) {
    if(!UsingBar) { 
      bar.setPixelColor(i+2, Ctt); 
    } //Third Dot Color
    bar.setPixelColor(i+1, Ct);   //Second Dot Color
    bar.setPixelColor(i, Co);     //Center Dot Color
    bar.setPixelColor(i-1, Ct);   //Second Dot Color
    if(!UsingBar) { 
      bar.setPixelColor(i-2, Ctt); 
    } //Third Dot Color

    if(!UsingBar) {
      bar.setPixelColor(i-3, bar.Color(0,0,0)); //Clears the dots after the 3rd color
    } 
    else {
      bar.setPixelColor(i-2, bar.Color(0,0,0)); //Clears the dots after the 2rd color
    }
    bar.show();
    //Serial.println(i); //Used For pixel Count Debugging
    delay(Delay);
  }
}
void CylonEyeDown(uint32_t Co, uint32_t Ct, uint32_t Ctt, uint8_t Delay, int TotalPixels, int pEnd) {
  for(int i=TotalPixels-1; i>pEnd; i--) {
    if(!UsingBar) { 
      bar.setPixelColor(i-2, Ctt); 
    } //Third Dot Color
    bar.setPixelColor(i-1, Ct);   //Second Dot Color
    bar.setPixelColor(i, Co);    //Center Dot Color
    bar.setPixelColor(i+1, Ct);  //Second Dot Color
    if(!UsingBar) { 
      bar.setPixelColor(i+2, Ctt); 
    } //Third Dot Color

    if(!UsingBar) { 
      bar.setPixelColor(i+3, bar.Color(0,0,0)); //Clears the dots after the 3rd color
    } 
    else {
      bar.setPixelColor(i+2, bar.Color(0,0,0)); //Clears the dots after the 2rd color
    }
    bar.show();
    //Serial.println(i); //Used For pixel Count Debugging
    delay(Delay);
  }
}

/* Helper functions */

// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b)
{
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}
void selectColourPatten(){

  if (number == 1) {
    while (number == 1) {
      colorWipeStrip(Color(0, 0, 0), 1);
      colorWipeBar(bar.Color(0, 0, 0), 1);
    }
  }

  if (number == 2) {

    while (number == 2) {
      colorWipeStrip(Color(255, 255, 255), 1);
      colorWipeBar(bar.Color(255, 255, 255), 1);
    }
  }

if (number == 3) {
  while (number == 3) {
    colorWipeStrip(Color(255, 255, 255), 1);
    colorWipeBar(bar.Color(255, 255, 255), 1);
    delay(500);
    colorWipeStrip(Color(0, 0, 0), 1);
    colorWipeBar(bar.Color(0, 0, 0), 1);
    delay(500);

  }
}
if (number == 4) {
  while (number == 4) {
    delay(500);
    colorWipeStrip(Color(255, 0, 0), 1);
    colorWipeBar(bar.Color(255, 0, 0), 1);
    delay(500);
    colorWipeStrip(Color(255, 255, 0), 1);
    colorWipeBar(bar.Color(255, 255, 0), 1);
    delay(500);
    colorWipeStrip(Color(0, 255, 0), 1);
    colorWipeBar(bar.Color(0, 255, 0), 1);
    delay(500);
    colorWipeStrip(Color(0, 255, 255), 1);
    colorWipeBar(bar.Color(0, 255, 255), 1);
    delay(500);
    colorWipeStrip(Color(0, 0, 255), 1);
    colorWipeBar(bar.Color(0, 0, 255), 1);
    delay(500);
    colorWipeStrip(Color(255, 255, 255), 1);
    colorWipeBar(bar.Color(255, 255, 255), 1);
    delay(500);
    colorWipeStrip(Color(255, 0, 255), 1);
    colorWipeBar(bar.Color(255, 0, 255), 1);

  }
}
if (number == 5) {
  while (number == 5) {
    delay(500);
    colorWipeStrip(Color(255, 0, 0), 1);
    colorWipeBar(bar.Color(0, 0, 0), 1);
    delay(500);
    colorWipeStrip(Color(255, 255, 0), 1);
    colorWipeBar(bar.Color(0, 0, 0), 1);
    delay(500);
    colorWipeStrip(Color(0, 255, 0), 1);
    colorWipeBar(bar.Color(0, 0, 0), 1);
    delay(500);
    colorWipeStrip(Color(0, 255, 255), 1);
    colorWipeBar(bar.Color(0, 0, 0), 1);
    delay(500);
    colorWipeStrip(Color(0, 0, 255), 1);
    colorWipeBar(bar.Color(0, 0, 0), 1);
    delay(500);
    colorWipeStrip(Color(255, 255, 255), 1);
    colorWipeBar(bar.Color(0, 0, 0), 1);
    delay(500);
    colorWipeStrip(Color(255, 0, 255), 1);
    colorWipeBar(bar.Color(0, 0, 0), 1);

  }
}
if (number == 6) {
  while (number == 6) {
    colorWipeStrip(Color(255, 128, 128), 1);
    colorWipeBar(bar.Color(255, 128, 128), 1);
    delay(500);
    colorWipeStrip(Color(255, 131, 0), 1);
    colorWipeBar(bar.Color(255, 1316, 0), 1);
    delay(500);
  }
}
if (number == 7) {

  while (number == 7) {
    colorWipeStrip(Color(255, 0, 0), 1);
    colorWipeBar(bar.Color(255, 0, 0), 1);
  }
}
if (number == 8) {

  while (number == 8) {
    colorWipeStrip(Color(0, 255, 0), 1);
    colorWipeBar(bar.Color(0, 255, 0), 1);

  }
}

if (number == 9) {

  while (number == 9) {
    colorWipeStrip(Color(0, 0, 255), 1);
    colorWipeBar(bar.Color(0, 0, 255), 1);
  }
}

if (number == 10) {
  while (number ==10){
    cylonScrobe();
  }
}
if (number == 11) {
  while (number == 11) {
    colorWipeBar(bar.Color(0, 0, 0), 1);
  }
}
if (number == 12) {
  while (number == 12) {
    colorWipeBar(bar.Color(0, 0, 255), 1);
  }
}
if (number == 13) {
  while (number == 13) {
    colorWipeBar2(bar.Color(255, 255, 255), 1);
    delay(250);
    colorWipeBar2(bar.Color(0, 0, 0), 1);
    delay(250);
  }
}
if (number == 14) {
  while (number == 14) {
    computing();
    delay(250);

  }



}
}
// callback for received data
void receiveData(int byteCount) {

  while (Wire.available()) {
    number = Wire.read();
    Serial.print("data received: ");
    Serial.println(number);
  }
}
// callback for sending data
void sendData() {
  Wire.write(number);
}







