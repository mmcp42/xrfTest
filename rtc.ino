//=================================
//RTC
//=================================
#include <Wire.h>
#include <Sodaq_DS3231.h>

extern DateTime timeNow;

void rtcInit(void)
{
  // initialise the clock  
  //=====================
  rtc.begin();
}

void setDateTime(void)
{
  // set date/time/dayOfWeek
  //========================
  DateTime dt;

  char charCom;
  int year = 0;
  int month = 0;
  int day = 0;
  int hour = 0;
  int minute = 0;
  int second = 0;
  int dayOfWeek = 0;
  
  // get year
  //=========
  charCom = ' ';
  year = getNumber(&charCom);

  // get month
  //==========
  charCom = ' ';
  month = getNumber(&charCom);
  
  // get day
  //========
  charCom = ' ';
  day = getNumber(&charCom);
  
  // get hour
  //=========
  charCom = ' ';
  hour = getNumber(&charCom);
  
  // get minute
  //===========
  charCom = ' ';
  minute = getNumber(&charCom);
  
  // get second
  //===========
  charCom = ' ';
  second = getNumber(&charCom);
  
  // get dayOfWeek
  //==============
  charCom = ' ';
  dayOfWeek = getNumber(&charCom);
 
  dt = DateTime(year, month, day, hour, minute, second, dayOfWeek);

  // set the date/time
  //==================
  rtc.setDateTime(dt);

  showTimestamp();
}

//=====================================================================================
// routine to format and print time
//=====================================================================================
void formatTime(void)
{
  int hour;
  int minute;
  int second;
  
  // show date/time
  //===============
  hour = timeNow.hour();
  minute = timeNow.minute();
  second = timeNow.second();

  if (hour<10)
    Serial.print("0");
  Serial.print(hour, DEC);
  Serial.print(':');
  if (minute<10)
    Serial.print("0");
  Serial.print(minute, DEC);
  Serial.print(':');
  if (second<10)
    Serial.print("0");
  Serial.print(second, DEC);
  Serial.print(' ');
}

//=====================================================================================
// routine to format and show timestamp
//=====================================================================================
void formatTimestamp(void)
{
  static char months[][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec", "xxx"};

  int day;
  int month;
    
  // show date/time
  //===============
  day = timeNow.date();
  month = timeNow.month();

  if (month<1)
    month = 1;
  if (month>12)
    month = 12;
  
  if (day<10)
    Serial.print("0");
  Serial.print(day, DEC);
  Serial.print('-');
  Serial.print(months[month-1]);
  Serial.print('-');
  Serial.print(timeNow.year(), DEC);
  Serial.print(' ');
  
  formatTime();
  Serial.println();
}

//=====================================================================================
// set epoch
//=====================================================================================
void setEpoch(void)
{
  uint32_t time;
  char charCom;

  charCom = ' ';
  time = getNumber(&charCom);
  Serial.print(F("epoch: ")); Serial.println(time);
  if (time > 0)
    rtc.setEpoch(time);
  showTimestamp();
}

//=====================================================================================
// routine to show timestamp
//=====================================================================================
void showTimestamp(void)
{
  Serial.println("showTimestamp()");

  timeNow = rtc.now();
  formatTimestamp();
}


