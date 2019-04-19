#!/usr/bin/python3

import serial
import time
import json
import requests

COMPORT="/dev/ttyACM0"				#Arduino goes here
AppID=""	#Openweather API
CityID=""	#Openweather City ID
ser = serial.Serial(COMPORT)

def sendTime() :
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
	sendTime()
	sendWeather()
	time.sleep(3600)
