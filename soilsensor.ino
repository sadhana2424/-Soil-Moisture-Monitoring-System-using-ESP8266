#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "Sadhana";
const char* password = "123456789";

ESP8266WebServer server(80);

int soilPin = A0;
int ledPin = D4;

void handleRoot() {

  int moisture = analogRead(soilPin);

  String statusText;
  String color;

  if (moisture > 700) {
    statusText = "DRY SOIL";
    color = "red";
    digitalWrite(ledPin, HIGH);
  }
  else if (moisture > 400) {
    statusText = "MODERATE";
    color = "orange";
    digitalWrite(ledPin, LOW);
  }
  else {
    statusText = "WET SOIL";
    color = "green";
    digitalWrite(ledPin, LOW);
  }

  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<meta http-equiv="refresh" content="3">
<title>Soil Monitoring System</title>

<style>
body{
font-family:Arial;
background:linear-gradient(135deg,#1abc9c,#3498db);
text-align:center;
padding-top:50px;
}

.card{
width:400px;
margin:auto;
background:white;
padding:30px;
border-radius:20px;
box-shadow:0 0 20px rgba(0,0,0,0.3);
}

h1{
color:#2c3e50;
}

.value{
font-size:40px;
font-weight:bold;
color:#2980b9;
}

.status{
font-size:30px;
font-weight:bold;
padding:10px;
border-radius:10px;
}

.footer{
margin-top:20px;
color:#555;
}
</style>

</head>
<body>

<div class="card">

<h1>🌱 Soil Moisture Monitor</h1>

<p>Sensor Value</p>

<div class="value">)rawliteral";

  html += moisture;

  html += R"rawliteral(</div>

<br>

<div class="status" style="color:)rawliteral";

  html += color;

  html += R"rawliteral(;">
)rawliteral";

  html += statusText;

  html += R"rawliteral(
</div>

<div class="footer">
ESP8266 Web Server
</div>

</div>

</body>
</html>
)rawliteral";

  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Connected");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();
}