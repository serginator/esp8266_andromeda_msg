#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

// Wifi network station credentials
#define WIFI_SSID "<ssid>"
#define WIFI_PASSWORD "<the_password>"
#define BOT_TOKEN "<token_for_the_bot>"
#define CHAT_ID "<the_chat_id_to_send_messages>"

const unsigned long BOT_MTBS = 1000; // mean time between scan messages
unsigned long bot_lasttime; // last time messages' scan has been done

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

void initWifi() {
  // attempt to connect to Wifi network:
  configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
  secured_client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());

  // Check NTP/Time, usually it is instantaneous and you can delete the code below.
  Serial.print("Retrieving time: ");
  time_t now = time(nullptr);
  while (now < 24 * 3600) {
    Serial.print(".");
    delay(100);
    now = time(nullptr);
  }
  Serial.println(now);
  bot.sendMessage(CHAT_ID, "NodeMCU Started!!!", "");
}

void handleNewMessages(int numNewMessages) {
  Serial.print("handleNewMessages ");
  Serial.println(numNewMessages);

  for (int i = 0; i < numNewMessages; i++) {

    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (from_name == "")
      from_name = "Guest";

    if(text.indexOf("/send") != -1) {
      text.remove(0, 6);
      Serial.print("El mensaje que quieres enviar es: ");
      Serial.println(text);
      // write on eInk
      printMessage(text);

      bot.sendMessage(chat_id, "Mensaje enviado correctamente", "");
    }

    if (text == "/start") {
      String welcome = "Welcome to Andromeda Messenger for Arduino, " + from_name + ".\n\n";
      welcome += "/send <msg> : Sends message to NodeMCU unit\n";
      bot.sendMessage(chat_id, welcome, "Markdown");
    }
  }
}
