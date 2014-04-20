

int ledpin = 13; //Set LED pin for visual debugging
volatile int state = LOW; // default hall sensor state is set to LOW
volatile int counter = 0; //base counter

/* ====== TIMER VARIABLES ========*/
unsigned long minute_1 = 60000; //1 minute variable
unsigned long tensec = 10000; //10 second variable to check for idle time
unsigned long usetime; // calculates use time to check for interaction
//pick your desired idletime
unsigned long idletime = tensec;


/* ====== FOR MAX COMMUNICATION ========*/
int started = 0; // flag for whether we've received serial yet
int serialvalue; // value for serial input

void setup()
{
  Serial.begin(9600);
  pinMode(ledpin, OUTPUT);
  attachInterrupt(2, use, CHANGE);
}

void loop()
{
  if(Serial.available()){
    serialvalue = Serial.read(); //read the Data from MAX
    started =1; //turn everything on.
  }

  if (started){ 
    unsigned time= millis();

    Serial.print(counter);
    Serial.print(" ");
      Serial.print("\r");

    digitalWrite(ledpin, state);


    //if there is an interaction
    if((millis() - usetime) < idletime) {

    }
    
    //Check for the time since the last interaction
    //if it is grater than the desired idle time, do nothing and subtract counter.
    if((millis() - usetime) >= idletime){
      //do nothing but reduce the counter if it is above 0
      counter --;
      delay(500);
      if (counter < 0){
        counter = 0;
      }
    }
    delay(10);
  } // end if started


} //end loop

void use()
{
  usetime = millis();
  state = !state;
  counter++;
}




