#include <Servo.h>

Servo myservo;

const int servo_pin = 6; // digital pin
const int SW_pin = 2; // digital pin connected to switch output
const int X_pin = 0; // analog pin connected to X output
const int Y_pin = 1; // analog pin connected to Y output

const int left_led = 12;
const int right_led = 11;
const int up_led = 10;
const int down_led = 9;

const int THRESHOLD = 200;
const int MAX_VALUE = 1023;


void on(int pin) {
  digitalWrite(pin, HIGH);
}

void off(int pin) {
  digitalWrite(pin, LOW);
}

void switch_state(int pin, int value_read) {
  if (value_read < THRESHOLD) {
    on(pin);
  } else {
    off(pin);
  }
}

void blink_all() {
  on(left_led);
  on(right_led);
  on(up_led);
  on(down_led);
  
  delay(1000);

  off(left_led);
  off(right_led);
  off(up_led);
  off(down_led);
}

void setup() {
  myservo.attach(servo_pin);
  pinMode(left_led, OUTPUT);
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH); // ??
  Serial.begin(115200);
}

void loop() {
  int SW_read = digitalRead(SW_pin);
  int X_read = analogRead(X_pin);
  int Y_read = analogRead(Y_pin);
  
//  Serial.print(SW_read);
//  Serial.print("\n");
//  Serial.print(X_read);
//  Serial.print("\n");
//  Serial.print(Y_read);
//  Serial.print("\n");

  if (SW_read == 0) {
    blink_all();
  } else {
    switch_state(left_led, X_read);
    switch_state(right_led, MAX_VALUE - X_read);
  
    switch_state(up_led, Y_read);
    switch_state(down_led, MAX_VALUE - Y_read);
  }
  
  float pos = 180 * (float(Y_read) / float(MAX_VALUE));
  Serial.print(pos);
  Serial.print("\n");
  myservo.write(int(pos));

  delay(10);
}
