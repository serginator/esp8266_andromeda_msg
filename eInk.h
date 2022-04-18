#include <SPI.h>
#include "epd1in54_V2.h"
#include "epdpaint.h"
#include <stdio.h>
#include "randomText.h"
#include "image.h"

// eInk stuff
/*  Pins to connect:
 *  BUSY  (purple)  -> D2 (GPIO 4)
 *  RST   (white)   -> D1 (GPIO 5)
 *  DC    (green)   -> D3 (GPIO 0)
 *  CS    (orange)  -> D8 (GPIO 15)
 *  CLK   (yellow)  -> D5 (GPIO 14)
 *  DIN   (blue)    -> D7 (GPIO 13)
 *  GND   (brown)   -> GND
 *  VCC   (grey)    -> 3V
 *
 *  Then, for the Deep Sleep, in NodeMCU RST should be wired to D0 (GPIO 16 / USER)
 */

//Epd epd;            // default Pins: Reset = 8, DC = 9, CS = 10, Busy 7
Epd epd(5, 0, SS, 4); // NodeMCU pins
unsigned char image[1024];
Paint paint(image, 0, 0);
#define COLORED     0
#define UNCOLORED   1

void initScreen() {
  Serial.println("e-Paper init and clear");
  epd.LDirInit();
}

void printImage() {
  long randomNumber = random(5);
  Serial.print("print " + String(randomNumber) + " image");
  switch (randomNumber) {
    case 0:
      epd.Display(image1_200x157);
      break;
    case 1:
      epd.Display(image2_200x157);
      break;
    case 2:
      epd.Display(image3_200x157);
      break;
    case 3:
      epd.Display(image4_200x157);
      break;
    case 4:
      epd.Display(image5_200x157);
      break;
    default:
      epd.Display(image1_200x157);
      break;
  }
}

void printMessage(String message) {
  epd.Clear();

  Serial.println("e-Paper paint");

  epd.HDirInit();
  printImage();

  // test to write full text
  paint.SetWidth(200);
  paint.SetHeight(40);
  paint.Clear(COLORED);
  paint.Clear(UNCOLORED);
  // CHECK THIS CODE
  if (message.length() > 24) {
    String aux;
    int l = message.length() / 24;
    for (int i = 0; i <= l; i++) {
      aux = message.substring(24 * i, 24 * (i+1));
      paint.DrawStringAt(5, 10 * i, aux.c_str(), &Font12, COLORED); // recuadro blanco con texto en negro
    }
  }
  epd.SetFrameMemory(paint.GetImage(), 0, 40, paint.GetWidth(), paint.GetHeight());
  epd.DisplayFrame();
}

void sendRandomMessage() {
  printMessage(getRandomText());
}

void sleepScreen() {
  // Serial.println("e-Paper clear and goto sleep");
  // epd.HDirInit();
  // epd.Clear();
  // epd.Sleep();
}
