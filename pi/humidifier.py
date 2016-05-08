#!/usr/bin/python
import serial, io, time

sensors = None
humidifer = None

def connect_humidifier():
  global humidifer
  print "Connecting humidifer... ",
  try:
    humidifer = serial.Serial('/dev/rfcomm1', 9600)
    print "OK"
  except:
    time.sleep(10)
    connect_humidifier()

def connect_sensors():
  global sensors
  print "Connecting sensors... ",
#  try:
  sensors_raw = serial.Serial('/dev/rfcomm0', 9600)

  sensors = io.TextIOWrapper(io.BufferedRWPair(sensors_raw, sensors_raw, 1),  
                                   newline = '\n',
                                   line_buffering = True)
  print "OK"
#  except:
#    time.sleep(10)
#    connect_sensors()

connect_sensors()
connect_humidifier()

while True:
  print(sensors.readline()),
