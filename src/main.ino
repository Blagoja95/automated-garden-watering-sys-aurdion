#include <LiquidCrystal.h>
#include <Keypad.h>
#include <DS3231.h>
// GLOBAL
//  2022 version

// set time values;
int h, m, s;
// int SDA = A4;
// int SCL = A5;

// keyboard input
char input;

// default time
float evening = 700.00;
float eveningEnd = 900.00;

// temporery value for manual opening of solenoid valve
float temp = 0;
// reley output pin
int releyCH1 = A3;

void lcdTime();
float getTime();

void openSolenoid();
void closeSolenid();
void solenoidTimer(int tStart, int tEnd);

void menu2();
void updateMenu();
void subMenu();
void exitSubMenu();

void setInterval();
void setClock();
void manualOpen();
void about();
void defaultClear();
void nextValue();

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
	lcd.clear();	  // Clears the LCD screen and positions the cursor in the upper-left corner

	// TIME
	//  Initialize the rtc object
	rtc.begin();
}

// MAIN
void loop()
{

	// Call on first run then add comments on secund run
	//  setTimeInDH(h, m, s);

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

	// For testing
	if (input == 'C')
		lcd.print(getTime());

	// check time and open solenoid
	solenoidTimer(evening, eveningEnd);
}

// FUNCTIONS

// Geting time from main then setting it on display
float getTime()
{
	t = rtc.getTime(); // get time then divede to hour, min and sec
	// add hour, min and sec to one value for easy check
	//  example time: 22:32:15 will be 223215
	return ((t.hour * 100) + t.min) + (float)t.sec / 100;
}

void lcdTime()
{
	t = rtc.getTime();

	float start = getTime();
	float end = start + 0.02;

	while (start <= end)
	{
		lcd.clear();
		lcd.setCursor(4, 1);
		lcd.print(t.hour);
		lcd.print(":");
		lcd.print(t.min);
		lcd.print(":");
		lcd.print((int)t.sec);

		start = getTime();

		delay(400);

		lcd.clear();
	}
}

// call to open solenoid
void openSolenoid()
{
	digitalWrite(releyCH1, LOW);
}
// call to close solenoid
void closeSolenid()
{
	digitalWrite(releyCH1, HIGH);
}

// checking time for sceldueld solenoid opening
void solenoidTimer(float tStart, float tEnd)
{
	float tim = getTime();
	if (tim >= tStart && tim < tEnd)
	{
		while (tim <= tEnd)
		{
			tim = getTime();
			lcd.setCursor(0, 0);
			lcd.print("S");
			lcd.print(tStart);

			lcd.setCursor(1, 0);
			lcd.print("E");
			lcd.print(tEnd);

			lcdTime();
			openSolenoid();
		}
		closeSolenid();
	}

	closeSolenid(); // default action
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
		setClock();
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
	lcd.print("TO DO");
	exitSubMenu();
}

void manualOpen()
{
	defaultClear();
	lcd.print("Valve open10min?");
	lcd.setCursor(2, 1);
	lcd.print("YES=1 NO=3");

	while (input != '3' || input != '1')
	{
		input = inputKeypad.waitForKey();
		Serial.print(input);
	}

	if (input == '1')
	{
		float start = getTime();
		// 10min
		float end = start + 10;
		solenoidTimer(start, end);
		updateMenu();
	}

	if (input == '3')
	{
		updateMenu();
	}

	manualOpen();
}

void about()
{
	defaultClear();
	lcd.print("Boris Blagojevic");
	lcd.setCursor(0, 1);
	lcd.print("Vib+387644151370");
	exitSubMenu();
}

void setClock()
{
	defaultClear();

	// hour
	lcd.print("Hours: ");
	h = inputKeypad.waitForKey();
	lcd.setCursor(1, 0);
	lcd.print("Time: " + h);
	nextValue();

	// minuts
	lcd.print("Minuts: ");
	m = inputKeypad.waitForKey();
	lcd.setCursor(1, 0);
	lcd.print("Time: " + m);
	nextValue();

	// secunds
	lcd.print("Secunds: ");
	m = inputKeypad.waitForKey();
	lcd.setCursor(1, 0);
	lcd.print("Time: " + s);
	nextValue();

	setTimeInDH(h, m, s);
	exitSubMenu();
}

void defaultClear()
{
	lcd.clear();
	lcd.setCursor(0, 0);
}

void exitSubMenu()
{
	while (input != 'D')
	{
		input = inputKeypad.getKey();
	}
	if (input == 'D')
	{
		menu = 0;
		updateMenu();
	}
}

void nextValue()
{
	while (input != 'B')
		input = inputKeypad.waitForKey();
	if (input == 'B')
	{
		defaultClear();
	}
}
