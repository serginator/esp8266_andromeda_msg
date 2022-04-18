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
  long randSeed = time(nullptr);
  Serial.println("Random seed is: " + str(randSeed));
  randomSeed(randSeed);
  bot.sendMessage(CHAT_ID, "NodeMCU Started!!!", "");
}

void reconnectWifi() {
  Serial.print("Reconnecting...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nConnected");
}

void bot_setup() {
  const String commands = F("["
                            "{\"command\":\"help\", \"description\":\"Bot help\"},"
                            "{\"command\":\"start\", \"description\":\"Start bot\"},"
                            "]");
  bot.setMyCommands(commands);
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

    if (text == "/help") {
       String help = "Hello! Useful commands are: /help, /start, /send <message>";
       bot.sendMessage(chat_id, help, "Markdown");
    } else if (text == "/start") {
      String welcome = "Welcome to Andromeda Messenger for Arduino, " + from_name + ".\n\n";
      welcome += "/send <msg> : Sends message to NodeMCU unit\n";
      bot.sendMessage(chat_id, welcome, "Markdown");
    } else if(text.indexOf("/send") != -1) {
      text.remove(0, 6);
      Serial.print("The message you want to send is: ");
      Serial.println(text);
      // write on eInk
      printMessage(text);
      bot.sendMessage(chat_id, "Message sent", "");
      delay(5000);
    } else {
       String error = "I didn't understand the message '" + text + "'. Please write /help or /start to get info on how to use me.";
       bot.sendMessage(chat_id, error, "Markdown");
    }
  }
}
