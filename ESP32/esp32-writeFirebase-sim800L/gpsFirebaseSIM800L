//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <Wire.h>

SoftwareSerial SIM800(10, 11);   // RX, TX for SIM800L
SoftwareSerial gpsSerial(3, 2);  // RX, TX for GPS
TinyGPSPlus gps;
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM

void init_gsm();
void gprs_connect();
boolean gprs_disconnect();
boolean is_gprs_connected();
void post_to_firebase(String data);
void smartdelay_gps(unsigned long ms);
boolean waitResponse(String expected_answer = "OK", unsigned int timeout = 2000);

const String APN = "m2mautotronic";
const String USER = "";
const String PASS = "";

const String FIREBASE_HOST = "https://mushy-gps-default-rtdb.asia-southeast1.firebasedatabase.app/";
const String FIREBASE_SECRET = "KFKbzptOMS9LfGxTrKjA6bVA2TSN3a8LZajxavyg";

#define USE_SSL true
#define DELAY_MS 500

void setup() {
  Serial.begin(9600);
  SIM800.begin(9600);
  gpsSerial.begin(9600);

  SIM800.listen();
  Serial.println("Initializing SIM800...");
  init_gsm();
}

void loop() {
  gpsSerial.listen();
  String data = get_data("", "", "", "");
  Serial.println(data);
  SIM800.listen();
  if (!is_gprs_connected()) {
    gprs_connect();
  }
  post_to_firebase(data);
  delay(1000);
}

String get_data(String lat, String lng, String spd, String move) {
topFn:
  //Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  gpsSerial.listen();
  smartdelay_gps(1000);
  if (gps.location.isValid()) {
    lat = String(gps.location.lat(), 6);
    lng = String(gps.location.lng(), 6);
    spd = String(gps.speed.kmph(), 6);
    if (gps.speed.kmph() > 1) {
      move = "True";
    } else {
      move = "False";
    }
  }

  // Check if any reads failed and exit early (to try again).
  if (lat == "" || lng == "" || spd == "") {
    Serial.println(F("Failed to read DATA!"));
    goto topFn;
  }
  String Data = "{";
  Data += "\"lat\":\"" + lat + "\",";
  Data += "\"lng\":\"" + lng + "\",";
  Data += "\"spd\":\"" + spd + "\",";
  Data += "\"move\":\"" + move + "\"";
  Data += "}";
  return Data;
}

void post_to_firebase(String data) {
  SIM800.println("AT+HTTPINIT");
  waitResponse();
  delay(DELAY_MS);

  if (USE_SSL == true) {
    SIM800.println("AT+HTTPSSL=1");
    waitResponse();
    delay(DELAY_MS);
  }

  SIM800.println("AT+HTTPPARA=\"CID\",1");
  waitResponse();
  delay(DELAY_MS);

  SIM800.println("AT+HTTPPARA=\"URL\"," + FIREBASE_HOST + "location.json?auth=" + FIREBASE_SECRET);
  waitResponse();
  delay(DELAY_MS);

  SIM800.println("AT+HTTPPARA=\"REDIR\",1");
  waitResponse();
  delay(DELAY_MS);
  SIM800.println("AT+HTTPPARA=\"CONTENT\",\"application/json\"");
  waitResponse();
  delay(DELAY_MS);
  SIM800.println("AT+HTTPDATA=" + String(data.length()) + ",10000");
  waitResponse("DOWNLOAD");
  SIM800.println(data);
  waitResponse();
  delay(DELAY_MS);
  SIM800.println("AT+HTTPACTION=1");

  for (uint32_t start = millis(); millis() - start < 20000;) {
    while (!SIM800.available())
      ;
    String response = SIM800.readString();
    if (response.indexOf("+HTTPACTION:") > 0) {
      Serial.println(response);
      break;
    }
  }

  delay(DELAY_MS);
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  //+HTTPACTION: 1,603,0 (POST to Firebase failed)
  //+HTTPACTION: 0,200,0 (POST to Firebase successfull)
  //Read the response
  SIM800.println("AT+HTTPREAD");
  waitResponse("OK");
  delay(DELAY_MS);

  SIM800.println("AT+HTTPTERM");
  waitResponse("OK", 1000);
  delay(DELAY_MS);
  Serial.println("Done");
}

void init_gsm() {
  SIM800.println("AT");
  waitResponse();
  delay(DELAY_MS);

  SIM800.println("AT+CPIN?");
  waitResponse("+CPIN: READY");
  delay(DELAY_MS);

  SIM800.println("AT+CFUN=1");
  waitResponse();
  delay(DELAY_MS);

  SIM800.println("AT+CMEE=2");
  waitResponse();
  delay(DELAY_MS);

  SIM800.println("AT+CBATCHK=1");
  waitResponse();
  delay(DELAY_MS);

  SIM800.println("AT+CREG?");
  waitResponse("+CREG: 0,");
  delay(DELAY_MS);

  SIM800.print("AT+CMGF=1\r");
  waitResponse("OK");
  delay(DELAY_MS);
}

void gprs_connect() {
  SIM800.println("AT+SAPBR=0,1");
  waitResponse("OK", 60000);
  delay(DELAY_MS);
  SIM800.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
  waitResponse();
  delay(DELAY_MS);
  SIM800.println("AT+SAPBR=3,1,\"APN\"," + APN);
  waitResponse();
  delay(DELAY_MS);
  if (USER != "") {
    SIM800.println("AT+SAPBR=3,1,\"USER\"," + USER);
    waitResponse();
    delay(DELAY_MS);
  }
  if (PASS != "") {
    SIM800.println("AT+SAPBR=3,1,\"PASS\"," + PASS);
    waitResponse();
    delay(DELAY_MS);
  }
  SIM800.println("AT+SAPBR=1,1");
  waitResponse("OK", 30000);
  delay(DELAY_MS);
  SIM800.println("AT+SAPBR=2,1");
  waitResponse("OK");
  delay(DELAY_MS);
}

boolean gprs_disconnect() {
  SIM800.println("AT+CGATT=0");
  waitResponse("OK", 60000);
  return true;
}

boolean is_gprs_connected() {
  SIM800.println("AT+CGATT?");
  if (waitResponse("+CGATT: 1", 6000) == 1) { return false; }

  return true;
}

boolean waitResponse(String expected_answer, unsigned int timeout) {
  uint8_t x = 0, answer = 0;
  String response;
  unsigned long previous;

  while (SIM800.available() > 0)
    SIM800.read();
  previous = millis();
  do {
    if (SIM800.available() != 0) {
      char c = SIM800.read();
      response.concat(c);
      x++;
      if (response.indexOf(expected_answer) > 0) {
        answer = 1;
      }
    }
  } while ((answer == 0) && ((millis() - previous) < timeout));
  Serial.println(response);
  return answer;
}

static void smartdelay_gps(unsigned long ms) {
  unsigned long start = millis();
  do {
    while (gpsSerial.available())
      gps.encode(gpsSerial.read());
  } while (millis() - start < ms);
}
