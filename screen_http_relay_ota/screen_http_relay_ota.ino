/*
To update:
curl -F "image=@/var/folders/.../ota_display.ino.bin" http://192.168.0.15/update

http://192.168.0.18:81/RELAY=OFF

size 1: 6x8
size 2: 12x16
size 3: 18x24
size 4: 24x32

*/

#include <SPI.h>
#include <Wire.h>
#include <ESP8266HTTPClient.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ArduinoJson.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>

const char* host = "esp8266-webupdate";
const char* ssid = "<YOUR_SSID>";
const char* password = "<YOUR_PASSWORD>";

ESP8266WebServer httpServer(80);
WiFiServer server(81);
ESP8266HTTPUpdateServer httpUpdater;

HTTPClient http;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET LED_BUILTIN
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define MAX_COUNTER 30
int counter = 0;
int displayonoff = 1;

int relayPin = 12;
int relayonoff = LOW;

void display_text(String text, int text_size) {
  display.clearDisplay();
  display.setTextSize(text_size);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println(text);
  display.display();
}

String get_request(String url) {
  http.begin(url);
  int httpCode = http.GET();
  String payload = "FAILED";
  if (httpCode > 0) {
    payload = http.getString();
  }
  http.end();
  return payload;
}

void setup(void) {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Booting Sketch...");

  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);
  
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    Serial.println("WiFi failed, retrying.");
  }

  MDNS.begin(host);
  server.begin();

  httpUpdater.setup(&httpServer);
  httpServer.begin();

  MDNS.addService("http", "tcp", 80);
  Serial.printf("HTTPUpdateServer ready! Open http://%s.local/update in your browser\n", host);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display_text(WiFi.localIP().toString(), 2);
  delay(2000);
}

void loop() {
  WiFiClient client = server.available();
  if (client){
    // Wait until the client sends some data
    Serial.println("new client");
    while(!client.available()){
      delay(1);
    }
   
    // Read the first line of the request
    String request = client.readStringUntil('\r');
    Serial.println(request);
    client.flush();
   
    if (request.indexOf("/OLED=ON") != -1)  {
      displayonoff = 1;
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println(""); //  do not forget this one
      client.println("<!DOCTYPE HTML>");
      client.println("<html>");
      client.print("OLED is now: ");
      if (displayonoff == 1) {
        client.print("On");
      } else {
        client.print("Off");
      }
      client.println("<br><br>");
      client.println("<a href=\"/OLED=ON\"\"><button>Turn On </button></a>");
      client.println("<a href=\"/OLED=OFF\"\"><button>Turn Off </button></a><br />");  
      client.println("</html>");
    } else if (request.indexOf("/OLED=OFF") != -1)  {
      displayonoff = -1;
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println(""); //  do not forget this one
      client.println("<!DOCTYPE HTML>");
      client.println("<html>");
      client.print("OLED is now: ");
      if (displayonoff == 1) {
        client.print("On");
      } else {
        client.print("Off");
      }
      client.println("<br><br>");
      client.println("<a href=\"/OLED=ON\"\"><button>Turn On </button></a>");
      client.println("<a href=\"/OLED=OFF\"\"><button>Turn Off </button></a><br />");  
      client.println("</html>");
    } else if (request.indexOf("/RELAY=ON") != -1)  {
      relayonoff = HIGH;
      digitalWrite(relayPin, HIGH);
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println(""); //  do not forget this one
      client.println("<!DOCTYPE HTML>");
      client.println("<html>");
      client.print("RELAY is now: ");
      if (relayonoff == HIGH) {
        client.print("On");
      } else {
        client.print("Off");
      }
      client.println("<br><br>");
      client.println("<a href=\"/RELAY=ON\"\"><button>Turn On </button></a>");
      client.println("<a href=\"/RELAY=OFF\"\"><button>Turn Off </button></a><br />");  
      client.println("</html>");
    } else if (request.indexOf("/RELAY=OFF") != -1)  {
      relayonoff = LOW;
      digitalWrite(relayPin, LOW);
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println(""); //  do not forget this one
      client.println("<!DOCTYPE HTML>");
      client.println("<html>");
      client.print("RELAY is now: ");
      if (relayonoff == HIGH) {
        client.print("On");
      } else {
        client.print("Off");
      }
      client.println("<br><br>");
      client.println("<a href=\"/RELAY=ON\"\"><button>Turn On </button></a>");
      client.println("<a href=\"/RELAY=OFF\"\"><button>Turn Off </button></a><br />");  
      client.println("</html>");
    }
  }
  
  httpServer.handleClient();

  if (displayonoff == -1) {
      display.clearDisplay();
      display.display();
      displayonoff = 0;
      counter = 0;
  } else if (displayonoff == 1) {
    String payload = get_request("http://192.168.0.16:5000/data");
    if (payload != "FAILED") {
      StaticJsonBuffer<300> jsonBuffer;
      JsonObject& root = jsonBuffer.parseObject(payload);
  
      if (counter < MAX_COUNTER / 3) {
        String date = root["time"];
        date = date.substring(0, 5);
        display.clearDisplay();
    
        display.setTextSize(2);
        display.setTextColor(WHITE);
        String topic_name = "Time";
        display.setCursor(round((128 - (topic_name.length() * 12)) / 2), 0);
        display.println(topic_name);
        
        display.setTextSize(4);
        display.setTextColor(WHITE);
        display.setCursor(round((128 - (date.length() * 24)) / 2), 20);
        display.println(date);
        display.display();
      } else if ((MAX_COUNTER / 3) <= counter && (counter < 2 * MAX_COUNTER / 3)) {
        String weather = root["temp"];
        display.clearDisplay();
    
        display.setTextSize(2);
        display.setTextColor(WHITE);
        String topic_name = "Weather";
        display.setCursor(round((128 - (topic_name.length() * 12)) / 2), 0);
        display.println(topic_name);
        
        display.setTextSize(3);
        display.setTextColor(WHITE);
        display.setCursor(round((128 - (weather.length() * 18)) / 2), 20);
        display.println(weather);
        display.display();
      } else {
        String train = root["traintime"];
        display.clearDisplay();
    
        display.setTextSize(2);
        display.setTextColor(WHITE);
        String topic_name = "Next Train";
        display.setCursor(round((128 - (topic_name.length() * 12)) / 2), 0);
        display.println(topic_name);
        
        display.setTextSize(3);
        display.setTextColor(WHITE);
        display.setCursor(round((128 - (train.length() * 18)) / 2), 20);
        display.println(train);
        display.display();
      }
      counter += 1;
      counter %= MAX_COUNTER;
    }
  }

  delay(100);
}
