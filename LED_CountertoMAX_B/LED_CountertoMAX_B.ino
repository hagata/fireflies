

//int ledpin = 13; //Set LED pin for visual debugging
volatile int state = LOW; // default hall sensor state is set to LOW
volatile int counter ; //base counter

/* ====== TIMER VARIABLES ========*/
unsigned long minute_1 = 60000; //1 minute variable
unsigned long tensec = 10000; //10 second variable to check for idle time
unsigned long threesec = 3000;
unsigned long usetime; // calculates use time to check for interaction
//pick your desired idletime
unsigned long idletime = 900;
unsigned long posttime = 30; //instead of using delay, use millis calculation
unsigned long previoustime = 0;

/* ====== FOR MAX COMMUNICATION ========*/
int started = 0; // flag for whether we've received serial yet
int serialvalue; // value for serial input

/* ===============================================================
 ========== BINARY PARTTERNS & LED STUFF  =========================
 =================================================================*/

#include <avr/pgmspace.h>        // allows use of PROGMEM to store patterns in flash

#define CUBESIZE 3
#define PLANESIZE CUBESIZE*CUBESIZE
#define PLANETIME 3333          // time each plane is displayed in us -> 100 Hz refresh
#define TIMECONST 20          // multiplies DisplayTime to get ms - why not =100?

//Define the variables needed in the LED Loop later
byte PatternBuf[ PLANESIZE ]; 
int PatternIdx;
byte DisplayTime; //time*100ms to display pattern
unsigned long EndTime;
int plane; //loop counter for cube refresh
int patbufidx; //indexes which byte from pattern buffer
int ledrow; //counts LEDs in refresh loop
int ledcol; //counts LEDS in refresh loop
int ledpin; //counts LEDS in refresh loop

// LED Pattern Table in PROGMEM - last column is display time in 100ms units
// TODO this could be a lot more compact but not with binary pattern representation

//flashonce really flashes twice to give  time for people to make 1 full revolution
int FlashOnce[] {
  B111, B111, B111, B111, B111, B111, B111, 5,
  B000, B000, B000, B000, B000, B000, B000, 1,
  B111, B111, B111, B111, B111, B111, B111, 5,
  B000, B000, B000, B000, B000, B000, B000, 0,

};
int ShortPattern[] = { //set for 7 columns  me
    B001, B001, B000, B000, B001, B001, B000, 1,
    B001, B001, B000, B000, B001, B001, B000, 1,
    B000, B001, B001, B000, B000, B001, B000, 1,
    B000, B000, B001, B000, B000, B001, B000, 1,
    B000, B000, B000, B000, B000, B001, B000, 1,
    B000, B000, B000, B000, B000, B000, B000, 1,
    B000, B000, B000, B000, B000, B000, B000, 1,
    B000, B000, B000, B000, B000, B000, B000, 1,
    B000, B000, B000, B000, B000, B000, B000, 1,
    B000, B000, B000, B000, B000, B000, B000, 1,
    B000, B000, B000, B000, B000, B000, B000, 1,


};


