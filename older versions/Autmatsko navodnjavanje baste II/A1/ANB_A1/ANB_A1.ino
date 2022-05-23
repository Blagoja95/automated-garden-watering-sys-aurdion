#include <LiquidCrystal.h>
#include <Keypad.h>
#include <DS3231.h>
//GLOBAL *********************************************

//value1
float evening = 800; //Start
float eveningEnd = 1030; //Stop

int tim; // global value 

//reley output pin
int releyCH1 = A3;

//*****************************************************

//PROTOTYPES ******************************************

void lcdTime();
void openSolenoid();
void closeSolenid();
void timeForSolenoid();
void menu2();
void manualopen();
void setTimeLCD();
void timeForScheduledOpenStart(float i);
void timeForScheduledOpenClose(float i);
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
//  float tim; //gloobal variable used by almost all functions
  Time t;


// Init the DS3231 using the hardware interface
  DS3231 rtc(SDA, SCL);
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
//  Serial.begin(9600);
  
  //Set on first run then add comments on secund run
// rtc.setTime(23, 17, 30);
  //*************************************************
}

//MAIN_____________________________________________-----------------------____________________________-----------------_____________------------------_________________
void loop() {
lcdTime(); //home menu
timeForSolenoid();
 }
//###################################################################################################################

//FUNCTIONS

//Geting time from main then setting it on display
void lcdTime(){
  t = rtc.getTime();  //get time then divede to hour, min and sec
  int a = t.hour; 
  int b = t.min;

 //add hour, min and sec to one value for easy check
 tim = a * 100 + b;

  //printin time on lcd
  lcd.print(rtc.getTimeStr());
  delay(300);
  lcd.clear();

//  Serial.println(tim);
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
   
    while(tim < eveningEnd)
    {
       lcdTime();
      openSolenoid();
     
    }
    closeSolenid();

  }
  closeSolenid(); //default action
  }
  
