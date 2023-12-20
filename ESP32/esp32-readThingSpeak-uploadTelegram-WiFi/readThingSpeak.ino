#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char *ssid = "Meh";
const char *password = "PlkQas132";
const char *thinkSpeakChannelId = "2374052";
const char *thinkSpeakApiKey = "OV1HKGD2JYS96CIG";
const char *telegramBotToken = "6822136070:AAFgKHq_yOyuLC9n5_Eq_n9u1PZ9MmTnDQo";
const char *telegramChatId = "6289052732";
int botRequestDelay = 100;
unsigned long lastTimeBotRan;
float currentLat = 0.0;
float currentLng = 0.0;

void readDataFromThingSpeak() {
  // Make a GET request to ThingSpeak
  HTTPClient http;
  String url = "http://api.thingspeak.com/channels/" + String(thinkSpeakChannelId) + "/feeds/last.json?api_key=" + String(thinkSpeakApiKey);
  http.begin(url);

  int httpCode = http.GET();
  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println("Received data from ThingSpeak:");
      Serial.println(payload);

      // Parse JSON
      const size_t capacity = JSON_OBJECT_SIZE(10);
      DynamicJsonDocument doc(capacity);

      DeserializationError error = deserializeJson(doc, payload);
      if (error) {
        Serial.println("Failed to parse JSON");
        return;
      }

      // Extract latitude and longitude
      currentLat = doc["field1"].as<float>();
      currentLng = doc["field2"].as<float>();
    } else {
      Serial.println("Error accessing ThingSpeak. HTTP code: " + String(httpCode));
    }
  } else {
    Serial.println("Error connecting to ThingSpeak");
  }

  http.end();
}

void sendTelegramMessage(float latitude, float longitude) {
  // Send latitude and longitude to Telegram
  HTTPClient http;
  String url = "https://api.telegram.org/bot" + String(telegramBotToken) + "/sendMessage";
  String message = "Latitude: " + String(latitude, 6) + "\nLongitude: " + String(longitude, 6);
  String locationMessage = "Lokasi : https://www.google.com/maps/@" + String(latitude, 6) + "," + String(longitude, 6) + ",21z?entry=ttu";
  String payload = "chat_id=" + String(telegramChatId) + "&text=" + message;
  String payload2 = "chat_id=" + String(telegramChatId) + "&text=" + locationMessage;
  
  http.begin(url);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  int httpCode = http.POST(payload);
  http.POST(payload2);
  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK) {
      String response = http.getString();
      Serial.println("Telegram API Response:");
      Serial.println(response);
    } else {
      Serial.println("Error sending message to Telegram. HTTP code: " + String(httpCode));
    }
  } else {
    Serial.println("Error connecting to Telegram API");
  }

  http.end();
}

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Read data from ThingSpeak
  readDataFromThingSpeak();
}

void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay) {
    // Send latitude and longitude to Telegram
    readDataFromThingSpeak();
    sendTelegramMessage(currentLat, currentLng);
    lastTimeBotRan = millis();
  }
  delay(30000);
}
