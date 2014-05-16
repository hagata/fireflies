

int ledpin = 13; //Set LED pin for visual debugging
volatile int state = LOW; // default hall sensor state is set to LOW
volatile int counter ; //base counter

/* ====== TIMER VARIABLES ========*/
unsigned long minute_1 = 60000; //1 minute variable
unsigned long tensec = 10000; //10 second variable to check for idle time
unsigned long usetime; // calculates use time to check for interaction
//pick your desired idletime
unsigned long idletime = tensec;
unsigned long posttime = 30; //instead of using delay, use millis calculation
unsigned long previoustime = 0;

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
    unsigned long currenttime= millis();

    digitalWrite(ledpin, state);


    //if there is an interaction
    if((millis() - usetime) < idletime) {

    }

    //Check for the time since the last interaction
    //if it is grater than the desired idle time, do nothing and subtract counter.
    if((millis() - usetime) >= idletime){
      //do nothing but reduce the counter if it is above 0
      counter --;
      
      if (counter < 0){
        counter = 0;
      }
    }

//    if (currenttime - previoustime > posttime){
      Serial.print(counter, DEC);
      Serial.println();
//      previoustime = currenttime;
//      }
  




  } // end if started


} //end loop

void use()
{
  usetime = millis();
  state = !state;
  counter++;
}





