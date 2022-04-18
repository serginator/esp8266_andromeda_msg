/*******************************************************************
    This project aims to read messages from a Telegram Bot and
    print them in a eInk display. I want to make a gift to my
    girlfriend where I can send her a message

    Parts:
    NodeMCU v3 ESP8266 board
    Waveshare 1.54" e-Paper

    Written by serginator <@serginator>
 *******************************************************************/

// eInk methods
#include "eInk.h"
// Wifi and Telegram stuff
#include "net.h"


void setup() {
  Serial.begin(115200);
  Serial.println();

  initScreen();
  initWifi();
  bot_setup();
}

void loop() {
  int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
  if (numNewMessages > 0) {
    while (numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
      delay(5000);
    }
  } else {
    sendRandomMessage();
  }
  // Time to deepSleep (max 71 minutes, 4260e6)
  // 3600e6 = 1h
  ESP.deepSleep(600e6); // 10 minutes
}
