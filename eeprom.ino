//=====================================================================
// eeprom.ino
//
//  Created on: 6th January 2012
//      Author: Mike McPherson
//=====================================================================

#include <EEPROM.h>

// offsets into EEPROM
//====================
#define eeAPN        0
#define eeSERVER    32
#define eeUSERNAME  48
#define eePASSWORD  64
#define eeFTPPATH   80
#define eeWXID      96
#define eeMAGIC    112
#define eeFTPSQL   128
#define eeTIMEURL  192

//==============================================================
// "globals"
//==============================================================
extern uint8_t      gAPN[32];
extern uint8_t   gServer[16];
extern uint8_t gUserName[16];
extern uint8_t gPassword[16];
extern uint8_t  gFTPpath[16];
extern uint8_t     gWxId[16];
extern uint8_t    gMagic[ 8];
extern uint8_t   gFtpSql[48];
extern uint8_t  gTimeURL[48];

// module to save and restore settings 
// settings are stored in EEPROM
//====================================

void eepromDump(void)
{
  int row;
  int col;
  byte oneByte;

  // read entire eeprom
  //===================
  for (row=0; row<16; row++)
  {
    Serial.print("0");
    Serial.print(row, HEX);
    Serial.print(":");
    for (col=0; col<16; col++)
    {
      oneByte = EEPROM.read(row*16+col);
      Serial.print(" ");
      if (oneByte < 16)
        Serial.print("0");
      Serial.print(oneByte, HEX);
    }
    Serial.println();
  }
  Serial.println();
}

void eepromRead(uint8_t *target, int offset, int size)
{
  int i;

  // read value from address
  //========================
  for (i=0; i<size-1; i++)
  {
    target[i] = EEPROM.read(offset + i);
  }
  target[size-1] = '/0';
}

void eepromReadAll(void)
{
  // read saved settings from EEPROM
  //================================
  eepromRead(     &gAPN[0], eeAPN, sizeof(gAPN));
  eepromRead(  &gServer[0], eeSERVER, sizeof(gServer));
  eepromRead(&gUserName[0], eeUSERNAME, sizeof(gUserName));
  eepromRead(&gPassword[0], eePASSWORD, sizeof(gPassword));
  eepromRead( &gFTPpath[0], eeFTPPATH, sizeof(gFTPpath));
  eepromRead(    &gWxId[0], eeWXID, sizeof(gWxId));
  eepromRead(   &gMagic[0], eeMAGIC, sizeof(gMagic));
  eepromRead(  &gFtpSql[0], eeFTPSQL, sizeof(gFtpSql));
  eepromRead( &gTimeURL[0], eeTIMEURL, sizeof(gTimeURL));
}

void eepromSaveAll(void)
{
  // save all settings
  //==================
  eepromWrite(     gAPN, eeAPN, sizeof(gAPN));
  eepromWrite(  gServer, eeSERVER, sizeof(gServer));
  eepromWrite(gUserName, eeUSERNAME, sizeof(gUserName));
  eepromWrite(gPassword, eePASSWORD, sizeof(gPassword));
  eepromWrite( gFTPpath, eeFTPPATH, sizeof(gFTPpath));
  eepromWrite(    gWxId, eeWXID, sizeof(gWxId));
  eepromWrite(   gMagic, eeMAGIC, sizeof(gMagic));
  eepromWrite(  gFtpSql, eeFTPSQL, sizeof(gFtpSql));
  eepromWrite( gTimeURL, eeTIMEURL, sizeof(gTimeURL));
}

void eepromWrite(uint8_t *target, int offset, int size)
{
  int i;

  // write value to next location
  //=============================
  for (i=0; i<size; i++)
  {
    // write one byte
    //===============
    EEPROM.write(offset + i, target[i]);
  }
}

void eepromWriteByte(int address, uint8_t data)
{
  EEPROM.write(address, data);
}


