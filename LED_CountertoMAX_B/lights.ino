void shortpattern () {


    PatternIdx = 0;
    
     do {
      // read pattern from PROGMEM and save in array
      memcpy_P( PatternBuf, ShortPattern+PatternIdx, PLANESIZE );
      PatternIdx += PLANESIZE;
      // read DisplayTime from PROGMEM and increment index
      DisplayTime = pgm_read_byte_near( ShortPattern + PatternIdx++ );    
      // compute EndTime from current time (ms) and DisplayTime
      EndTime = millis() + ((unsigned long) DisplayTime) * TIMECONST;

      // loop while DisplayTime>0 and current time < EndTime
      while ( millis() < EndTime ) {
        patbufidx = 0;    // reset index counter to beginning of buffer
        // loop over planes
        for (plane=0; plane<CUBESIZE; plane++) {
          // turn previous plane off
          if (plane==0) {
            digitalWrite( PlanePin[CUBESIZE-1], HIGH );
          } 
          else {
            digitalWrite( PlanePin[plane-1], HIGH );
          }

          // load current plane pattern data into ports
          ledpin = 0;
          for (ledrow=0; ledrow<CUBESIZE; ledrow++) {
            for (ledcol=0; ledcol<CUBESIZE; ledcol++) {
              digitalWrite( LEDPin[ledpin++], PatternBuf[patbufidx] & (1 << ledcol) );
            }
            patbufidx++;
          } 

          // turn current plane on
          digitalWrite( PlanePin[plane], LOW );
          // delay PLANETIME us
          delayMicroseconds( PLANETIME );
        }    // for plane
      }    // while <EndTime
    } 
    while (DisplayTime > 0);        
}



void flashonce(){
 
 
    PatternIdx = 0;
    
     do {
      // read pattern from PROGMEM and save in array
      memcpy_P( PatternBuf, FlashOnce+PatternIdx, PLANESIZE );
      PatternIdx += PLANESIZE;
      // read DisplayTime from PROGMEM and increment index
      DisplayTime = pgm_read_byte_near( FlashOnce + PatternIdx++ );    
      // compute EndTime from current time (ms) and DisplayTime
      EndTime = millis() + ((unsigned long) DisplayTime) * TIMECONST;

      // loop while DisplayTime>0 and current time < EndTime
      while ( millis() < EndTime ) {
        patbufidx = 0;    // reset index counter to beginning of buffer
        // loop over planes
        for (plane=0; plane<CUBESIZE; plane++) {
          // turn previous plane off
          if (plane==0) {
            digitalWrite( PlanePin[CUBESIZE-1], HIGH );
          } 
          else {
            digitalWrite( PlanePin[plane-1], HIGH );
          }

          // load current plane pattern data into ports
          ledpin = 0;
          for (ledrow=0; ledrow<CUBESIZE; ledrow++) {
            for (ledcol=0; ledcol<CUBESIZE; ledcol++) {
              digitalWrite( LEDPin[ledpin++], PatternBuf[patbufidx] & (1 << ledcol) );
            }
            patbufidx++;
          } 

          // turn current plane on
          digitalWrite( PlanePin[plane], LOW );
          // delay PLANETIME us
          delayMicroseconds( PLANETIME );
        }    // for plane
      }    // while <EndTime
    } 
    while (DisplayTime > 0);      
  
}
