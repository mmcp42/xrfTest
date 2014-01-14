#define GROVE 6
#define XRF 7

int curPage;

#include "DataRecord.h"
uint8_t lineBuffer[16];

//==============================================================
// data flash
//==============================================================
#define DF_SELECT   10
#define DF_MOSI     11
#define DF_MISO     12
#define DF_SPICLOCK 13
#include <Sodaq_dataflash.h>

#include <Sodaq_DS3231.h>
DateTime timeNow;

//==============================================================
// battery voltage
//==============================================================
#define BATVOLT_R1      10              // voltage divider R1 = 10M
#define BATVOLT_R2      2               // voltage divider R2 = 2M
#define ADC_AREF        3.3

// get global settings
//====================
#include "globals.h"

boolean gLoop;

void setup(void)
{
  pinMode(GROVE, OUTPUT);
  pinMode(XRF, OUTPUT);
  
  digitalWrite(GROVE, HIGH);
  
  rtcInit();
  
  flashInit();
  
  digitalWrite(XRF, LOW);
  Serial.begin(57600);
  showVersion();
  showCommands();
  gLoop = false;
  Serial.println("setup complete");
}

void loop(void)
{
  int i;

  if (gLoop)
  {  
    for (i=0; i<10; i++)
    {
      Serial.print("hello world: ");
      Serial.println(i);
      delay(250);
    }
    Serial.println();
    delay(500);
  }
  
  checkCommands();
}
