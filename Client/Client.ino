#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "ggalaxy_2.4G";
const char* password = "021146pop";
const char* hostESP32_1 = "192.168.1.111"; // IP Address ของ ESP-32(1)
const int ledPin = 25; // Change as per your setup

WebServer server(80);

String getHtmlPage() {
  String strHtml = "<html><head>";
  strHtml += "<style>";
  strHtml += ".button {background-color: #990033; border: none; border-radius: 4px; color: white; padding: 7px 15px; text-align: center; text-decoration: none;}";
  strHtml += "</style>";
  strHtml += "<title>Controller</title>";
  strHtml += "</head>";
  strHtml += "<body><h1>Controller</h1>";
  strHtml += "<h3>LED/Switch</h3>";
  strHtml += "<a href='/on' class='button'>ON</a><br><br>";
  strHtml += "<a href='/off' class='button'>OFF</a>";
  strHtml += "</body></html>";
  return strHtml;
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi ");
  Serial.println(WiFi.localIP());


 server.on("/", []() {
    server.send(200, "text/html", getHtmlPage());
  });

  server.on("/on", []() {
    // ส่งคำขอไปยัง ESP-32(1) เพื่อเปิด LED
    WiFiClient client;
    if (client.connect("192.168.1.111", 80)) {
      client.print("GET /turnOnLED HTTP/1.1\r\nHost: 192.168.1.111\r\nConnection: close\r\n\r\n");
    }
    server.send(200, "text/plain", "Sent ON command to ESP-32(1)");
  });

  server.on("/off", []() {
    // ส่งคำขอไปยัง ESP-32(1) เพื่อปิด LED
    WiFiClient client;
    if (client.connect("192.168.1.111", 80)) {
      client.print("GET /turnOffLED HTTP/1.1\r\nHost: 192.168.1.111\r\nConnection: close\r\n\r\n");
    }
    server.send(200, "text/plain", "Sent OFF command to ESP-32(1)");
  });

  server.on("/turnOnLED", []() {
    digitalWrite(ledPin, HIGH);
    server.send(200, "text/plain", "LED is ON");
  });

  server.on("/turnOffLED", []() {
    digitalWrite(ledPin, LOW);
    server.send(200, "text/plain", "LED is OFF");
  });

  server.begin();
}

void loop() {
  server.handleClient();
}
