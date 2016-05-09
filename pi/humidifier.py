#!/usr/bin/python
import serial, io, time, sys

LOW = float(sys.argv[1])
HIGH = float(sys.argv[2])

sensors = None
humidifier = None

def connect_humidifier():
  global humidifier
  print 'Connecting humidifier... ',
  try:
    humidifier = serial.Serial('/dev/rfcomm1', 9600)
    print 'OK'
  except serial.SerialException:
    time.sleep(10)
    connect_humidifier()

def connect_sensors():
  global sensors
  print 'Connecting sensors... ',
  try:
    sensors_raw = serial.Serial('/dev/rfcomm0', 9600)

    sensors = io.TextIOWrapper(io.BufferedRWPair(sensors_raw, sensors_raw, 1),  
                                   newline = '\n',
                                   line_buffering = True)
    print 'OK'
  except serial.SerialException:
    time.sleep(10)
    connect_sensors()

connect_sensors()
connect_humidifier()

while True:
  try:
    reading = sensors.readline().split()
  except serial.SerialException:
    connect_sensors()
    continue
  if reading[0] == 'DHT22':
    try:
      humidity = float(reading[2])
      temperature = float(reading[5])
    except ValueError:
      print 'Bad humidity value'
      continue

    print "Humidity: %.1f%% - Temperature %.1fC - [%.1f%%, %.1f%%]" % (humidity, temperature, LOW, HIGH)

    try:
      if humidity < LOW:
        humidifier.write('1')
      elif  humidity > HIGH:
        humidifier.write('0')
    except serial.SerialException:
      connect_humidifier()
      continue

  elif reading[0] == 'Dust':
    print ' '.join(reading)
