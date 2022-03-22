#include <LiquidCrystal.h>
#include <Keypad.h>
#include <DS3231.h>
//GLOBAL *********************************************
// 2021 version

//value1
float morning = 600.00;
float morningEnd = 800.00;

float evening = 2000.00;
float eveningEnd = 2200.00;

//temporery value for manual opening of solenoid valve
float temp = 0;
//reley output pin
int releyCH1 = A3;
//****************************************************

//PROTOTYPES ******************************************

void lcdTime();
void openSolenoid();
void closeSolenid();
void timeForSolenoid();
void menu2();
void manualopen();
//*****************************************************

//LCD **************************************************
LiquidCrystal lcd(A2, A1, 10, 11, 12, 13);
//int lcdRow = 0;
//int lcdCol = 0;
//******************************************************

//KEYPAD ***********************************************
//byte empty[8] = {B00000, B00000, B00000, B00000, B00000, B00000, B00000, B00000};

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad
 
//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
//******************************************************

//TIME**************************************************
  float tim; //gloobal variable used by almost all functions
  Time t;


// Init the DS3231 using the hardware interface
  DS3231  rtc(SDA, SCL);
//******************************************************
void setup() {

  //seting reley pin A3 to ouput 
  pinMode(releyCH1, OUTPUT);
  
  //LCD
  //lcd.createChar(0, empty);
  lcd.begin(16, 2);    // set up the LCD's number of columns and rows: 
  lcd.clear();         //Clears the LCD screen and positions the cursor in the upper-left corner
  //*****************************************************************************

  //TIME
  // Initialize the rtc object
  rtc.begin();

  //*************************************************
}

//MAIN ###############################################################################################################
void loop(){

 //Call on first run then add comments on secund run

 /*
  int h = 10;
  int m = 29;
  int s = 5;

  setTimeInDH(h, m, s);*/
  
//Show time on lcd
lcdTime();


//Check if time is right, if it is then open valve and wait for right time to stop
timeForSolenoid();
}
//###################################################################################################################

//FUNCTIONS

//Geting time from main then setting it on display
void  lcdTime(){
  t = rtc.getTime();  //get time then divede to hour, min and sec
  int a = t.hour; 
  int b = t.min;
  float c = t.sec;


 //add hour, min and sec to one value for easy check
 tim = a * 100 + b + (c / 100);

  //printin time on lcd
  lcd.setCursor(4, 0);
  lcd.print(a);
  lcd.print(":");
  lcd.print(b);
  lcd.print(":");
  lcd.print((int)c);
  delay(100);
  lcd.clear();

  //return tim;
}
//call to open solenoid
void openSolenoid()
{
  digitalWrite(releyCH1, LOW);
}
//call to close solenoid
void closeSolenid()
{
  digitalWrite(releyCH1, HIGH);
}

//checking time for sceldueld solenoid opening
void timeForSolenoid()
{
  if(tim >= evening && tim < eveningEnd)
  {
   
    while(tim <= eveningEnd)
    {
       lcdTime();
      openSolenoid();
     
    }
    closeSolenid();

  }
  else if (tim >= morning && tim < morningEnd)
  {
    while(tim <= morningEnd)
    {
       lcdTime();
      openSolenoid();
     
    }
    closeSolenid();
  }
  closeSolenid(); //default action
  }

  void setTimeInDH(int h, int m, int s){

    rtc.setTime(h, m, s);
  }
