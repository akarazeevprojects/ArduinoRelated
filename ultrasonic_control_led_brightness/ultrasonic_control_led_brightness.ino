#include <Ultrasonic.h>

const int led_pin = 3;
const int max_counter = 10;
const float max_distance = 40.;

int counter = 0;
int distance = 0;
float prev_brightness = 0.;
float brightness = 0.;
float cumulated_distance = 0.;

Ultrasonic ultrasonic(11, 12);  // (trig, echo)

void setup() {
  pinMode(led_pin, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  distance = ultrasonic.distanceRead(CM);
  Serial.print(distance);
//  
//  if (counter >= max_counter) {
//    counter = 0;
//  } else {
//    cumulated_distance += distance;
//    counter += 1;
//  }
//  distance = cumulated_distance / counter;

  if (distance > max_distance) {
    distance = max_distance;
  }
  
  brightness = distance / max_distance;
  Serial.print("  ");
  Serial.print(brightness);

  if (brightness != 0.) {
    prev_brightness = brightness;
  } else {
    brightness = prev_brightness;
  }
  Serial.print("  ");
  Serial.println(brightness);
  
  analogWrite(led_pin, int(brightness * 255));
  
  delay(1);
}
