# About

This is my own home automation project, at a very early stage. I'll keep updating it with schematics and code as I add features and document what has already been done.

It currently controls a humidifier to keep room humidity between the desired thresholds via Bluetooth. It also monitors dust particle levels and switches a warning LED when particle levels are too high.

There are three components
 * Sensor component - it includes a Sharp dust sensor and a humidity/temperature sensor (DHT22), sends reading via Bluetooth serial (HC06)
 * Humidifier controller - switches humidifier using signal received through Bluetooth serial (HC06)
 * Raspberry Pi - receives sensors readings and talks to the humidifer controller, the idea is that it will do much more e.g. make sensor data available on the web, record time series readings, visualise data etc

The circuits are in `pcb/` in [Fritzing](http://fritzing.org/) format

Arduino sketches are found in `arduino/`

Code for the Pi is found in `pi/` in Python