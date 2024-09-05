/***********************************************************
File name: 16_thermistor.ino
Description: The information which a thermistor collects 
             temperature is displayed on the LCD1602.
Website: www.adeept.com
E-mail: support@adeept.com
Author: Tom
Date: 2015/05/02 
***********************************************************/
#include <Keypad.h>
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(A2, A1, 10, 11, 12, 13);
int lcdRow = 0;
int lcdCol = 0;

byte empty[8] = {B00000, B00000, B00000, B00000, B00000, B00000, B00000, B00000};

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
  
void setup()
{
  lcd.createChar(0, empty);
  lcd.begin(16, 2);    // set up the LCD's number of columns and rows: 
  lcd.clear();         //Clears the LCD screen and positions the cursor in the upper-left corner 
}

void loop() 
{
    lcd.setCursor(lcdCol, lcdRow); // set the cursor to column 0, line 0
  
  char customKey = customKeypad.getKey();//Read Key data   
  if (customKey){
    if (customKey == 'D')
    {
      lcdCol--;
      lcd.write(byte(0));
    }
    else
    {
    lcd.print(customKey); //send the key data by serial port (UART)

    
    lcdCol++;

    if(lcdCol > 15){
      lcdCol = 0;
      lcdRow++;
      
      if(lcdRow > 1){
        lcdRow = 0;
        lcd.clear();
        }
        }
      }
    } 
} 
