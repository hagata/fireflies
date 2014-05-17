

void setup() {
  
   //Initiate an SPI communication instance.
  SPI.begin();
  //Configure the SPI connection for the ADXL345.
  SPI.setDataMode(SPI_MODE3);
  //Create a serial connection to display the data on the terminal.
  Serial.begin(9600);

  //Set up the Chip Select pin to be an output from the Arduino.
  pinMode(CS, OUTPUT);
  //Before communication starts, the Chip Select pin needs to be set high.
  digitalWrite(CS, HIGH);

  //Create an interrupt that will trigger when a tap is detected.
  attachInterrupt(2, runleds, RISING);

  //Put the ADXL345 into +/- 4G range by writing the value 0x01 to the DATA_FORMAT register.
  writeRegister(DATA_FORMAT, 0x01);

  //Send the Tap and Double Tap Interrupts to INT1 pin
  writeRegister(INT_MAP, 0x9F);
  //Look for taps on the Z axis only.
  writeRegister(TAP_AXES, 0x01);
  //Set the Tap Threshold to 3g
  writeRegister(THRESH_TAP, 0x38);
  //Set the Tap Duration that must be reached
  writeRegister(DURATION, 0x10);

  //100ms Latency before the second tap can occur.
  writeRegister(LATENT, 0x50);
  writeRegister(WINDOW, 0xFF);

  //Enable the Single and Double Taps.
  writeRegister(INT_ENABLE, 0xE0);  

  //Put the ADXL345 into Measurement Mode by writing 0x08 to the POWER_CTL register.
  writeRegister(POWER_CTL, 0x08);  //Measurement mode
  readRegister(INT_SOURCE, 1, values); //Clear the interrupts from the INT_SOURCE register.
  
  
  
//  *******************
//   LEDS
//
//  ******************//
  
  
  
  //set pins to output because they are addressed in the main loop
  pinMode(latchPin, OUTPUT);
 

  //Arduino doesn't seem to have a way to write binary straight into the code 
  //so these values are in HEX.  Decimal would have been fine, too. 
  dataArrayRED[0] = 0xFF; //11111111
  dataArrayRED[1] = 0xFE; //11111110
  dataArrayRED[2] = 0xFC; //11111100
  dataArrayRED[3] = 0xF8; //11111000
  dataArrayRED[4] = 0xF0; //11110000
  dataArrayRED[5] = 0xE0; //11100000
  dataArrayRED[6] = 0xC0; //11000000
  dataArrayRED[7] = 0x80; //10000000
  dataArrayRED[8] = 0x00; //00000000
  dataArrayRED[9] = 0xE0; //11100000

  //Arduino doesn't seem to have a way to write binary straight into the code 
  //so these values are in HEX.  Decimal would have been fine, too. 
  dataArrayGREEN[0] = 0xFF; //11111111
  dataArrayGREEN[1] = 0x7F; //01111111
  dataArrayGREEN[2] = 0x3F; //00111111
  dataArrayGREEN[3] = 0x1F; //00011111
  dataArrayGREEN[4] = 0x0F; //00001111
  dataArrayGREEN[5] = 0x07; //00000111
  dataArrayGREEN[6] = 0x03; //00000011
  dataArrayGREEN[7] = 0x01; //00000001
  dataArrayGREEN[8] = 0x00; //00000000
  dataArrayGREEN[9] = 0x07; //00000111

  //function that blinks all the LEDs
  //gets passed the number of blinks and the pause time
  
//  blinkAll_2Bytes(1,500); 
}

