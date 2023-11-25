#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
//---------------------------------------------------------------------------------------------------------------
// WiFi credentials
const char* ssid = "Test";
const char* password = "123456789";
const char* host = "192.168.137.32";
const int ledPin = 25;
const int buttonPin = 27; // Change as per your setup
bool ledState = false;
//---------------------------------------------------------------------------------------------------------------
WebServer server(80);
//---------------------------------------------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
//---------------------------------------------------------------------------------------------------------------
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());
//---------------------------------------------------------------------------------------------------------------


server.on("/turnOnLED", []() {
    digitalWrite(ledPin, HIGH);
    server.send(200, "text/plain", "LED is ON");
  });
//---------------------------------------------------------------------------------------------------------------
  server.on("/turnOffLED", []() {
    digitalWrite(ledPin, LOW);
    server.send(200, "text/plain", "LED is OFF");
  });
//---------------------------------------------------------------------------------------------------------------
  server.begin();
//---------------------------------------------------------------------------------------------------------------
}

void loop() {
  int buttonState = digitalRead(buttonPin);
//---------------------------------------------------------------------------------------------------------------
  // ตรวจสอบว่าปุ่มถูกกด
  if (buttonState == HIGH) {
    ledState = !ledState; // สลับสถานะ LED
//---------------------------------------------------------------------------------------------------------------
    WiFiClient client;
    if (!client.connect(host, 80)) {
      Serial.println("Connection to ESP-32(2) failed");
      return;
    }
//---------------------------------------------------------------------------------------------------------------
    // ส่งคำขอตามสถานะ LED
    if (ledState) {
      client.print(String("GET /turnOnLED HTTP/1.1\r\n") +
                   "Host: " + host + "\r\n" +
                   "Connection: close\r\n\r\n");
    } /*else {
      client.print(String("GET /turnOffLED HTTP/1.1\r\n") +
                   "Host: " + host + "\r\n" +
                   "Connection: close\r\n\r\n");
    }*/
//---------------------------------------------------------------------------------------------------------------
    if (client.connect(host, 80)) {
    String message = "Hello OLED"; // ข้อความที่ต้องการส่ง
    client.print(String("GET /display?message=") + message + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
    }
  //---------------------------------------------------------------------------------------------------------------
  server.handleClient();
  //---------------------------------------------------------------------------------------------------------------
}

}