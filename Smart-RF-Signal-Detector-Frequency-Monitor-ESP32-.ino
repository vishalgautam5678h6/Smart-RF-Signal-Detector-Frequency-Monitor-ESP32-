
#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ===== WiFi Details =====
const char* ssid = "Airtel_vish_1456";
const char* password = "Air@57014";

// ===== Pin Definitions =====
int rfReceiverPin = 4;
int rfTransmitterPin = 18;
int ledPin = 2;

// ===== LCD Setup =====
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ===== Web Server =====
WiFiServer server(80);

void setup() {

  Serial.begin(115200);

  pinMode(rfReceiverPin, INPUT_PULLDOWN);
  pinMode(rfTransmitterPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("RF Monitor");
  delay(2000);
  lcd.clear();

  // WiFi Connect
  WiFi.begin(ssid, password);
  lcd.setCursor(0,0);
  lcd.print("Connecting WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  lcd.clear();
  lcd.print("WiFi Connected");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {

  // Send test signal from transmitter
  digitalWrite(rfTransmitterPin, HIGH);
  delay(200);
  digitalWrite(rfTransmitterPin, LOW);

  int signal = digitalRead(rfReceiverPin);

  WiFiClient client = server.available();

  if (signal == HIGH) {

    digitalWrite(ledPin, HIGH);

    lcd.setCursor(0,0);
    lcd.print("Signal Detected ");
    
  } else {

    digitalWrite(ledPin, LOW);
  

    lcd.setCursor(0,0);
    lcd.print("No Signal ");
  }

  // ===== Web Page =====
  if (client) {

    String html = "<!DOCTYPE html><html>";
    html += "<head><title>RF Monitor</title></head>";
    html += "<body><h1>ESP32 RF Monitor</h1>";

    if(signal == HIGH){
      html += "<h2 style='color:red;'>Signal Detected</h2>";
    } else {
      html += "<h2 style='color:green;'>No Signal</h2>";
    }

    html += "</body></html>";

    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println();
    client.println(html);
    client.println();
    client.stop();
  }
} 