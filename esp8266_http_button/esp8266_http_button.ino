/*

OFF: http://192.168.0.111:51828/?accessoryId=switch1&state=false
ON: http://192.168.0.111:51828/?accessoryId=switch1&state=true

*/

#include <SPI.h>
#include <Wire.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
 
const char* ssid = "InScienceWeTrust";
const char* password = "kogdabudetborsch";
String payload = "NONE";

HTTPClient http;
 
int ledPin = 2;
int buttonPin = 0;

int ledState = 0;
int buttonState = 0;

WiFiServer server(80);

String get_request(String url) {
  http.begin(url);
  int httpCode = http.GET();
  String kek = "FAILED";
  if (httpCode > 0) {
    kek = http.getString();
  }
  http.end();
  return kek;
}
 
void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.print("Hi!");
 
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  digitalWrite(ledPin, LOW);
 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW);
  delay(500);
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW);
  delay(500);
}
 
void loop() {
  buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH) {
    
    while (payload != "{\"success\":true}") {
      digitalWrite(ledPin, HIGH);
      delay(100);
      digitalWrite(ledPin, LOW);
      delay(100);
      digitalWrite(ledPin, HIGH);
      delay(100);
      digitalWrite(ledPin, LOW);
      delay(100);
      
      Serial.println(ledState);
      if (ledState == HIGH) {
        payload = get_request("http://192.168.0.111:51828/?accessoryId=switch1&state=true");
      } else {
        payload = get_request("http://192.168.0.111:51828/?accessoryId=switch1&state=false");
      }
      delay(100);
    }
    
    if (ledState == HIGH) {
      ledState = LOW;
    } else {
      ledState = HIGH;
    }
    
    payload = "NONE";
    digitalWrite(ledPin, ledState);
    delay(1000);
  }
  delay(10);
}
