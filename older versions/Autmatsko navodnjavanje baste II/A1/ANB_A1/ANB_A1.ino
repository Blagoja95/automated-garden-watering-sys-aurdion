#include <LiquidCrystal.h>
#include <Keypad.h>
#include <DS3231.h>
//GLOBAL *********************************************
//time value
int h;  //HOURS
int m; //MINS
int s; //SECUNDS

//value1
float evening; //Start
float eveningEnd; //Stop

//value2, used for manual 
float manualStart;
float manualEnd;


//temporery value for manual opening of solenoid valve
float temp = 0;
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

//MENU___________________________________________________________________
int p = 1; //used to remember position in menus
//***********************************************************************

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

  //Set on first run then add comments on secund run
 //rtc.setTime(17, 15, 00);
  //*************************************************
}

//MAIN_____________________________________________-----------------------____________________________-----------------_____________------------------_________________
void loop() {

//lcdTime(); //home menu
timeForSolenoid();


char customKey = customKeypad.getKey();//Read Key data if typed
  if (customKey){
    if (customKey == '6')
    {
      if(p > 3)
      {
        p = 0;
        
      }
      switch(p){

      case 0: //home menu
      lcdTime();
      break;

      case 1:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Zakazano: " + (int)evening);
      lcd.setCursor(0, 1);
      lcd.print("Do: " + (int)eveningEnd);

      
        if(customKey == 'D')
        {
          float i;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Unesi pocetak:");
          do
          {
            lcd.setCursor(4, 1);
            customKey = customKeypad.getKey(); //get custom key from keyboard
            lcd.print(customKey);
          }
          while(customKey != 'D');
          
          i = customKey; //assign key to new variable
          
          timeForScheduledOpenOpen(i);

          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Unesi vrijeme: ");
          do
          {
            lcd.setCursor(4, 1);            
            customKey = customKeypad.getKey(); //get custom key from keyboard
            lcd.print(customKey);
          }
          while(customKey != 'D');
          
         timeForScheduledOpenClose(i);
        }
        else{
          delay(2000);
          p = 0;
        }
      break;

      case 2:
      lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Menu 3");
      break;

      case 3:
      lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Menu4");
      break;
      }
    }
 }
}
//###################################################################################################################

//FUNCTIONS

//This function set automatic opening time for solenoid
void timeForScheduledOpenOpen(float i)
{
  evening = i;
}

void timeForScheduledOpenClose(float i)
{
  eveningEnd = i;
}

void setTimeLCD(){
    rtc.setTime(h, m, s);
}

//Geting time from main then setting it on display
void   lcdTime(){
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
  delay(10);
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
  closeSolenid(); //default action
  }

  //for manual opening of solenoid valve
  void manualopen(){
    manualStart = tim;
    manualEnd = manualStart + tim;
  
    if(manualStart >= manualEnd && manualStart < manualEnd)
  {
   
    while(manualStart <= manualEnd)
    {
      lcdTime();
      lcd.setCursor(2, 1);
      lcd.print("Kraj: " + (int)manualEnd);
      openSolenoid();
     
    }
    closeSolenid();

  }
 
  closeSolenid(); //default action
  } 
  void menu2(){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Rucno otvori cesm");
    lcd.setCursor(2, 1);

    char customKey = customKeypad.getKey();//Read Key data if typed
      if (customKey){
        if (customKey == '2')
        {
          if(temp <= 60)
          {        
              temp += 15;
              lcd.print("Na: " + (int)temp);
          }
          else
          {
            temp = 0;
            lcd.print("Na: " + (int)temp);
          }
        }
        else if (customKey == '8')
        {
            if (temp > 0)
            {
              temp -= 15;
              lcd.print("Na: " + (int)temp);
            }
            else
            {
              temp = 60;
              lcd.print("Na: " + (int)temp);
            }       
        }
        else if (customKey == '5')
        {
          manualopen();
        }
      }
      
    
  }
  
