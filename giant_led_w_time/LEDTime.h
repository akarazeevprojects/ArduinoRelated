#include <ESP8266WiFi.h>
const char *ssid     = "<YOUR_SSID>";
const char *password = "<YOUR_PASSWORD>";

#include <NTPClient.h>
#include <WiFiUdp.h>
#define NTP_OFFSET  3  * 60 * 60         // In seconds
#define NTP_INTERVAL 60 * 1000 * 1000    // In miliseconds
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", NTP_OFFSET, NTP_INTERVAL);

#include <Adafruit_SSD1306.h>
#define OLED_RESET LED_BUILTIN
Adafruit_SSD1306 display(OLED_RESET);

void displayDate() {
  timeClient.update();

  int currentHour = timeClient.getHours();
  int currentMinute = timeClient.getMinutes();

  static int prevMinute = currentMinute + 1;
  if (currentMinute != prevMinute) {
    String formattedTime = timeClient.getFormattedTime();
    Serial.println(formattedTime);
    prevMinute = currentMinute;
    delay(10);
  }

  static unsigned long colonBlink = millis();
  static char sep = ':';
  const long colonDelay = 1000;
  if (millis() - colonBlink >= colonDelay) {
    if (sep == ':') {
      sep = ' ';
    } else {
      sep = ':';
    }
    String currentHourStr = currentHour < 10 ? "0" + String(currentHour) : String(currentHour);
    String currentMinuteStr = currentMinute < 10 ? "0" + String(currentMinute) : String(currentMinute);
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print(String(currentHourStr));
    display.print(sep);
    display.print(String(currentMinuteStr));
    display.display();
    colonBlink = millis();
  }
}

void connectWiFi(void) {
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print ( "." );
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  delay(1000);
}
