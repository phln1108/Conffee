#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <UniversalTelegramBot.h>  // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <WiFiClientSecure.h>

#include "const.h"
// #include "db.h"

#define TELEGRAM_DEBUG 1


WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// Checks for new messages every 1 second.
int botRequestDelay = 0;
unsigned long lastTimeBotRan;

// Handle what happens when you receive new messages
void handleNewMessages(int numNewMessages) {
    Serial.print("handleNewMessages: ");
    Serial.println(String(numNewMessages));

    for (int i = 0; i < numNewMessages; i++) {
        // Chat id of the requester
        String chat_id = String(bot.messages[i].chat_id);
        if (chat_id != CHAT_ID) {
            bot.sendMessage(chat_id, ":D", "");
            continue;
        }

        // Print the received message
        String text = bot.messages[i].text;
        Serial.println(text);

        String from_name = bot.messages[i].from_name;
        String from_id = bot.messages[i].from_id;

        if (text == "/ola") {
            String welcome = "Ola, " + from_name + "!\n";
            bot.sendMessage(chat_id, welcome, "");
        }

        // if (text == "!create") {
        //   Person p = {
        //     .name = {'p','e','d','r','o'},
        //     .caps = 0,
        //     .id = from_id.toInt()
        //   };
        //   addPerson(p);
        //   bot.sendMessage(chat_id, "teste", "");
        // }

        // if (text == "!add") {
        //   int q = addCaps(1,from_id.toInt());

        //   bot.sendMessage(chat_id, "Agora você tem: "+String(q)+" Capsulas", "");
        // }
    }
}

void setup() {
    Serial.begin(115200);

#ifdef ESP8266
    X509List cert(TELEGRAM_CERTIFICATE_ROOT);
    configTime(0, 0, "pool.ntp.org");  // get UTC time via NTP
    client.setTrustAnchors(&cert);     // Add root certificate for api.telegram.org
#endif

    // Connect to Wi-Fi
    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID, PASSWORD);

#ifdef ESP32
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT);  // Add root certificate for api.telegram.org
#endif

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }
    // Print ESP32 Local IP Address
    Serial.println(WiFi.localIP());
}

int numNewMessages = 0;

void loop() {
    if (millis() > lastTimeBotRan + botRequestDelay) {
        // Serial.println("opa");
        numNewMessages = bot.getUpdates(bot.last_message_received + 1);

        while (numNewMessages > 0) {
            Serial.println("got response");
            handleNewMessages(numNewMessages);
            numNewMessages = bot.getUpdates(bot.last_message_received + 1);
        }
        lastTimeBotRan = millis();
    }
    // Serial.println(numNewMessages);

}
