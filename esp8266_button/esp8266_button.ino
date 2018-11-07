int ledpin = 0;
int button = 2;
int buttonState=0;

void setup() {
  pinMode(ledpin, OUTPUT);
  pinMode(button, INPUT);
}

void loop() {
  buttonState = digitalRead(button); // put your main code here, to run repeatedly:

  if (buttonState == 1) {
    digitalWrite(ledpin, HIGH); 
    delay(200);
  }
  
  if (buttonState==0) {
    digitalWrite(ledpin, LOW); 
    delay(200);
  }
}