prog_uchar PROGMEM PatternTable[] = {
  // blink on and off
  B111, B111, B111, B111, B111, B111, B111, B111, B111, 5,
  B000, B000, B000, B000, B000, B000, B000, B000, B000, 1,
  B111, B111, B111, B111, B111, B111, B111, B111, B111, 5,
  B000, B000, B000, B000, B000, B000, B000, B000, B000, 1,

  /*
  // flash each LED in sequence: 
   // Left->Right column, then Top->Bottom row, then Upper->Lower plane
   B100, B000, B000, B000, B000, B000, B000, B000, B000, 1,
   B010, B000, B000, B000, B000, B000, B000, B000, B000, 1,
   B001, B000, B000, B000, B000, B000, B000, B000, B000, 1,
   B000, B100, B000, B000, B000, B000, B000, B000, B000, 1,
   B000, B010, B000, B000, B000, B000, B000, B000, B000, 1,
   B000, B001, B000, B000, B000, B000, B000, B000, B000, 1,
   B000, B000, B100, B000, B000, B000, B000, B000, B000, 1,
   B000, B000, B010, B000, B000, B000, B000, B000, B000, 1,
   B000, B000, B001, B000, B000, B000, B000, B000, B000, 1,
   B000, B000, B000, B100, B000, B000, B000, B000, B000, 1,
   B000, B000, B000, B010, B000, B000, B000, B000, B000, 1,
   B000, B000, B000, B001, B000, B000, B000, B000, B000, 1,
   B000, B000, B000, B000, B100, B000, B000, B000, B000, 1,
   B000, B000, B000, B000, B010, B000, B000, B000, B000, 1,
   B000, B000, B000, B000, B001, B000, B000, B000, B000, 1,
   B000, B000, B000, B000, B000, B100, B000, B000, B000, 1,
   B000, B000, B000, B000, B000, B010, B000, B000, B000, 1,
   B000, B000, B000, B000, B000, B001, B000, B000, B000, 1,
   B000, B000, B000, B000, B000, B000, B100, B000, B000, 1,
   B000, B000, B000, B000, B000, B000, B010, B000, B000, 1,
   B000, B000, B000, B000, B000, B000, B001, B000, B000, 1,
   B000, B000, B000, B000, B000, B000, B000, B100, B000, 1,
   B000, B000, B000, B000, B000, B000, B000, B010, B000, 1,
   B000, B000, B000, B000, B000, B000, B000, B001, B000, 1,
   B000, B000, B000, B000, B000, B000, B000, B000, B100, 1,
   B000, B000, B000, B000, B000, B000, B000, B000, B010, 1,
   B000, B000, B000, B000, B000, B000, B000, B000, B001, 10,
   */

  /*
  // Some little cube - big cube fun  
   B000, B000, B000, B000, B011, B011, B000, B011, B011, 10,
   B111, B111, B111, B111, B111, B111, B111, B111, B111, 10,
   B000, B000, B000, B000, B011, B011, B000, B011, B011, 5,
   B000, B000, B000, B000, B000, B000, B000, B000, B001, 2,
   B000, B000, B000, B000, B011, B011, B000, B011, B011, 2,
   B111, B111, B111, B111, B111, B111, B111, B111, B111, 2,
   B000, B000, B000, B000, B011, B011, B000, B011, B011, 2,
   B000, B000, B000, B000, B000, B000, B000, B000, B001, 2,
   B000, B000, B000, B000, B011, B011, B000, B011, B011, 2,
   B111, B111, B111, B111, B111, B111, B111, B111, B111, 1,
   B000, B000, B000, B000, B011, B011, B000, B011, B011, 1,
   B000, B000, B000, B000, B000, B000, B000, B000, B001, 1,
   B000, B000, B000, B000, B011, B011, B000, B011, B011, 1,
   B111, B111, B111, B111, B111, B111, B111, B111, B111, 1,
   B110, B110, B000, B110, B110, B000, B000, B000, B000, 1,
   B100, B000, B000, B000, B000, B000, B000, B000, B000, 1,
   B110, B110, B000, B110, B110, B000, B000, B000, B000, 1,
   B111, B111, B111, B111, B111, B111, B111, B111, B111, 1,
   B000, B000, B000, B000, B011, B011, B000, B011, B011, 1,
   B000, B000, B000, B000, B000, B000, B000, B000, B001, 1,
   B000, B000, B000, B000, B011, B011, B000, B011, B011, 1,
   B111, B111, B111, B111, B111, B111, B111, B111, B111, 1,
   B110, B110, B000, B110, B110, B000, B000, B000, B000, 1,
   B100, B000, B000, B000, B000, B000, B000, B000, B000, 1,
   B110, B110, B000, B110, B110, B000, B000, B000, B000, 1,
   B111, B111, B111, B111, B111, B111, B111, B111, B111, 1,
   B000, B011, B011, B000, B011, B011, B000, B000, B000, 1,
   B000, B000, B001, B000, B000, B000, B000, B000, B000, 1,
   B000, B011, B011, B000, B011, B011, B000, B000, B000, 1,
   B111, B111, B111, B111, B111, B111, B111, B111, B111, 1,
   B000, B000, B000, B110, B110, B000, B110, B110, B000, 1,
   B000, B000, B000, B000, B000, B000, B100, B000, B000, 1,
   B000, B000, B000, B110, B110, B000, B110, B110, B000, 1,
   B111, B111, B111, B111, B111, B111, B111, B111, B111, 1,
   B000, B011, B011, B000, B011, B011, B000, B000, B000, 1,
   B000, B000, B001, B000, B000, B000, B000, B000, B000, 1,
   B000, B011, B011, B000, B011, B011, B000, B000, B000, 1,
   B111, B111, B111, B111, B111, B111, B111, B111, B111, 1,
   B000, B000, B000, B110, B110, B000, B110, B110, B000, 1,
   B000, B000, B000, B000, B000, B000, B100, B000, B000, 1,
   B000, B000, B000, B110, B110, B000, B110, B110, B000, 1,
   */
  // Diagonal wipe, starting in the center
  B111, B111, B111, B111, B111, B111, B111, B111, B111, 5,
  B111, B111, B111, B111, B111, B111, B111, B101, B111, 1,
  B111, B111, B111, B111, B101, B111, B111, B101, B111, 1,
  B111, B111, B111, B111, B101, B111, B111, B100, B111, 1,
  B111, B101, B111, B111, B100, B111, B111, B100, B110, 1,
  B111, B101, B111, B111, B100, B111, B111, B100, B110, 1,
  B111, B011, B111, B111, B100, B110, B111, B100, B100, 1,
  B111, B100, B110, B111, B100, B100, B111, B100, B000, 1,
  B111, B100, B100, B111, B100, B000, B111, B000, B000, 1,
  B111, B100, B000, B111, B000, B000, B011, B000, B000, 1,
  B111, B000, B000, B011, B000, B000, B001, B001, B000, 1,
  /*
  // 2-LED wide diaginal stripe that orbits the cube
   B011, B000, B000, B001, B001, B000, B000, B001, B001, 1,
   B001, B001, B000, B000, B001, B001, B000, B000, B011, 1,
   B000, B001, B001, B000, B000, B011, B000, B000, B110, 1,
   B000, B000, B011, B000, B000, B110, B000, B100, B100, 1,
   B000, B000, B110, B000, B100, B100, B100, B100, B000, 1,
   B000, B100, B100, B100, B100, B000, B110, B000, B000, 1,
   B100, B100, B000, B110, B000, B000, B011, B000, B000, 1,
   B110, B000, B000, B011, B000, B000, B001, B001, B000, 1,
   */

  /*
  // Now, with center flashies, for flavor  
   B011, B000, B000, B001, B001, B000, B000, B011, B001, 1,
   B001, B001, B000, B000, B001, B001, B000, B000, B011, 1,
   B000, B001, B001, B000, B000, B011, B000, B010, B110, 1,
   B000, B000, B011, B000, B000, B110, B000, B100, B100, 1,
   B000, B000, B110, B000, B100, B100, B100, B110, B000, 1,
   B000, B100, B100, B100, B100, B000, B110, B000, B000, 1,
   B100, B100, B000, B110, B000, B000, B011, B010, B000, 1,
   B110, B000, B000, B011, B000, B000, B001, B001, B000, 1,
   B011, B000, B000, B001, B011, B000, B000, B001, B001, 1,
   B001, B001, B000, B000, B001, B001, B000, B000, B011, 1,
   B000, B001, B001, B000, B010, B011, B000, B000, B110, 1,
   B000, B000, B011, B000, B000, B110, B000, B100, B100, 1,
   B000, B000, B110, B000, B110, B100, B100, B100, B000, 1,
   B000, B100, B100, B100, B100, B000, B110, B000, B000, 1,
   B100, B100, B000, B110, B010, B000, B011, B000, B000, 1,
   B110, B000, B000, B011, B000, B000, B001, B001, B000, 1,
   B011, B010, B000, B001, B001, B000, B000, B001, B001, 1,
   B001, B001, B000, B000, B001, B001, B000, B000, B011, 1,
   B000, B011, B001, B000, B000, B011, B000, B000, B110, 1,
   B000, B000, B011, B000, B000, B110, B000, B100, B100, 1,
   B000, B010, B110, B000, B100, B100, B100, B100, B000, 1,
   B000, B100, B100, B100, B100, B000, B110, B000, B000, 1,
   B100, B110, B000, B110, B000, B000, B011, B000, B000, 1,
   B110, B000, B000, B011, B000, B000, B001, B001, B000, 1,
   */
  /*
  // Wrapping up 
   B001, B001, B000, B000, B001, B001, B000, B000, B001, 1,
   B001, B001, B000, B000, B001, B001, B000, B000, B001, 1,
   B000, B001, B001, B000, B000, B001, B000, B000, B001, 1,
   B000, B000, B001, B000, B000, B001, B000, B000, B001, 1,
   B000, B000, B000, B000, B000, B001, B000, B000, B001, 1,
   B000, B000, B000, B000, B000, B000, B000, B000, B001, 5,
   B000, B000, B000, B000, B000, B000, B000, B000, B000, 3,
   B000, B000, B000, B000, B000, B000, B000, B000, B001, 5,
   B000, B000, B000, B000, B000, B000, B000, B000, B000, 3,
   B000, B000, B000, B000, B000, B000, B000, B000, B001, 5,
   B000, B000, B000, B000, B000, B000, B000, B000, B000, 3,
   */

  // this is a dummy element for end of table (duration=0)
  B000, B000, B000, B000, B000, B000, B000, B000, B000, 0  
};

