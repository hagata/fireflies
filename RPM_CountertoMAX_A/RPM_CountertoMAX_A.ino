int pin = 13;
volatile int state = LOW;
volatile int counter = 0;

unsigned long minute_1 = 60000;
unsigned long tensec = 10000;

unsigned long usetime;

void setup()
{
  Serial.begin(9600);
  pinMode(pin, OUTPUT);
  attachInterrupt(2, use, CHANGE);
}

void loop()
{
  if(Serial.available == true){
    
    unsigned time= millis();
    Serial.print("timer:");
    Serial.print("\t");
    Serial.print(time,10);
    Serial.print("\t");
    Serial.print("Counter");
    Serial.print("\t");
    Serial.println(counter);
    delay(1000);
    digitalWrite(pin, state);

    if((millis() - usetime) >= tensec){
      Serial.print("it works"); 
      counter --;
      if (counter < 0){
        counter = 0;
      }
    }
  }


}

void use()
{
  usetime = millis();
  state = !state;
  counter++;
}


