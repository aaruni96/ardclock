#!/usr/bin/python3

import serial
import time
import json
import requests

COMPORT="/dev/ttyACM0"				#Arduino goes here
AppID=""	#Openweather API
CityID=""	#Openweather City ID
ser = serial.Serial(COMPORT, timeout=2)

now = time.localtime(time.time())
oldhour=now.tm_hour

def sendTime() :
	global now
	now = time.localtime(time.time())
	ser.write(b'\x01')			#broadcast intent to send time
	ser.write(str(now.tm_sec).encode())
	ser.write(b'\x00')
	ser.write(str(now.tm_min).encode())
	ser.write(b'\x00')
	ser.write(str(now.tm_hour).encode())
	ser.write(b'\x00')
	ser.write(str(now.tm_year).encode())
	ser.write(b'\x00')
	ser.write(str(now.tm_mon).encode())
	ser.write(b'\x00')
	ser.write(str(now.tm_mday).encode())
	ser.write(b'\x00')
	ser.write(str(now.tm_wday).encode())

def sendWeather():
	data = json.loads(requests.get('http://api.openweathermap.org/data/2.5/weather?id='+CityID+'appid='+AppID).content.decode('utf-8'))
	ser.write(b'\x02')			#intent to send temp
	ser.write(str(int(data['main']['temp']-273.15)).encode())
	

while (1):
	#check for incoming request
	if(ser.read(12).decode() == "Give me time"):
		print("Request for time recieved at "+str(now.tm_year)+"-"+str(now.tm_mon)+"-"+str(now.tm_mday)+" "+str(now.tm_hour)+':'+str(now.tm_min)+':'+str(now.tm_sec))
		sendTime()
		print("Time update sent at "+str(now.tm_year)+"-"+str(now.tm_mon)+"-"+str(now.tm_mday)+" "+str(now.tm_hour)+':'+str(now.tm_min)+':'+str(now.tm_sec))
		sendWeather()
		print("Weather update sent at "+str(now.tm_year)+"-"+str(now.tm_mon)+"-"+str(now.tm_mday)+" "+str(now.tm_hour)+':'+str(now.tm_min)+':'+str(now.tm_sec))
		
	if(oldhour != now.tm_hour):		#updates time only on the hour, but doesn't sleep in the meantime because we have to listen
		sendTime()
		print("Time update sent at "+str(now.tm_year)+"-"+str(now.tm_mon)+"-"+str(now.tm_mday)+" "+str(now.tm_hour)+':'+str(now.tm_min)+':'+str(now.tm_sec))
		sendWeather()
		print("Weather update sent at "+str(now.tm_year)+"-"+str(now.tm_mon)+"-"+str(now.tm_mday)+" "+str(now.tm_hour)+':'+str(now.tm_min)+':'+str(now.tm_sec))
		oldhour = now.tm_hour
