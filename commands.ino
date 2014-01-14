//=====================================================================
// commands.ino
//
//  Created on: 25 Jul 2011
//      Author: Mike McPherson
//=====================================================================

// define some characters
//=======================
#define null (char)0
#define CR '\r'
#define LF '\n'

static int freeRAM(void);
void listSettings(void);

// command interface when connected to PC
//=======================================
void checkCommands(void)
{
  char charCom;
  char parameter;
  int i, j;

  charCom = getChar();
  while (charCom != null)
  {
    switch (charCom)
    {
    case '?':
      showVersion();
      showCommands();
      break;
      
    case 'd':
      // set rtc date and time
      //======================
      setDateTime();
      break;

    case 'e':
      // EEPROM functions
      // next character says which action
      //====================================
      parameter = skipChars(' ');
      if (parameter==CR || parameter==LF)
      {
        Serial.println(F("eeprom?"));
        break;
      }
      switch (parameter)
      {
        // clear eeprom
        //==================
        case 'c':
        Serial.println("EEEPROM clear");
        for (i=0; i<256; i=i+1)
          eepromWriteByte(i, 0xFF);
        Serial.println(F("EEPROM cleared"));
        break;

        case 'd':
        // dump EEPROM
        //============
        Serial.println("EEEPROM dump");
        eepromDump();
        break;
        
        case 'r':
        // read all from EEPROM
        //=====================
        Serial.println("EEEPROM read settings");
        eepromReadAll();
        Serial.println(F("settings read from EEPROM"));
        listSettings();
        break;

        case 's':
        // saveall to EEPROM
        //====================
        Serial.println("EEEPROM save settings");
        eepromSaveAll();
        Serial.println(F("settings saved to EEPROM"));
        break;
      }
      break;
      
    case 'f':    
      // flash functions
      // next character says which action
      //====================================
      parameter = skipChars(' ');
      if (parameter==CR || parameter==LF)
      {
        Serial.println(F("flash?"));
        break;
      }
      switch (parameter)
      {
      case 'c':
        // erase entire flash chip
        //========================
        flashClear();
        break;

      case 'd':
        // dump flash page
        //================
        
        // get page number
        //================
        charCom = ' ';
        i = getNumber(&charCom);
        if (i >= 0)
          curPage = i;
        flashDump(curPage);
        curPage++;
        break;
        
      case 'e':
        // erase flash page
        //=================
        
        // get page number
        //================
        charCom = ' ';
        i = getNumber(&charCom);
        if (i >= 0)
          curPage = i;
        flashErase(curPage);
        curPage++;
        break;

      case 'f':
        // fill flash page
        //================
        
        // get page
        //=========
        charCom = ' ';
        i = getNumber(&charCom);
        
        // get fill
        //=========
        charCom = ' ';
        j = getNumber(&charCom); 
        flashFill(i, j);
        curPage++;
        break;

      case 'r':
        // read flash page
        //================
        
        // get page number
        //================
        charCom = ' ';
        i = getNumber(&charCom);
        if (i >= 0)
          curPage = i;
        flashRead(curPage);
        curPage++;
        break;
      }
      break;
    
    case 'g':
      //toggle grove
      //============
      digitalWrite(GROVE, !digitalRead(GROVE));
      break;
      
    case 'h':
      // set epoch
      //==========
      setEpoch();
      break;
        
    case 'l':
      // toggle loop
      gLoop = !gLoop;
      break;
      
    case 's':
      // list settings
      //==============
      listSettings();
      break;

    // parameter setting
    //==================
    case 'p':
      // set parameter
      // next character says which parameter
      // followed by value to set
      //====================================
      parameter = skipChars(' ');
      if (parameter==CR || parameter==LF)
      {
        Serial.println(F("parameter?"));
        break;
      }

      switch (parameter)
      {
      case 'a':
      // set APN
      //========
      setString(&gAPN[0], sizeof(gAPN));
      listSettings();
      break;
      
      case 'c':
      // set FTP SQL command
      //====================
      setString(&gFtpSql[0], sizeof(gFtpSql));
      listSettings();
      break;
      
      case 's':
      // set server name
      //================
      setString(&gServer[0], sizeof(gServer));
      listSettings();
      break;
      
      case 'u':
      // set UserName
      //=============
      setString(&gUserName[0], sizeof(gUserName));
      listSettings();
      break;

      case 'p':
      // set Password
      //=============
      setString(&gPassword[0], sizeof(gPassword));
      listSettings();
      break;

      case 'f':
      // set FTPpath
      //============
      setString(&gFTPpath[0], sizeof(gFTPpath));
      listSettings();
      break;

      case 'm':
      // set Magic
      //============
      setString(&gMagic[0], sizeof(gMagic));
      listSettings();
      break;

      case 't':
      // set timeURL
      //============
      setString(&gTimeURL[0], sizeof(gTimeURL));
      listSettings();
      break;

      case 'w':
      // set WxId
      //=========
      setString(&gWxId[0], sizeof(gWxId));
      listSettings();
      break;

      default:
        Serial.print(parameter);
        Serial.println(F("??"));
        break;
      }
      break;
    
    case 'z':
      // sleep for x seconds
      //====================
      charCom = ' ';
      i = getNumber(&charCom);      
      sleep(i);
      break;

    case '\r':
      break;

    case '\n':
      break;

    default:
      Serial.print(charCom);
      Serial.println("??");
      break;
    }
    charCom = getChar();
  }
}

