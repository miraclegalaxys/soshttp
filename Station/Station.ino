#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
//---------------------------------------------------------------------------------------------------------------
// WiFi credentials
const char* ssid = "Test";
const char* password = "123456789";
const char* host = "192.168.137.185";
const int ledPin = 25;
const int buttonPin = 27; // Change as per your setup
//---------------------------------------------------------------------------------------------------------------
WebServer server(80);
//---------------------------------------------------------------------------------------------------------------

String getHtmlPage() {
  String strHtml = "<html><head>";
  strHtml += "<style>.button {background-color: #990033; border: none; border-radius: 4px; color: white; padding: 7px 15px; text-align: center; text-decoration: none;}</style>";
  strHtml += "<title>LED Controller</title></head><body><h1>LED Controller</h1><h3>Control the LED</h3>";
  strHtml += "<a href='/turnOnLED' class='button'>Turn ON</a><br><br>";
  strHtml += "</body></html>";
  return strHtml;
}


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
 server.on("/", []() {
    server.send(200, "text/html", getHtmlPage());
  });


server.on("/turnOnLED", []() {
    digitalWrite(ledPin, HIGH);
    server.send(200, "text/plain", "LED is ON");
  });
//---------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------------
  server.begin();
//---------------------------------------------------------------------------------------------------------------
}

void loop() {
    server.handleClient();

  int buttonState = digitalRead(buttonPin);
//---------------------------------------------------------------------------------------------------------------
  // ตรวจสอบว่าปุ่มถูกกด
  if (buttonState == HIGH) {
//---------------------------------------------------------------------------------------------------------------
        WiFiClient client;
    if (client.connect(host, 80)) {
      // ส่งคำขอ GET ไปยัง ESP-32(2) เพื่อเปิด LED
      client.print(String("GET /turnOnLED HTTP/1.1\r\n") +
                   "Host: " + host + "\r\n" +
                   "Connection: close\r\n\r\n");
      delay(100); // ให้เวลาสำหรับการส่งข้อมูล
      client.stop(); // ปิดการเชื่อมต่อ
    } else {
      Serial.println("Connection to ESP-32(2) failed");
    }
//---------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------------
   if (client.connect(host, 80)) {
    String line1 = WiFi.localIP().toString();
    String line2 = "HELP";
    String message = line1 + "%0A" + line2; // ใช้ %0A เพื่อแทนขึ้นบรรทัดใหม่

    client.print(String("GET /display?message=") + message + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
}
  //---------------------------------------------------------------------------------------------------------------

  
  //---------------------------------------------------------------------------------------------------------------
}

}