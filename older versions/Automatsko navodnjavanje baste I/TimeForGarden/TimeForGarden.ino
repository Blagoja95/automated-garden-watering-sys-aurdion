
#include <DS3231.h>

      /*prototypes*/
int getTimeInt();

//for time
int tim;

Time t;

// Init the DS3231 using the hardware interface
DS3231  rtc(SDA, SCL);

void setup()
{
        /*SETTING CLOCK*/
        
  // Setup Serial connection
  Serial.begin(9600);
  
  // Initialize the rtc object
  rtc.begin();

  //Set on first run then add comments on secund run
  //rtc.setTime(13, 48, 0); 

        /*SETTING SEVEN SEGMENT DISPLAY

  byte numDigits = 4;
  
 //4 digits settup
 byte digitPins[] = {12, 11, 10, 9};

//7 segment display settup
byte segmentPins[] = {1, 2, 3, 4, 5, 6, 7, 8};


sevseg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins);

sevseg.setBrightness(90);
  */
}

void loop()
{
  tim = getTimeInt();
  Serial.println(tim);

  
  //delay 1s 
  delay(1000);
  
}

int getTimeInt(){
  t = rtc.getTime();
  int tim;
  int a = t.hour;
  int b = t.min;

  tim = a * 100 + b;

  return tim;
}
