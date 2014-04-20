

const int hallPin = 12;     // the number of the hall effect sensor pin
const int ledPin =  13;     // the number of the LED pin


int hallState = 0;          // variable for reading the hall sensor status
//variables for reading "start" from MA
int serialvalue; // value for serial input
int started = 0; // flag for whether we've received serial yet

void setup() {
  Serial.begin(9600);

  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);      
  // initialize the hall effect sensor pin as an input:
  pinMode(hallPin, INPUT);     
}

void loop(){

  if(Serial.available()) // check to see if there's serial data in the buffer
  {
    serialvalue = Serial.read(); // read a byte of serial data
    started = 1; // set the started flag to on
  }

  // loop once serial data has been received
  if(started){
    Serial.print("start");
    // read the state of the hall effect sensor:
    hallState = digitalRead(hallPin);

    if (hallState == LOW) {     
      // turn LED on:    
      digitalWrite(ledPin, LOW);  
      Serial.print(hallState);
      Serial.println();
    } 
    else {
      // turn LED off:
      digitalWrite(ledPin, HIGH); 
      Serial.print(hallState);
      Serial.println();

    }


    delay(10);
  }


}


