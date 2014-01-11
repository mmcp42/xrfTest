//==============================================================
//  mikeWeatherStation
//
//  sleep module
//
//  Mike McPherson
//  December 2013
//==============================================================

//=================================
// sleep info
//=================================
#include <avr/sleep.h>
#include <avr/wdt.h>

//=====================================================================================
// sleep routine
//=====================================================================================
void sleep(int seconds)
{
  // prepare to sleep
  //=================
  sleepInit(seconds);
  
  // sleep
  //======
  Serial.println(F("sleeping ..."));
  systemSleep();
  
  // we just woke up!
  //=================
  Serial.println(F("... and awake"));
}

//=====================================================================================
// prepare the watch dog timer
//=====================================================================================
void sleepInit(int seconds)
{
  // clear various "reset" flags
  //============================
  MCUSR = 0;     
    
  // allow changes, disable reset
  //=============================
  WDTCSR = bit (WDCE) | bit (WDE);
  
  //============================================
  // set interrupt mode and interval
  //
  // WDP3 WDP2 WDP1 WDP0 time mS ~seconds
  //============================================
  //    0    0    0    0   16 mS
  //    0    0    0    1   32 mS
  //    0    0    1    0   64 mS
  //    0    0    1    1  128 mS 0.125
  //    0    1    0    0  256 mS 0.25
  //    0    1    0    1  512 mS 0.5
  //    0    1    1    0 1024 mS 1
  //    0    1    1    1 2048 mS 2
  //    1    0    0    0 4096 mS 4
  //    1    0    0    1 8192 mS 8
  //============================================

  switch (seconds)
  {
    case 1:
      WDTCSR = bit (WDIE) |              bit (WDP2) | bit (WDP1);
      break;
    case 2:
      WDTCSR = bit (WDIE) |              bit (WDP2) | bit (WDP1) | bit (WDP0);
      break;
    case 4:
      WDTCSR = bit (WDIE) | bit (WDP3);
      break;
    case 8:
      WDTCSR = bit (WDIE) | bit (WDP3)                           | bit (WDP0);
      break;
    default:
      Serial.println(F("must be 1, 2, 4 or 8 seconds only"));
      return;
      break;
  }
  
  // pat the dog
  //============
  wdt_reset();

  // set sleep mode
  //===============
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); 
}

//=====================================================================================
// set up sleep mode and sleep
//=====================================================================================
void systemSleep()
{
  // disable ADC
  //============
  ADCSRA &= ~_BV(ADEN);
  
  // turn off brown-out enable in software
  //======================================
  MCUCR = bit (BODS) | bit (BODSE);
  MCUCR = bit (BODS); 
  // DIAGPRINT("sleep... ");

  // sleep...
  //=========
  sleep_mode(); 

  // ... and wake-up
  //================
  // DIAGPRINTLN(" ...waken");

  // enable ADC
  //===========
  ADCSRA |= _BV(ADEN);
}


