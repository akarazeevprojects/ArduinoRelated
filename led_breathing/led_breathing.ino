const int led_pin = 3;

void setup() {
  pinMode(led_pin, OUTPUT);
}

void loop() {
  for (int i = 0; i < 256; i++) {
    analogWrite(led_pin, i);
    delay(10);
  }
  for (int i = 255; i >= 0; i--) {
    analogWrite(led_pin, i);
    delay(10);
  }
}
