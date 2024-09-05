#include <LiquidCrystal.h>
#include <Keypad.h>
#include <DS3231.h>
// GLOBA
//  2022 version

// set time values;
int h, m, s;

// keyboard input
char input;

// value1
float evening = 918.00;
float eveningEnd = 919.00;

// temporery value for manual opening of solenoid valve
float temp = 0;
// reley output pin
int releyCH1 = A3;

void lcdTime();
int getTime();

void openSolenoid();
void closeSolenid();
void timeForSolenoid();

void menu2();
void updateMenu();
void subMenu();

void setInterval();
void manualOpen();
void about();
void defaultClear();

void setTimeInDH(int h, int m, int s);

// LCD
LiquidCrystal lcd(A2, A1, 10, 11, 12, 13);
int lcdRow = 0;
int lcdCol = 0;
int menu = 1;

// KEYPAD
// byte empty[8] = {B00000, B00000, B00000, B00000, B00000, B00000, B00000, B00000};

const byte ROWS = 4; // four rows
const byte COLS = 4; // four columns
// define the cymbols on the buttons of the keypads

char hexaKeys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

byte rowPins[ROWS] = {9, 8, 7, 6}; // connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; // connect to the column pinouts of the keypad

// initialize an instance of class NewKeypad
Keypad inputKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// TIME
float tim; // gloobal variable used by almost all functions
Time t;

// Init the DS3231 using the hardware interface
DS3231 rtc(SDA, SCL);

void setup()
{
  // seting reley pin A3 to ouput
  pinMode(releyCH1, OUTPUT);

  // LCD
  // lcd.createChar(0, empty);
  lcd.begin(16, 2); // set up the LCD's number of columns and rows:
  lcd.clear();      // Clears the LCD screen and positions the cursor in the upper-left corner

  // TIME
  //  Initialize the rtc object
  rtc.begin();
}

// MAIN
void loop()
{

  // Call on first run then add comments on secund run
  //  setTimeInDH(h, m, s);
  //  setTimeInDH(9, 16, 40);

  // Show time on lcd
  //  lcdTime();
  lcd.setCursor(lcdCol, lcdRow);
  input = inputKeypad.getKey();

  if (input != NO_KEY)
  {
    if (input == '8')
    {
      menu++;
      updateMenu();
      delay(100);
    }

    if (input == '2')
    {
      menu--;
      updateMenu();
      delay(100);
    }

    if (input == '5')
      subMenu();
  }

  if (input == 'A')
    lcdTime();

  if (input == 'D')
    updateMenu();
}

// FUNCTIONS

// Geting time from main then setting it on display
int getTime()
{
  t = rtc.getTime(); // get time then divede to hour, min and sec
  // add hour, min and sec to one value for easy check
  return t.hour * 100 + t.min + ((int)t.sec / 100);
}

void lcdTime()
{
  t = rtc.getTime();

  float start = t.sec;
  float end = start + (float)10;
  // lcd.print(start);
  // lcd.setCursor(0, 1);
  // lcd.print(end);
  // delay(500);

  // printin time on lcd
  while (start <= end)
  {
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print(t.hour);
    lcd.print(":");
    lcd.print(t.min);
    lcd.print(":");
    lcd.print((int)t.sec);
    start = t.sec;
    delay(200);
    lcd.clear();
  }
}

void setTimeInDH(int h, int m, int s)
{
  rtc.setTime(h, m, s);
}

void updateMenu()
{
  switch (menu)
  {
  case 0:
    lcd.clear();
    lcd.print(" Manually open");
    lcd.setCursor(0, 1);
    lcd.print(">About");
    menu = 4;
    break;

  case 1:
    lcd.clear();
    lcd.print(">Change time");
    lcd.setCursor(0, 1);
    lcd.print(" Set interval");
    break;

  case 2:
    lcd.clear();
    lcd.print(" Change time");
    lcd.setCursor(0, 1);
    lcd.print(">Set interval");
    break;

  case 3:
    lcd.clear();
    lcd.print(">Manually open");
    lcd.setCursor(0, 1);
    lcd.print(" About");
    break;

  case 4:
    lcd.clear();
    lcd.print(" Manually open");
    lcd.setCursor(0, 1);
    lcd.print(">About");
    break;

  case 5:
    lcd.clear();
    lcd.print(">Change time");
    lcd.setCursor(0, 1);
    lcd.print(" Set interval");
    menu = 1;
    break;
  }
}

void subMenu()
{
  switch (menu)
  {
  case 1:
    // setTimeInDH(h, m, s);
    break;

  case 2:
    setInterval();
    break;

  case 3:
    manualOpen();
    break;

  case 4:
    about();
    break;
  }
}

void setInterval()
{
  defaultClear();
  lcd.print("Set interval");
}

void manualOpen()
{
  defaultClear();
  lcd.print("Valve start: ");
}

void about()
{
  defaultClear();
  lcd.print("Boris Blagojevic");
  lcd.setCursor(0, 1);
  lcd.print('Vib+387644151370');
}

void defaultClear()
{
  lcd.clear();
  lcd.setCursor(0, 0);
}
