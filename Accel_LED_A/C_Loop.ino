
void loop() {
  //*****
  //  ACCELEROMETER
  //*****

  tapType=0;

  //Reading 6 bytes of data starting at register DATAX0 will retrieve the x,y and z acceleration values from the ADXL345.
  //The results of the read operation will get stored to the values[] buffer.
  readRegister(DATAX0, 6, values);

  //The ADXL345 gives 10-bit acceleration values, but they are stored as bytes (8-bits). To get the full value, two bytes must be combined for each axis.
  //The X value is stored in values[0] and values[1].
  x = ((int)values[1]<<8)|(int)values[0];
  //The Y value is stored in values[2] and values[3].
  y = ((int)values[3]<<8)|(int)values[2];
  //The Z value is stored in values[4] and values[5].
  z = ((int)values[5]<<8)|(int)values[4];

  //Convert the accelerometer value to G's. 
  //With 10 bits measuring over a +/-4g range we can find how to convert by using the equation:
  // Gs = Measurement Value * (G-range/(2^10)) or Gs = Measurement Value * (8/1024)
  xg = x * 0.0078;
  yg = y * 0.0078;
  zg = z * 0.0078;

  //Print Basic Data
  /*
  Serial.print(tapType+", ");
   Serial.print(xg);
   Serial.print(',');
   Serial.print(yg);
   Serial.print(',');
   Serial.println(zg);
   */

  //Send This Data
  //  FIRST -- Check to see if MAX is connected before doing anything
  if(Serial.available()){
    serialvalue = Serial.read(); //read the Data from MAX
    started =1; //turn everything on.
  }

  //If MAX IS connected...
  if (started){ 
    Serial.println(counter);
//    Serial.print(\n);
 
  }
 
//In Use
  if(xg > 0.05){
    usetime = millis();
    counter ++;
  }
  if((millis() - usetime) >= idletime){
    //do nothing but reduce the counter if it is above 0
    counter -=3;
    if (counter >= 1){

      runleds();
    }
    if (counter < 0){
      counter = 0;
   
    }
  }




  if (xg >.3 && xg < 1.999 ) {
    Serial.print("ONE G, ");
    Serial.println(xg, DEC);
    runleds();

  }

  if (xg >2 && xg < 2.999 ) {
    Serial.print("TWO Gs, ");
    Serial.println(xg, DEC);
  }



  if(tapType > 0)
  {
    if(tapType == 1){
      Serial.println("SINGLE");

    }
    else{
      Serial.println("DOUBLE");
      Serial.print((float)xg,2);
      Serial.print("g,");
      Serial.print((float)yg,2);
      Serial.print("g,");
      Serial.print((float)zg,2);
      Serial.println("g");
    }
    detachInterrupt(0);
    delay(10);
    attachInterrupt(2, tap, RISING);
    //    intType=0;    
  }
  delay(10); 




  // *****
  //  LEDS
  // *****

}







