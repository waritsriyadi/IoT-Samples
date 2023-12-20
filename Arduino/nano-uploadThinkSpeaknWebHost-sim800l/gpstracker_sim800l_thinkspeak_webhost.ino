// THIS PROJECT WAS MADE BY github.com/waritsriyadi

#include <SoftwareSerial.h>
#include <TinyGPS++.h>

SoftwareSerial sim800lSerial(10, 11);  // RX, TX for SIM800L
SoftwareSerial gpsSerial(3, 2);        // RX, TX for GPS
TinyGPSPlus gps;

const char* SERVER_NAME = "YOUR_WEBHOST000_WEB"; // Example: motorcylealertsystem.000webhostapp.com
const int SERVER_PORT = 80;
const String API_PATH = "/gpsdata.php";
const char* thingspeakApiKey = "YOUR_API_KEY";

void setup() {
  Serial.begin(9600);
  sim800lSerial.begin(9600);
  gpsSerial.begin(9600);

  sendATcommand("AT", "OK", 2000);
  sendATcommand("AT+CMGF=1", "OK", 2000);

  delay(1000);
}

void loop() {
  Serial.println("Finding GPS...");
  smartdelay_gps(1000);
  String latitude, longitude, speedKmph;

  if (gps.location.isValid()) {
    latitude = String(gps.location.lat(), 6);
    longitude = String(gps.location.lng(), 6);
    speedKmph = String(gps.speed.kmph(), 6);

    Serial.print("Latitude: ");
    Serial.println(latitude);
    Serial.print("Longitude: ");
    Serial.println(longitude);
    Serial.print("Speed: ");
    Serial.print(speedKmph);
    Serial.println(" km/h");

    uploadToThingSpeak(latitude, longitude, speedKmph);
    uploadToWebhost(latitude, longitude);
  } 
  delay(15000);  // Sesuaikan delay ini berdasarkan kebutuhan Anda
}


void uploadToThingSpeak(const String& latitude, const String& longitude, const String& speedKmph) {
  Serial.println("Uploading to ThingSpeak...");
  sim800lSerial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");
  delay(500);
  ShowSerialData();

  sim800lSerial.println("AT+CIPSEND");
  delay(500);
  ShowSerialData();

  String str = "GET /update?api_key=" + String(thingspeakApiKey) +
               "&field1=" + latitude +
               "&field2=" + longitude +
               "&field3=" + speedKmph;
  sim800lSerial.println(str);
  delay(1000);
  ShowSerialData();

  sim800lSerial.println((char)26);  // Sending
  delay(5000);
  ShowSerialData();

  sim800lSerial.println("AT+CIPSHUT");  // Close the connection
  delay(100);
  ShowSerialData();
  Serial.println("Done Uploading to ThingSpeak...");
}

void uploadToWebhost(const String& latitude, const String& longitude) {
  Serial.println("Uploading to Webhost000..");
  String url, temp;
  url = "http://YOUR_WEBHOST000_WEB/gpsdata.php?lat="; // Example: http://motorcylealertsystem.000webhostapp.com/gpsdata.php?lat=
  url += latitude;
  url += "&lng=";
  url += longitude;

  delay(300);

  sendATcommand("AT+CFUN=1", "OK", 2000);
  sendATcommand("AT+CGATT=1", "OK", 2000);
  sendATcommand("AT+SAPBR=3,1,\"Contype\",\"GPRS\"", "OK", 2000);
  sendATcommand("AT+SAPBR=3,1,\"APN\",\"YOUR_APN_HERE_DONT_FORGET_THIS\"", "OK", 2000);
  sendATcommand("AT+SAPBR=1,1", "OK", 2000);
  sendATcommand("AT+HTTPINIT", "OK", 2000);
  sendATcommand("AT+HTTPPARA=\"CID\",1", "OK", 1000);
  sim800lSerial.print("AT+HTTPPARA=\"URL\",\"");
  sim800lSerial.print(url);
  sendATcommand("\"", "OK", 1000);
  sendATcommand("AT+HTTPACTION=0", "0,200", 1000);
  sendATcommand("AT+HTTPTERM", "OK", 1000);
  sendATcommand("AT+CIPSHUT", "SHUT OK", 1000);
  Serial.println("Done Uploading to Webhost000..");
}

void ShowSerialData() {
  while (sim800lSerial.available() != 0)
    Serial.write(sim800lSerial.read());
  delay(5000);
}

static void smartdelay_gps(unsigned long ms) {
  unsigned long start = millis();
  do {
    while (gpsSerial.available())
      gps.encode(gpsSerial.read());
  } while (millis() - start < ms);
}

void sendATcommand(const char* command, const char* expectedResponse, int timeout) {
  sim800lSerial.println(command);
  unsigned long start = millis();

  while (millis() - start < timeout) {
    if (sim800lSerial.find(const_cast<char*>(expectedResponse))) {
      return;
    }
  }
}

// THIS PROJECT WAS MADE BY github.com/waritsriyadi
