// include the library code:
#include <LiquidCrystal.h>	//for LCD Display
#include "pitches.h"

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int pauseBetweenNotes, thisNote,day,month,year,dow,dated;
long int starth,startm,starts,rawtime,seconds,minutes,hours,homehours,homeminutes;
String h,hh,m,hm,s,d,mm,y;

int melody[] = {NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};
int noteDurations[] = {4, 8, 8, 4, 4, 4, 4, 4};
int dates[] = {31,28,31,30,31,30,31,31,30,31,30,31,29};

String days[] = {"Mon", "Die", "Mit", "Don", "Fre", "Sam", "Son"};
String months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec","Feb"};


void printDate();
void printTime();
void alarm();

void setup()
{
	//set initial date
	dow=1;
	day=5;
	month=3;
	year=2019;
	//set initial time
	starth=23;
	startm=59;
	starts=58;
	//don't play alarm on startup
	thisNote=8;
	// set up the LCD's number of columns and rows: 
	lcd.begin(16, 2);
	// Print a message to the LCD.
	lcd.setCursor(0,0);
	printDate();
	Serial.begin(9600);

}

void loop() 
{
	rawtime=millis()/1000+( (starth*60+startm)*60+starts);
	//discard the milliseconds
	seconds=rawtime%60;
	rawtime=rawtime/60;
	minutes=rawtime%60;
	homeminutes=(minutes+30);
	rawtime=rawtime/60;
	hours=rawtime%24;
	homehours=(hours+4);
	if(homeminutes>60)
	{
		homeminutes=homeminutes%60;
		homehours++;
	}
	if(!hours && !minutes && !seconds && !dated)
	{
		dow++;
		if(dow>6)
			dow=0;
		day++;
		if(day>dates[month-1])
		{
			day=1;
			month++;
			if(month>12)
			{
				month=1;
				year++;
			}
		}
		lcd.begin(16,2);	//resets the LCD every 24 hours
		lcd.setCursor(0,0);
		printDate();
		dated++;
	}
	lcd.setCursor(0,1);
	printTime();
	if(hours==5 && minutes==0 && seconds==0)
	{
		thisNote=0;
		dated=0;
	}
	if(thisNote<8)
	{
		alarm();
	}
	
	
}

void alarm()
{
	{
		int noteDuration = 1000 / noteDurations[thisNote];
		tone(8, melody[thisNote], noteDuration);
		pauseBetweenNotes = noteDuration * 1.30;
		delay(pauseBetweenNotes);
		noTone(8);
	}
	if(++thisNote==7 && hours == 5 && minutes <= 10)
	{
		thisNote=0;
	}
}

void printDate()
{
	if(day<10)
		d="0"+String(day);
	else
		d=String(day);
	y=String(year);
	lcd.print(days[dow]+","+d+"-"+months[month-1]+"-"+y);
}

void printTime()
{
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
	if(homeminutes<10)
	{
		hm="0"+String(homeminutes);
	}
	else
	{
		hm=String(homeminutes);
	}
	if(homehours<10)
	{
		hh="0"+String(homehours);
	}
	else
	{
		hh=String(homehours);
	}
	if(seconds%2)
		lcd.print(h+":"+m+":"+s+"   ");
	else
		lcd.print(h+" "+m+" "+s+"   ");
	lcd.print(hh+":"+hm);
}
