#define TINY_GSM_MODEM_SIM800
#include <TinyGsmClient.h>
#include <SoftwareSerial.h>

// Your ThinkSpeak channel details
String channelId = "2381662";
String readApiKey = "YRCSLQV7MLIE2EQE";

// TinyGsmClient sim800l
#define APN "m2mautotronic"
#define SIM800_RX 10
#define SIM800_TX 11
int PIN_BUZZER = A5;
SoftwareSerial sim800_serial(SIM800_RX, SIM800_TX);
TinyGsm gsm_client(sim800_serial);
TinyGsmClient client(gsm_client);

void readFromThinkSpeak() {
  if (client.connected() && client.available()) {
    String line = client.readStringUntil('\r');
    Serial.println(line);
    if (line == "1") {
      digitalWrite(PIN_BUZZER, HIGH);
      Serial.println("Buzzer Nyala");
    } else if (line == "0") {
      digitalWrite(PIN_BUZZER, LOW);
      Serial.println("Buzzer Mati");
    } else {
      Serial.println("Bukan status normal");
    }
  }

  // Check if client is disconnected
  if (!client.connected() && !client.available()) {
    Serial.println();
    Serial.println("Disconnecting from ThinkSpeak...");
    client.stop();
    Serial.println("Disconnected from ThinkSpeak");

    // Delay before reconnecting
    delay(10000);

    // Reconnect to ThinkSpeak
    Serial.println("Reconnecting to ThinkSpeak...");
    if (client.connect("api.thingspeak.com", 80)) {
      Serial.println("Connected to ThinkSpeak");

      // Make a request for the last entry
      String getRequest = "GET /channels/" + channelId + "/fields/1/last.txt?api_key=" + readApiKey;
      client.println(getRequest);
      client.println("Host: api.thingspeak.com");
      client.println("Connection: close");
      client.println();
    } else {
      Serial.println("Failed to reconnect to ThinkSpeak");
    }
  }
}

void setup() {
  Serial.begin(9600);
  sim800_serial.begin(9600);
  pinMode(PIN_BUZZER, OUTPUT);
  gsm_client.init();
  gsm_client.waitForNetwork();
  gsm_client.gprsConnect(APN, "", "");

  Serial.println("Connecting to ThinkSpeak...");
  if (client.connect("api.thingspeak.com", 80)) {
    Serial.println("Connected to ThinkSpeak");

    // Make a request for the last entry
    String getRequest = "GET /channels/" + channelId + "/fields/1/last.txt?api_key=" + readApiKey;
    client.println(getRequest);
    client.println("Host: api.thingspeak.com");
    client.println("Connection: close");
    client.println();
  } else {
    Serial.println("Failed to connect to ThinkSpeak");
  }
}

void loop() {
  // Call the readFromThinkSpeak function
  readFromThinkSpeak();
}
