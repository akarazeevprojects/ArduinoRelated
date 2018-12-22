#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "InScienceWeTrust";
const char* password = "kogdabudetborsch";

HTTPClient http;

void setup() {
  Serial.begin(115200);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void loop() {

  String payload = get_request("http://192.168.0.12:5000/data");
  Serial.println(payload);
  
  StaticJsonBuffer<300> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(payload);

  const char* train = root["traintime"];
  Serial.println(train);
  Serial.println("==========");
  
  delay(5000);
}

String get_request(String url) {
  http.begin(url);
  int httpCode = http.GET();

  String payload = "FAILED";

  if (httpCode > 0) { //Check the returning code
    payload = http.getString();   //Get the request response payload
  }
  
  http.end();

  return payload;
}
