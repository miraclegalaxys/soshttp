#include <WiFi.h>
#include <WebServer.h>
#include <U8g2lib.h>
//---------------------------------------------------------------------------------------------------------------
const char* ssid = "Test";
const char* password = "123456789";
const char* hostESP32_1 = "192.168.137.72"; // IP Address ของ ESP-32(1)
const int ledPin = 25; // Change as per your setup
//---------------------------------------------------------------------------------------------------------------
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
//---------------------------------------------------------------------------------------------------------------

String getHtmlPage() {
  String strHtml = "<html><head>";
  strHtml += "<style>.button {background-color: #990033; border: none; border-radius: 4px; color: white; padding: 7px 15px; text-align: center; text-decoration: none;}</style>";
  strHtml += "<title>LED Controller</title></head><body><h1>LED Controller</h1><h3>Control the LED</h3>";
  strHtml += "<a href='/commandTurnOnLED' class='button'>Turn ON</a><br><br>";
  strHtml += "</body></html>";
  return strHtml;
}

//---------------------------------------------------------------------------------------------------------------
WebServer server(80);
//---------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------------
void displayMessage() {
  String message = server.arg("message"); // รับข้อความที่ส่งมา

  u8g2.clearBuffer();         
  u8g2.setFont(u8g2_font_ncenB08_tr);

  int y = 10; // ตำแหน่งเริ่มต้นของบรรทัดแรก
  for (int i = 0; i < message.length(); i++) {
    if (message[i] == '\n') { // หากพบการขึ้นบรรทัดใหม่
      y += 10; // เพิ่มตำแหน่ง y สำหรับบรรทัดถัดไป
      continue;
    }
    String line = message.substring(i, message.indexOf('\n', i));
    i += line.length();
    u8g2.drawStr(0, y, line.c_str()); // แสดงข้อความบนบรรทัดใหม่
    y += 10; // เตรียมตำแหน่งสำหรับบรรทัดถัดไป
  }

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

server.on("/commandTurnOnLED", []() {
  WiFiClient client;
  if (client.connect(hostESP32_1, 80)) {
    // ส่งคำขอ GET ไปที่ ESP-32(1)
    client.print("GET /turnOnLED HTTP/1.1\r\n" +
                 String("Host: ") + hostESP32_1 + "\r\n" + 
                 "Connection: close\r\n\r\n");
    client.stop();
    server.send(200, "text/plain", "Command sent to turn on LED");
  } else {
    server.send(500, "text/plain", "Failed to connect to ESP-32(1)");
  }
});
//---------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------------
  server.on("/turnOnLED", []() {
    digitalWrite(ledPin, HIGH);
    server.send(200, "text/plain", "LED is ON");
  });
//---------------------------------------------------------------------------------------------------------------

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
