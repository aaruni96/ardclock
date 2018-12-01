// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

long int starth,startm,starts,rawtime,seconds,minutes,hours,days;
String h,m,s;

void setup()
{
	//set initial time
	starth=23;
	startm=33;
	starts=15;
	// set up the LCD's number of columns and rows: 
	lcd.begin(16, 2);
	// Print a message to the LCD.
	lcd.print("I Have Awoken!");
	Serial.begin(9600);
  
}



void loop() 
{
	lcd.setCursor(0, 1);
	rawtime=millis()/1000+( (starth*60+startm)*60+starts);
	//discard the milliseconds
	seconds=rawtime%60;
	rawtime=rawtime/60;
	minutes=rawtime%60;
	rawtime=rawtime/60;
	hours=rawtime%24;
	if(seconds<10)
	{
		s="0"+String(seconds);
	}
	else
	{
		s=String(seconds);
	}
	if(minutes<10)
	{
		m="0"+String(minutes);
	}
	else
	{
		m=String(minutes);
	}
	if(hours<10)
	{
		h="0"+String(hours);
	}
	else
	{
		h=String(hours);
	}
	if(seconds%2)
		lcd.print(h+":"+m+":"+s);
	else
		lcd.print(h+" "+m+" "+s);
}
