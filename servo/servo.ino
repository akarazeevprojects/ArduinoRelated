#include <Servo.h>

Servo myservo;

const int servo_pin = 6; // digital pin

void setup() {
  myservo.attach(servo_pin);
}

void loop() {
  int pos = 0;
  for (pos = 0; pos <= 180; pos += 1) {
    myservo.write(pos);
    delay(15);
  }
  for (pos = 180; pos >= 0; pos -= 1) {
    myservo.write(pos);
    delay(15);
  }
}
