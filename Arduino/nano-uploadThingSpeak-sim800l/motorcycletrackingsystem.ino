#include <SoftwareSerial.h>
#include <TinyGPS++.h>

SoftwareSerial sim800lSerial(10, 11);  // RX, TX for SIM800L
SoftwareSerial gpsSerial(3, 2);      // RX, TX for GPS
TinyGPSPlus gps;

const char* SERVER_NAME = "motorcylealertsystem.000webhostapp.com";
const int SERVER_PORT = 80;
const String API_PATH = "/gpsdata.php";
const char* thingspeakApiKey = "OV1HKGD2JYS96CIG";

void setup() {
  Serial.begin(115200);
  sim800lSerial.begin(9600);
  gpsSerial.begin(9600);
  Serial.println("Initializing...");

  sendATcommand("AT", "OK", 2000);
  sendATcommand("AT+CMGF=1", "OK", 2000);

  delay(1000);
}

void loop() {
  smartdelay_gps(1000);
  String latitude, longitude;

  if (gps.location.isValid()) {
    latitude = String(gps.location.lat(), 6);
    longitude = String(gps.location.lng(), 6);
    Serial.print("Latitude: ");
    Serial.println(latitude);
    Serial.print("Longitude: ");
    Serial.println(longitude);

    uploadToThingSpeak(latitude, longitude);
    uploadToWebhost(latitude, longitude);
  }
  delay(15000);  // Adjust this delay based on your requirements
}

void uploadToThingSpeak(const String& latitude, const String& longitude) {
  Serial.println("Uploading to ThingSpeak...");

  sim800lSerial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");
  delay(500);
  ShowSerialData();

  sim800lSerial.println("AT+CIPSEND");
  delay(500);
  ShowSerialData();

  String str = "GET /update?api_key=" + String(thingspeakApiKey) + "&field1=" + latitude + "&field2=" + longitude;
  Serial.println(str);
  sim800lSerial.println(str);
  delay(1000);
  ShowSerialData();

  sim800lSerial.println((char)26);  // Sending
  delay(5000);
  ShowSerialData();

  sim800lSerial.println("AT+CIPSHUT");  // Close the connection
  delay(100);
  ShowSerialData();
}

void uploadToWebhost(const String& latitude, const String& longitude) {
  Serial.println("Uploading to webhost000...");

  String url, temp;
  url = "http://motorcylealertsystem.000webhostapp.com/gpsdata.php?lat=";
  url += latitude;
  url += "&lng=";
  url += longitude;

  Serial.println(url);
  delay(300);

  sendATcommand("AT+CFUN=1", "OK", 2000);
  //AT+CGATT = 1 Connect modem is attached to GPRS to a network. AT+CGATT = 0, modem is not attached to GPRS to a network
  sendATcommand("AT+CGATT=1", "OK", 2000);
  //Connection type: GPRS - bearer profile 1
  sendATcommand("AT+SAPBR=3,1,\"Contype\",\"GPRS\"", "OK", 2000);
  //sets the APN settings for your network provider.
  sendATcommand("AT+SAPBR=3,1,\"APN\",\"m2mautotronic\"", "OK", 2000);
  //enable the GPRS - enable bearer 1
  sendATcommand("AT+SAPBR=1,1", "OK", 2000);
  //Init HTTP service
  sendATcommand("AT+HTTPINIT", "OK", 2000);
  sendATcommand("AT+HTTPPARA=\"CID\",1", "OK", 1000);
  //Set the HTTP URL sim800.print("AT+HTTPPARA="URL","http://ahmadssd.000webhostapp.com/gpsdata.php?lat=222&lng=222"\r");
  sim800lSerial.print("AT+HTTPPARA=\"URL\",\"");
  sim800lSerial.print(url);
  sendATcommand("\"", "OK", 1000);
  //Set up the HTTP action
  sendATcommand("AT+HTTPACTION=0", "0,200", 1000);
  //Terminate the HTTP service
  sendATcommand("AT+HTTPTERM", "OK", 1000);
  //shuts down the GPRS connection. This returns "SHUT OK".
  sendATcommand("AT+CIPSHUT", "SHUT OK", 1000);
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
  delay(500);
}