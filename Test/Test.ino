#include <WiFi.h>
#include <ESPAsyncWebServer.h>

// Replace with your network credentials
const char* ssid = "ggalaxy_2.4G";
const char* password = "021146pop";

const int LEDPIN = 25; // Change to your LED pin

AsyncWebServer server(80);

const char* index_html = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP LED Control</title>
  <script>
    function toggleLED(state) {
      var xhr = new XMLHttpRequest();
      xhr.open("GET", "/" + state, true);
      xhr.send();
    }
  </script>
</head>
<body>
  <h1>ESP LED Control</h1>
  <button onclick="toggleLED('on')">Turn On</button>
  <button onclick="toggleLED('off')">Turn Off</button>
</body>
</html>
)rawliteral";


void setup() {
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, LOW);
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  // Print the IP address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html); // index_html is your HTML content
  });

  // Route to turn LED on
  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request) {
    digitalWrite(LEDPIN, HIGH);
    request->send(200, "text/plain", "LED is ON");
  });

  // Route to turn LED off
  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request) {
    digitalWrite(LEDPIN, LOW);
    request->send(200, "text/plain", "LED is OFF");
  });

  // Start server
  server.begin();
}

void loop() {
  // Handle server
}
