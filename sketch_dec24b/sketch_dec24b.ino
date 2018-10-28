int inPin = 7;  // the pin number for input (for me a push button)
int ledPin = 13; 

int current;         // Current state of the button
                     // (LOW is pressed b/c i'm using the pullup resistors)
long millis_held;    // How long the button was held (milliseconds)
long secs_held;      // How long the button was held (seconds)
long prev_secs_held; // How long the button was held in the previous check
byte previous = HIGH;
unsigned long firstTime; // how long since the button was first pressed 

byte state = HIGH;


void setup() {
  Serial.begin(9600);         // Use serial for debugging
  pinMode(ledPin, OUTPUT);
  digitalWrite(inPin, HIGH);  // Turn on 20k pullup resistors to simplify switch input
}

void loop() {
  current = digitalRead(inPin);

  if (current == LOW && previous == HIGH && (millis() - firstTime) > 200) {
    firstTime = millis();
  }

  millis_held = (millis() - firstTime);
  secs_held = millis_held / 1000;
  
  if (millis_held > 50) {
    if (current == HIGH && previous == LOW) {
      Serial.println("Switch!");
      if (state == HIGH) {
        state = LOW;
      } else {
        state = HIGH;
      }
      digitalWrite(ledPin, state);
    }
  }

  previous = current;
  prev_secs_held = secs_held;
}