// Returns the number of bytes currently free in RAM
//==================================================
static int freeRAM(void) 
{
  extern int  __bss_end;
  extern int* __brkval;
  int free_memory;
  if (reinterpret_cast<int>(__brkval) == 0) 
  {
    // if no heap use from end of bss section
    //=======================================
    free_memory = reinterpret_cast<int>(&free_memory) - reinterpret_cast<int>(&__bss_end);
  } else 
  {
    // use from top of stack to heap
    //==============================
    free_memory = reinterpret_cast<int>(&free_memory) - reinterpret_cast<int>(__brkval);
  }
  return free_memory;
}

void listSettings(void)
{
  // show version to serial monitor
  //===============================
  showVersion();
  
  // list settings
  //==============
  Serial.print("     APN: "); Serial.println((char*) gAPN);
  Serial.print("  server: "); Serial.println((char*) gServer);
  Serial.print("userName: "); Serial.println((char*) gUserName);
  Serial.print("passWord: "); Serial.println((char*) gPassword);
  Serial.print(" FTPpath: "); Serial.println((char*) gFTPpath);
  Serial.print("    wxID: "); Serial.println((char*) gWxId);
  Serial.print("   magic: "); Serial.println((char*) gMagic);
  Serial.print("  FtpSql: "); Serial.println((char*) gFtpSql);
  Serial.print(" timeURL: "); Serial.println((char*) gTimeURL);
}

void resetFactoryDefaults(void)
{
  // set and save initial values
  //============================
}

void showVersion(void)
{
  Serial.println();
  Serial.print(F("dataflashTest XRF"));
  Serial.print(' ');
  Serial.println(F("v1.02"));
  
  showTimestamp();

  Serial.print(F("free RAM: ")); 
  Serial.print(freeRAM()); 
  Serial.println(F(" bytes"));
  Serial.println();
}

void showCommands(void)
{
  Serial.println(F("d yy mm dd hh nn ss x = set RTC to yy/mm/dd hh:nn:ss day of week = x"));
  Serial.println(F("ec    = eeprom clear"));
  Serial.println(F("ed    = eeprom hex dump"));
  Serial.println(F("er    = eeprom read all settings"));
  Serial.println(F("es    = eeprom save all settings"));
  Serial.println(F("fc    = flash clear entire chip"));
  Serial.println(F("fex   = flash erase page x"));
  Serial.println(F("fdx   = flash hex dump page x"));
  Serial.println(F("ffx y = flash fill page x with y"));
  Serial.println(F("frx   = flash read page x"));
  Serial.println(F("g     = toggle grove"));
  Serial.println(F("h xxx = set epoch"));
  Serial.println(F("l     = loop toggle"));
  Serial.println(F("s     = list settings"));
  Serial.println(F("p a   = set APN"));
  Serial.println(F("p c   = set FTP SQL command"));
  Serial.println(F("p s   = set server name"));
  Serial.println(F("p u   = set username"));
  Serial.println(F("p p   = set password"));
  Serial.println(F("p f   = set FTP path"));
  Serial.println(F("p m   = set magic header prefix"));
  Serial.println(F("p t   = set getTime URL"));
  Serial.println(F("p w   = set wx station id"));
  Serial.println(F("zx    = sleep for x seconds"));
}