/*
** Defining pins in array makes it easier to rearrange how cube is wired
 ** Adjust numbers here until LEDs flash in order - L to R, T to B
 ** Note that analog inputs 0-5 are also digital outputs 14-19!
 ** Pin DigitalOut0 (serial RX) and AnalogIn5 are left open for future apps
 */

//int LEDPin[] = {16, 3, 1, 15, 4, 6, 14, 5, 7};
int LEDPin[] = {
  1, 3, 4, 5, 6, 7, 8, 9, 10};

//int PlanePin[] = {19, 18, 17};
int PlanePin[] = {
  11, 12, 13};


/*============================================================
 End Binary
 ============================================================*/


void setup()
{
  Serial.begin(9600);
  pinMode(ledpin, OUTPUT);
  attachInterrupt(2, use, CHANGE);

  //LED SETUP
  int pin;      // loop counter
  // set up LED pins as output (active HIGH)
  for (pin=0; pin<PLANESIZE; pin++) {
    pinMode( LEDPin[pin], OUTPUT );
  }
  // set up plane pins as outputs (active LOW) 
  for (pin=0; pin<CUBESIZE; pin++) {
    pinMode( PlanePin[pin], OUTPUT );
  }
}





void loop()
{
  //  FIRST -- Check to see if MAX is connected before doing anything
  if(Serial.available()){
    serialvalue = Serial.read(); //read the Data from MAX
    started =1; //turn everything on.
  }

  //If MAX IS connected...
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
      if (counter >= 1){

        shortpattern();
      }
      if (counter < 0){
        counter = 0;

      }
    }

    //    if (currenttime - previoustime > posttime){
    Serial.print(counter, DEC);
    Serial.println();
    //      previoustime = currenttime;
    //      }

    /* ================LED LOOP =================*/



  } // end if started


} //end loop

void use()
{
  usetime = millis();
  state = !state;
  counter++;
  flashonce();
}










