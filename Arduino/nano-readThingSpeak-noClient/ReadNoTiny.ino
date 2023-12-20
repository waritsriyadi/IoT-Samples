#include <SoftwareSerial.h>
#include <ArduinoJson.h>

SoftwareSerial sim800l(10, 11);  // RX, TX

String url = "api.thingspeak.com";
String writeAPIKey = "YRCSLQV7MLIE2EQE";
unsigned long lastCheckTime = 0;  // The last time the ThingSpeak data was checked
unsigned long checkInterval = 2000;

String data1 = "";  // Store the field1 data here

void setup() {
  Serial.begin(9600);
  sim800l.begin(9600);
}

void loop() {
  unsigned long currentTime = millis();  // Get the current time

  if (currentTime - lastCheckTime >= checkInterval) {
    lastCheckTime = currentTime;  // Update the last check time
    connectToGPRS();              // Check the ThingSpeak data
  }
}

void connectToGPRS() {
  sim800l.println("AT+CGATT=1");
  delay(2000);

  sim800l.println("AT+CGDCONT=1,\"IP\",\"YRCSLQV7MLIE2EQE\"");
  delay(5000);

  while (sim800l.available()) {
    sim800l.read();
  }

  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += url;
  cmd += "\",80";

  sim800l.println(cmd);
  delay(2000);

  String getRequest = "GET /channels/2381662/feeds/last.json?api_key=YRCSLQV7MLIE2EQE HTTP/1.1";
  getRequest += "\r\nHost: ";
  getRequest += url;
  getRequest += "\r\nConnection: close";
  getRequest += "\r\n\r\n";

  sim800l.print("AT+CIPSEND=");
  sim800l.println(getRequest.length());
  delay(1000);
  sim800l.print(getRequest);

  String response = "";
  unsigned long timeout = millis() + 5000;  // 5 seconds

  while (millis() < timeout) {
    while (sim800l.available()) {
      char c = sim800l.read();
      response += c;
    }
  }
  // Find the field1 data
  int startPos = response.indexOf("field1\":\"") + 9;
  int endPos = response.indexOf("\"", startPos);

  if (startPos > 0 && endPos > 0) {
    data1 = response.substring(startPos, endPos);
    Serial.print("Data1: ");
    Serial.println(data1);
  }
}