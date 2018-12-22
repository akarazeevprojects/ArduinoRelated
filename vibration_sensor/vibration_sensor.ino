const int vib_pin_analog = 0;
const int vib_pin_dig = 4;
const int led_pin = 2;

void setup() {
  Serial.begin(115200);
  pinMode(vib_pin_dig, INPUT);
  pinMode(vib_pin_analog, INPUT);
  pinMode(led_pin, OUTPUT);
}

void loop() {
    int val = analogRead(vib_pin_analog);
    int dig_val = digitalRead(vib_pin_dig);

    Serial.println(dig_val, DEC);
    Serial.println(val, DEC);
    
    if (dig_val == 0) {
        digitalWrite(led_pin,HIGH);
        delay(1000);
        digitalWrite(led_pin,LOW);
        delay(1000);
    } else {
        digitalWrite(led_pin,LOW);
    }
    
    delay(10);
}
