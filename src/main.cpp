#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <UniversalTelegramBot.h>  // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <WiFiClientSecure.h>

#include "const.h"
#include "db.h"

#define TELEGRAM_DEBUG 1

X509List cert(TELEGRAM_CERTIFICATE_ROOT);

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

int botRequestDelay = 1; // milliseconds
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

        if (text[0] != '!')
            return;

        String from_name = bot.messages[i].from_name;
        String from_id = bot.messages[i].from_id;


        String* params = new String[10];
        uint8 params_size = 0;
        String alt = "";
        for (unsigned int j = 0; j< text.length(); j++){
            if (text[j] != ' '){
                alt+=text[j];
            }else if (alt != ""){
                params[params_size] = alt; 
                params_size++;
                alt = "";
                // if (sizeof(params) == params_size ) {
                //     String 
                // }
            }
        }
        if (alt != ""){
            params[params_size] = alt; 
            params_size++;
        }

        for (unsigned int j = 0; j< params_size; j++){
            Serial.print(params[j]);
            Serial.print(" ");
        }
        Serial.println();


        if (text == "!ola") {
            String welcome = "Ola, " + from_name + "!\n";
            bot.sendMessage(chat_id, from_id, "");
        }

        if (params[0] == "!create" && params_size == 2) {
            Person p = {.caps = 0};
            memccpy(p.id, from_id.c_str(), 0, 10);
            memccpy(p.name, &params[1], 0, params[1].length());

            Person new_p = addPerson(p);

            String personStringfy = "name: " + String(new_p.name) + "\nCaps: " + String(new_p.caps);
            bot.sendMessage(chat_id, personStringfy , "");
        }

        if (params[0] == "!add" && params_size == 2) {
            int q = addCaps(atoi(params[1].c_str()),(char*)from_id.c_str());
            if (q == -1){
                bot.sendMessage(chat_id, "Usuário não criado. Use -> !create [apelido]", "");
            }else {
                bot.sendMessage(chat_id, "Agora você tem: " + String(q) + " Capsulas", "");
            }
        }
    }
}

void setup() {
    Serial.begin(115200);

#ifdef ESP8266
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

// int numNewMessages = 0;

void loop() {
    if (millis() > lastTimeBotRan + botRequestDelay) {
        // Serial.println("opa");
        int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

        while (numNewMessages > 0) {
            Serial.println("got response");
            handleNewMessages(numNewMessages);
            numNewMessages = bot.getUpdates(bot.last_message_received + 1);
        }
        lastTimeBotRan = millis();
    }
    // Serial.println(numNewMessages);
}
