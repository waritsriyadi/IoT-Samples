#include <TinyGPS++.h>  //https://github.com/mikalhart/TinyGPSPlus
#include <WiFi.h>
#include <HTTPClient.h>

//NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
//enter WIFI credentials
const char* ssid = "Meh";
const char* password = "PlkQas132";
//NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN


//NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
//enter domain name and path
//http://www.example.com/gpsdata.php
const char* SERVER_NAME = "http://motorcylealertsystem.000webhostapp.com/gpsdata.php";
const char* thingspeakApiKey = "OV1HKGD2JYS96CIG";

//ESP32_API_KEY is the exact duplicate of, ESP32_API_KEY in config.php file
//Both values must be same
String ESP32_API_KEY = "Ad5F10jkBM0";
//NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN


//NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
//Send an HTTP POST request every 30 seconds
unsigned long previousMillis = 0;
long interval = 30000;
//NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN


//NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
//GSM Module RX pin to ESP32 2
//GSM Module TX pin to ESP32 4
#define rxPin 23 // Dibalik
#define txPin 22
HardwareSerial neogps(1);

TinyGPSPlus gps;
//NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN


void setup() {

  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  Serial.begin(115200);
  Serial.println("esp32 serial initialize");

  neogps.begin(9600, SERIAL_8N1, rxPin, txPin);
  Serial.println("neogps serial initialize");
  delay(1000);
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM


  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
}

void loop() {
  smartdelay_gps(1000);
  //NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
  //Check WiFi connection status
  if (WiFi.status() == WL_CONNECTED) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis > interval) {
      previousMillis = currentMillis;
      //Send an HTTP POST request every 30 seconds
      sendGpsToServer();
    }
  }
  //NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
  else {
    Serial.println("WiFi Disconnected");
  }
  //NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN

  delay(5000);
}

int sendGpsToServer() {
  //NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
  //Can take up to 60 seconds
  boolean newData = false;
  for (unsigned long start = millis(); millis() - start < 2000;) {
    while (neogps.available()) {
      if (gps.encode(neogps.read())) {
        if (gps.location.isValid() == 1) {
          newData = true;
          break;
        }
      }
    }
  }
  //NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN

  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  //If newData is true
  if (true) {
    newData = false;

    //NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
    String latitude, longitude;
    //float altitude;
    //unsigned long date, time, speed, satellites;

    latitude = String(gps.location.lat(), 6);   // Latitude in degrees (double)
    longitude = String(gps.location.lng(), 6);  // Longitude in degrees (double)

    //altitude = gps.altitude.meters(); // Altitude in meters (double)
    //date = gps.date.value(); // Raw date in DDMMYY format (u32)
    //time = gps.time.value(); // Raw time in HHMMSSCC format (u32)
    //speed = gps.speed.kmph();

    //Serial.print("Latitude= ");
    //Serial.print(latitude);
    //Serial.print(" Longitude= ");
    //Serial.println(longitude);
    //NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN

    //NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
    HTTPClient http;

    //domain name with path
    http.begin(SERVER_NAME);

    //Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    //----------------------------------------------
    //HTTP POST request data
    String gps_data;
    gps_data = "api_key=" + ESP32_API_KEY;
    gps_data += "&lat=" + latitude;
    gps_data += "&lng=" + longitude;

    Serial.print("gps_data: ");
    Serial.println(gps_data);
    //----------------------------------------------

    //----------------------------------------------
    //Send HTTP POST Request and get response
    int httpResponseCode = http.POST(gps_data);
    String httpResponseString = http.getString();
    //----------------------------------------------

    //--------------------------------------------------------------------------------
    // If you need an HTTP request with a content type: text/plain
    //http.addHeader("Content-Type", "text/plain");
    //int httpResponseCode = http.POST("Hello, World!");

    // If you need an HTTP request with a content type: application/json, use the following:
    //http.addHeader("Content-Type", "application/json");
    //int httpResponseCode = http.POST("{\"value1\":\"19\",\"value2\":\"67\",\"value3\":\"78\"}");
    //--------------------------------------------------------------------------------

    //----------------------------------------------
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      Serial.println(httpResponseString);
    }
    //----------------------------------------------
    else {
      Serial.print("Error on HTTP request - Error code: ");
      Serial.println(httpResponseCode);
      Serial.println(httpResponseString);
    }
    //----------------------------------------------
    // Free resources
    http.end();

    //Start for ThingSpeak upload
    String apiKeyString = String(thingspeakApiKey);
    String field1 = "&field1=" + latitude;
    String field2 = "&field2=" + longitude;

    // Create the URL for sending data to ThingSpeak
    String url = "GET /update?api_key=" + apiKeyString + field1 + field2;

    // Send HTTP request to ThingSpeak
    WiFiClient client;
    if (client.connect("api.thingspeak.com", 80)) {
      client.println(url);
      client.println("Host: api.thingspeak.com");
      client.println("Connection: close");
      client.println();
      delay(3000);  // Give time for the server to respond
      client.stop();
    } else {
      Serial.println("Failed to connect to ThingSpeak");
    }
  }
  //NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
}

static void smartdelay_gps(unsigned long ms) {
  unsigned long start = millis();
  do {
    while (neogps.available())
      gps.encode(neogps.read());
  } while (millis() - start < ms);
}
//end if(true)
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM