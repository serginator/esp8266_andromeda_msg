/*******************************************************************
    This project aims to read messages from a Telegram Bot and
    print them in a eInk display. I want to make a gift to my
    girlfriend where I can send her a message

    Parts:
    NodeMCU v3 ESP8266 board
    Waveshare 1.54" e-Paper
    Battery?

    Written by serginator <@serginator>
 *******************************************************************/

// eInk methods
#include "eInk.h"
// Wifi and Telegram stuff
#include "net.h"

void setup() {
  Serial.begin(115200);
  Serial.println();
  randomSeed(analogRead(0));

  initScreen();
  initWifi();
}

void loop() {
  if (millis() - bot_lasttime > BOT_MTBS) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    if (numNewMessages > 0) {
      while (numNewMessages) {
        Serial.println("got response");
        handleNewMessages(numNewMessages);
        numNewMessages = bot.getUpdates(bot.last_message_received + 1);
      }
    } else {
      sendRandomMessage();
    }

    bot_lasttime = millis();

    // 3600e6 = 1h
    // 86400e6 = 24h
    ESP.deepSleep(3600e6); // sleep for 3600 seconds
  }
}
