//==============================================================
// initialise communication with flash chip
//==============================================================
void flashInit(void)
{
  dflash.init(DF_MISO, DF_MOSI, DF_SPICLOCK, DF_SELECT);
}

//==============================================================
// clear entire flash chip
//==============================================================
void flashClear(void)
{
  Serial.print("chip erase...");
  dflash.chipErase();
  Serial.println("completed");
}

//==============================================================
// hex dump one page
//==============================================================
void flashDump(int page)
{
  int line;
  int i;
  
  Serial.print("dump page ");
  Serial.println(page);
  
  // read page into flash RAM
  //=========================
  dflash.readPageToBuf1(page);
  
  for (line=0; line<32; line++)
  {
    // read line from flash
    //=====================
    for (i=0; i<16; i++)
      lineBuffer[i] = dflash.readByteBuf1(line*16+i);
    flashDumpLine(line, lineBuffer);
  }
  Serial.println();
}

//==============================================================
// hex dump one line
//==============================================================
void flashDumpLine(int line, uint8_t *buffer)
{
  int col;
  int data;

  if (line<16)
    Serial.print("0");
  Serial.print(line, HEX);
  Serial.print(": ");
  for (col=0; col<16; col++)
  {
    data = buffer[col];
    if (data < 16)
      Serial.print("0");
    Serial.print(data, HEX);
    Serial.print(" ");       
  }
  Serial.println();
}

//==============================================================
// erase one page of flash
//==============================================================
void flashErase(int page)
{
  // erase a page of flash
  //======================
  Serial.print("erase page ");
  Serial.println(page);

  dflash.pageErase(page);
}

//==============================================================
// fill one page with "data"
//==============================================================
void flashFill(int page, uint8_t data)
{
  // initalise flash by writing data to all bytes
  //=============================================
  int i;
  
  Serial.print("fill page ");
  Serial.print(page);
  Serial.print(" with: ");
  Serial.println(data);
  
 // first erase the page
  //=====================
  flashErase(page);
  
  // fill flash internal buffer with data
  //=====================================
  for(i=0; i<512; i++)
    flashWriteByte(i, data);

  // flush flash internal buffer to flash
  //=====================================
  flashFlushPage(page);
}

//==============================================================
// flush one page from its internal buffer to flash
//==============================================================
void flashFlushPage(int page)
{
  // write a page from internal buffer to flash
  //===========================================
  dflash.writeBuf1ToPage(page);
}

//==============================================================
// read and format one page
//==============================================================
void flashRead(int page)
{
  int record;
  float batteryVoltage;

  myHeader headerRecord;
  myRecord dataRecord;
  
  Serial.print("read page ");
  Serial.println(page);
  
  // get header
  //===========
  readPageHeader(page, &headerRecord);
  separatorPrint(40);

  // show timestamp
  //===============
  timeNow = DateTime(headerRecord.ts);
  Serial.print(F("   timestamp: "));
  formatTimestamp();
  
  // show data version
  //==================
  Serial.print(F("data version: ")); Serial.println(headerRecord.version);
  
  // show "magic"
  //=============
  Serial.print(F("magic string: ")); Serial.println(headerRecord.magic);
  separatorPrint(40);

  // show records
  //=============
  for (record=0; record<15; record++)
  {
    // read record
    //============
    if (readPageNthRecord(page, record, &dataRecord))
    {
      // show timestamp
      //===============
      timeNow = DateTime(dataRecord.ts);
      Serial.print(F("   timestamp: "));
      formatTimestamp();
      
      // show RTC temperature
      //=====================
      Serial.print(F("         RTC: ")); Serial.print(dataRecord.temperatureRTC/10.0, 1); Serial.println(F(" C"));
      
      // show battery voltage
      // This pin is connected to the middle of a 10M and 2M resistor
      // that are between Vcc and GND.
      // So actual battery voltage is:
      //    (<adc value> * Aref / 1023) * (R1+R2) / R2
      //=============================================================
      batteryVoltage = (dataRecord.batteryVoltage * ADC_AREF / 1023) * (BATVOLT_R1+BATVOLT_R2) / BATVOLT_R2;
      Serial.print(F("     battery: ")); Serial.print(batteryVoltage); Serial.println(F(" volts"));
      
      // show pressure
      //==============
      Serial.print(F(" temperature: ")); Serial.print(dataRecord.temperatureP/10.0, 1); Serial.println(F(" C"));
      Serial.print(F("    pressure: ")); Serial.print(dataRecord.pressure); Serial.println(F(" hPa"));
      
      // show Wind
      //==========
      Serial.print(F("        wind: ")); Serial.println(dataRecord.wind_ticks);
      
      // show humidity
      //==============
      Serial.print(F(" temperature: ")); Serial.print(dataRecord.temperatureH/10.0, 1); Serial.println(F(" C"));
      Serial.print(F("    humidity: ")); Serial.print(dataRecord.humidity/10.0, 1); Serial.println(F(" %"));
      separatorPrint(40);
    }
  }
  Serial.println();
}

//=====================================================================================
// print separator
//=====================================================================================
void separatorPrint(int count)
{
  // print a separator
  //==================
  for (int i = 0; i<count; i++)
  Serial.print("=");
  Serial.println();
}

//==============================================================
// write one byte to flash internal buffer
//==============================================================
void flashWriteByte(int address, uint8_t data)
{
  // write one byte of data to flash internal buffer
  //================================================
  dflash.writeByteBuf1(address, data);
}

