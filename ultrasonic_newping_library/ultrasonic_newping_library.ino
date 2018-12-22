#include <NewPing.h>

#define TRIGGER_PIN  12
#define ECHO_PIN     11
#define MAX_DISTANCE 200

NewPing sonar(TRIGGER_PIN, ECHO_PIN);

void setup() {
  Serial.begin(115200);
}

void loop() {
  Serial.println(sonar.ping_cm());
  delay(100);
}
