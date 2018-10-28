#include <Ultrasonic.h>

Ultrasonic ultrasonic(12,13);  // (trig, echo)

int ledPin = 5;
int threshold = 30;  // centimeters
int distance = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  distance = ultrasonic.distanceRead(CM);
  
  Serial.print(distance);
  Serial.println(" cm");
  
  if (distance > threshold) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
  delay(100);
}
