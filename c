#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "----";
const char* password = "-----";

String botToken = "----";
String chatID = "----";

int sensorPin = 34;

// 🔧 Calibration
float multiplier = 5.0;

// 🔋 State
float lastVoltage = 0;
bool lowAlertSent = false;
bool charging = false;

// 🔋 Read Voltage
float getVoltage() {
  int value = analogRead(sensorPin);
  float vout = (value * 3.3) / 4095.0;
  return vout * multiplier;
}

// 🔋 Status
String getStatus(float voltage) {
  if (voltage > 4.0) return "Full 🔋";
  else if (voltage > 3.5) return "Medium ⚡";
  else return "Low ⚠️";
}

// 🔋 Percentage
int getPercentage(float voltage) {
  int percent = ((voltage - 3.0) / (4.2 - 3.0)) * 100;
  if (percent > 100) percent = 100;
  if (percent < 0) percent = 0;
  return percent;
}

// 📩 Send Message
void sendMessage(String message) {
  WiFiClientSecure client;
  client.setInsecure();

  HTTPClient http;

  message.replace(" ", "%20");
  message.replace("\n", "%0A");

  String url = "https://api.telegram.org/bot" + botToken +
               "/sendMessage?chat_id=" + chatID +
               "&text=" + message;

  http.begin(client, url);
  http.setTimeout(10000);

  int code = http.GET();
  Serial.println("HTTP: " + String(code));

  http.end();
}

// 📡 Telegram
void checkTelegram() {
  WiFiClientSecure client;
  client.setInsecure();

  HTTPClient http;

  String url = "https://api.telegram.org/bot" + botToken + "/getUpdates";

  http.begin(client, url);
  http.setTimeout(10000);

  int httpCode = http.GET();

  if (httpCode == 200) {
    String payload = http.getString();

    DynamicJsonDocument doc(4096);
    deserializeJson(doc, payload);

    if (doc["result"].size() > 0) {

      int last = doc["result"].size() - 1;
      String text = doc["result"][last]["message"]["text"];

      if (text == "/status") {

        float voltage = getVoltage();
        String status = getStatus(voltage);
        int percent = getPercentage(voltage);

        String reply = "🔋 Battery: " + String(voltage) + "V\n";
        reply += "Level: " + String(percent) + "% (" + status + ")";

        sendMessage(reply);
      }
    }
  }

  http.end();
}

// 🚨 Low Battery Alert
void checkLowBattery() {
  float voltage = getVoltage();

  if (voltage < 3.5 && !lowAlertSent) {
    sendMessage("⚠️ Battery LOW: " + String(voltage) + "V");
    lowAlertSent = true;
  }

  if (voltage > 3.7) {
    lowAlertSent = false;
  }
}

// ☀️ Charging Detection
void checkCharging() {
  float voltage = getVoltage();

  if (voltage > lastVoltage + 0.02 && !charging) {
    sendMessage("☀️ Charging Started");
    charging = true;
  }

  if (voltage < lastVoltage - 0.02 && charging) {
    sendMessage("🔌 Charging Stopped");
    charging = false;
  }

  lastVoltage = voltage;
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  Serial.println("Connected");
}

void loop() {
  checkTelegram();
  checkLowBattery();
  checkCharging();

  delay(5000);
}
