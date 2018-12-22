int soundPin = A0;
 
void setup()
{
  Serial.begin(9600);
}
 
void loop()
{
    long sum = 0;
    for(int i=0; i<100; i++)
    {
        sum += analogRead(soundPin);
    }
 
    sum = sum/100;
 
    Serial.println(sum);
 
    delay(10);
}
