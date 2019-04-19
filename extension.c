// include the library code:
#include <LiquidCrystal.h>	//for LCD Display

//global variables
int a,dtemp,hours,minutes,seconds,nminute,nsecond,nhour,nday,nmonth,nyear,wday,dsecond,dminute,dhour;
long int rawtime,zerotime;
String h,m,s;

String days[] = {"Mon", "Die", "Mit", "Don", "Fre", "Sam", "Son"};
String months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void printTime();
void updateTime();
void updateWeather();
void printDate();

int isBig(int);
int isSmall(int);
int isFeb(int);

void setup()
{
	// set up the LCD's number of columns and rows: 
	lcd.begin(16, 2);
	// Print a message to the LCD.
	Serial.begin(9600);
}

void loop() 
{
	//check for incoming intent
	if (Serial.available() > 0)
	{
		a=Serial.read();
		switch (a)
		{
			case 1:
				updateTime();
				break;
				
			case 2:
				updateWeather();
				break;
			
		}
	}
	rawtime=(millis()-zerotime)/1000;
	seconds=rawtime%60;
	rawtime=rawtime/60;
	minutes=rawtime%60;
	rawtime=rawtime/60;
	hours=rawtime%24;
	dsecond=seconds+nsecond;
	if(dsecond>=60)
	{
		minutes++;
		dsecond=dsecond%60;
	}
	dminute=minutes+nminute;
	if(dminute>=60)
	{
		hours++;
		dminute%=60;
	}
	dhour=hours+nhour;
	if(dhour>=24)
	{
		(++wday)%7;nday++;
		if(isBig(nmonth))
			nday%=31;
		else if (isSmall(nmonth))
			nday%=30;
		else if (isFeb(nmonth))
			nday%=isFeb(nmonth);
		dhour%=24;
	}
	printDate();
	printTime();
	
}

int isBig(int m)
{
	if((m%2)==0)
		return 1;
	if(m==7)
		return 1;
	return 0;
}

int isSmall(int m)
{
	if(m==1)
		return 0;
	if(m%2)
		return 1;
	return 0;
}

int isFeb(int m)
{
	if(m!=1)
		return -1;
	if((nyear%4)==0)
	{
		if ((nyear%400)==0)
		{
			if(nyear%100)
				return 29;
		}
	}
	return 28;
}

void printDate()
{
	lcd.setCursor(0,0);
	lcd.print(days[wday]+','+String(nday)+'-'+months[nmonth]+'-'+String(nyear));
}

void updateWeather()
{
	int temp=0;
	delay(60);
	while(Serial.available()>0)
	{
		temp=temp*10;
		temp+=Serial.read()-48;
	}
	dtemp=temp;
}

void updateTime()
{
	long int time=0;
	zerotime=millis();
	delay(60);
	while(Serial.available()>0)
	{
		a=Serial.read();
		if(a==0)
		      break;
		time=time*10;
		time=time+a-48;
	}
	nsecond=time;
	time=0;
	while(Serial.available()>0)
	{
		a=Serial.read();
		if(a==0)
		      break;
		time=time*10;
		time=time+a-48;
	}
	nminute=time;
	time=0;
	while(Serial.available()>0)
	{
		a=Serial.read();
		if(a==0)
		      break;
		time=time*10;
		time=time+a-48;
	}
	nhour=time;
	time=0;
	while(Serial.available()>0)
	{
		a=Serial.read();
		if(a==0)
		      break;
		time=time*10;
		time=time+a-48;
	}
	nyear=time;
	time=0;
	while(Serial.available()>0)
	{
		a=Serial.read();
		if(a==0)
		      break;
		time=time*10;
		time=time+a-48;
	}
	nmonth=time-1;
	time=0;
	while(Serial.available()>0)
	{
		a=Serial.read();
		if(a==0)
		      break;
		time=time*10;
		time=time+a-48;
	}
	nday=time;
	time=0;
	while(Serial.available()>0)
	{
		a=Serial.read();
		if(a==0)
		      break;
		time=time*10;
		time=time+a-48;
	}
	wday=time;
}

void printTime()
{
	lcd.setCursor(0,1);
	//lcd.print(String(nhour+hours)+' '+String(nminute+minutes)+' '+String(nsecond+seconds));
	if(dsecond<10)
	{
		s="0"+String(dsecond);
	}
	else
	{
		s=String(dsecond);
	}
	if(dminute<10)
	{
		m="0"+String(dminute);
	}
	else
	{
		m=String(dminute);
	}
	if(dhour<10)
	{
		h="0"+String(dhour);
	}
	else
	{
		h=String(dhour);
	}
	if((nsecond+seconds)%2)
		lcd.print(h+":"+m+":"+s+"   ");
	else
		lcd.print(h+" "+m+" "+s+"   ");
	lcd.print(String(dtemp)+String(char(223))+"C");
}
