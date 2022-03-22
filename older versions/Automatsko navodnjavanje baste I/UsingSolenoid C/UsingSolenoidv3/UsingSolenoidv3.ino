#include <DS3231.h>
#include <SevSeg.h>

      /*prototypes*/
int getTimeInt();
void printSevSeg(int tim);
void openSolenoid();
void closeSolenid();
void timeForSolenoid(int tim);
//void maunallyOpenSolenoidValve(int tim);
//bool checkTiltSensor();
//#################################################

//state for tilt sensor
//bool stateOfSensor = false;

//reley output pin
int releyCH1 = A3;

//tilt sensor used to check wather level
//int tiltSensor = A0;
//#################################################

        //FOR TIME
  int tim;
  Time t;
//#################################################

// Init the DS3231 using the hardware interface
  DS3231  rtc(SDA, SCL);

        //SEVEN SEGMENT DISPLAY
  SevSeg sevseg;

void setup()
{
  //seting reley pin A3 to ouput 
  pinMode(releyCH1, OUTPUT);

  //tilt switch settup
  //pinMode(tiltSensor, INPUT_PULLUP);
  
 
        /*SETTING CLOCK*/
        
  // Setup Serial connection
  //Serial.begin(9600);
  
  // Initialize the rtc object
  rtc.begin();

  //Set on first run then add comments on secund run
  rtc.setTime(18, 20, 00); 
//#################################################

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
  getTimeInt();

  //Only for testing
  //Serial.println(tim);

  //Sending time to seven segment display
  printSevSeg(tim);

  //solenoid valve
  timeForSolenoid(tim);

  //manually open the valve
//  maunallyOpenSolenoidValve();

  
  
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

//Function for opening solenoid valve when time is right
void openSolenoid()
{
  digitalWrite(releyCH1, LOW);
}
void closeSolenid()
{
  digitalWrite(releyCH1, HIGH);
}
//This function check time that is manualy added in this function, if time is right open solenoid with Low signal
//then wait for the end time and then close valve with High signal
void timeForSolenoid(int tim)
{
 /* //Time to pump water in morning
  int morning = 800;
  int moningEnd = 930;*/

  //Time to pump water in evning;
  int evening = 2000;
  int eveningEnd = 2200;

  if(tim >= evening && tim <= eveningEnd)
  {
   
    while((tim = getTimeInt()) < eveningEnd)
    {
      printSevSeg(tim);
      openSolenoid();
      //stateOfSensor = checkTiltSensor();

     /* if((stateOfSensor = checkTiltSensor()) == true);
      {
        closeSolenid();
        stateOfSensor = false;
        break;
      }*/
    }
    closeSolenid();
  }
  closeSolenid(); //default action
  }
  
//checking sensor value to stop water overflowing
/*bool checkTiltSensor()
{
  int stateTilt = 0;

  stateTilt = digitalRead(tiltSensor);

  if(stateTilt == LOW)
  {
    return true;
  }
    else
    {
      return false;
    }
}*/
