#include <DS3231.h>
#include <SevSeg.h>

      /*prototypes*/
int getTimeInt();
void printSevSeg(int tim);

        //FOR TIME
  int tim;

  Time t;

// Init the DS3231 using the hardware interface
  DS3231  rtc(SDA, SCL);

        //SEVEN SEGMENT DISPLAY
  SevSeg sevseg;

void setup()
{
        /*SETTING CLOCK*/
        
  // Setup Serial connection
  //Serial.begin(9600);
  
  // Initialize the rtc object
  rtc.begin();

  //Set on first run then add comments on secund run
  //rtc.setTime(13, 42, 40); 

        /*SETTING SEVEN SEGMENT DISPLAY*/

  byte numDigits = 4;
  
 //4 digits settup
 byte digitPins[] = {13, 12, 11, 10};

  //7 segment display settup
  byte segmentPins[] = {2, 3, 4, 5, 6, 7, 8, 9};

  bool resistorsOnSegments = false; // 'false' means resistors are on digit pins
  byte hardwareConfig = COMMON_CATHODE; // See README.md for options
  bool updateWithDelays = false; // Default 'false' is Recommended
  bool leadingZeros = true; // Use 'true' if you'd like to keep the leading zeros
  bool disableDecPoint = true; // Use 'true' if your decimal point doesn't exist or isn't connected
  
  //sevseg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins);

  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments,
  updateWithDelays, leadingZeros, disableDecPoint);
  
  sevseg.setBrightness(90);
  
}

void loop()
{
  //geth the time in integer
  tim = getTimeInt();

  //Only for testing
  //Serial.println(tim);

  //Sending time to seven segment display
  printSevSeg(tim);
  
  //delay 1s 
  //delay(1000);
  
}
//Geting time from RTC then calculating it and sending it to main
int getTimeInt(){
  t = rtc.getTime();
  int tim;
  int a = t.hour;
  int b = t.min;

  tim = a * 100 + b;

  return tim;
}
//Geting time from main then setting it on display
void printSevSeg(int tim){

  sevseg.setNumber(tim);

  sevseg.refreshDisplay();
}
