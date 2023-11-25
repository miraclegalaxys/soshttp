#include <WiFi.h>
#include <WebServer.h>
#include <U8g2lib.h>
//---------------------------------------------------------------------------------------------------------------
const char* ssid = "Test";
const char* password = "123456789";
const char* hostESP32_1 = "192.168.137.74"; // IP Address ของ ESP-32(1)
const int ledPin = 25; // Change as per your setup
//---------------------------------------------------------------------------------------------------------------
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
//---------------------------------------------------------------------------------------------------------------
String host = String(hostESP32_1);
//---------------------------------------------------------------------------------------------------------------
WebServer server(80);
//---------------------------------------------------------------------------------------------------------------
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
//---------------------------------------------------------------------------------------------------------------
void displayMessage() {
  String message = server.arg("message"); // รับข้อความที่ส่งมา

  u8g2.clearBuffer();         
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0, 10, message.c_str()); // แสดงข้อความบน OLED
  u8g2.sendBuffer();

  server.send(200, "text/plain", "Message displayed: " + message);
}

//---------------------------------------------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  u8g2.begin();
  pinMode(ledPin, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi ");
  Serial.println(WiFi.localIP());
//---------------------------------------------------------------------------------------------------------------

 server.on("/", []() {
    server.send(200, "text/html", getHtmlPage());
  });
//---------------------------------------------------------------------------------------------------------------
  server.on("/on", []() {
    // ส่งคำขอไปยัง ESP-32(1) เพื่อเปิด LED
    WiFiClient client;
    if (client.connect(hostESP32_1, 80)) {
      client.print("GET /turnOnLED HTTP/1.1\r\nHost: " + host + "\r\nConnection: close\r\n\r\n");
    }
    server.send(200, "text/plain", "Sent ON command to ESP-32(1)");
  });
//---------------------------------------------------------------------------------------------------------------
  /*server.on("/off", []() {
    // ส่งคำขอไปยัง ESP-32(1) เพื่อปิด LED
    WiFiClient client;
    if (client.connect(hostESP32_1, 80)) {
      client.print("GET /turnOffLED HTTP/1.1\r\nHost: " + host + "\r\nConnection: close\r\n\r\n");
    }
    server.send(200, "text/plain", "Sent OFF command to ESP-32(1)");
  });*/
//---------------------------------------------------------------------------------------------------------------
  server.on("/turnOnLED", []() {
    digitalWrite(ledPin, HIGH);
    server.send(200, "text/plain", "LED is ON");
  });
//---------------------------------------------------------------------------------------------------------------
  /*server.on("/turnOffLED", []() {
    digitalWrite(ledPin, LOW);
    server.send(200, "text/plain", "LED is OFF");
  });*/
//---------------------------------------------------------------------------------------------------------------

server.on("/display", displayMessage); // ตั้งค่าเส้นทางสำหรับการแสดงข้อความ
 //---------------------------------------------------------------------------------------------------------------

  server.begin();
//---------------------------------------------------------------------------------------------------------------
}
void loop() {
  server.handleClient();

  //----------------------------------------------------------------------------------------------------


  //---------------------------------------------------------------------------------------------------------------
}
