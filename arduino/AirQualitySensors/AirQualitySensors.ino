#include <SoftwareSerial.h>
#include "DHT.h"

#define DHT_PIN 2

DHT dht22(DHT_PIN, DHT22);

SoftwareSerial softSerial(10, 11); // RX, TX

struct options {
  const int dustMeasurePin;
  const int dustLedPowerPin;
  const int samplingTime;
  const int deltaTime;
  const int sleepTime;
  const float voltageOffset;
  const float warningThreshold;
  const int warningLedPin;
  const int resetAverageCycles;
};

const struct options options = {
  .dustMeasurePin = 5,
  .dustLedPowerPin = 4,
  .samplingTime = 280,
  .deltaTime = 40,
  .sleepTime = 9680,
  .voltageOffset = 0.77,
  .warningThreshold = 25.0,
  .warningLedPin = 3,
  .resetAverageCycles = 100
};

typedef struct DustMeasurements {
   float density;
   float voltage;
} Dust;

float averageDustDensity = 0.0;
int numDustMeasurements = 0;

void setup(){
  softSerial.begin(9600);
  dht22.begin();
  pinMode(options.dustLedPowerPin, OUTPUT);
  pinMode(options.warningLedPin, OUTPUT);
}

float measureDustV0() {
  float voMeasured = 0;
  
  digitalWrite(options.dustLedPowerPin,LOW); // power on the LED
  delayMicroseconds(options.samplingTime);

  voMeasured = analogRead(options.dustMeasurePin); // read the dust value
  
  delayMicroseconds(options.deltaTime);
  digitalWrite(options.dustLedPowerPin,HIGH); // turn the LED off
  delayMicroseconds(options.sleepTime);

  return voMeasured;
}

Dust getDustMeasurement(int times) {
  float calcVoltage = 0;
  float dustDensity = 0;
  float voMeasured = 0;
  const float chracteristicFactor = 0.172; // == 0.29V / 0.1mg/m3 - from http://www.howmuchsnow.com/arduino/airquality/
  Dust measurement;
  
  for (int i = 0; i < times; i++) {
    voMeasured = (voMeasured * (float)i + measureDustV0()) / ((float)i + 1.0);
    delay(100);
  }
   
  // 0 - 5.0V mapped to 0 - 1023 integer values 
  calcVoltage = voMeasured * (5.0 / 1024);

  measurement.voltage = calcVoltage;
  
  dustDensity = (chracteristicFactor * (calcVoltage - options.voltageOffset))*1000;
  if (dustDensity < 0.001) {
    dustDensity = 0.0;
  }
  
  measurement.density = dustDensity;

  averageDustDensity  = (averageDustDensity*(float)numDustMeasurements + dustDensity) / ((float)numDustMeasurements + 1.0);
  numDustMeasurements += 1;

  if (numDustMeasurements >= options.resetAverageCycles) {
    numDustMeasurements = 0;
  }
  
  return measurement;
}


void dust() {
  Dust dust = getDustMeasurement(10);

  softSerial.print("Dust Density [ug/m3]: ");
  softSerial.print(dust.density);
  softSerial.print(" (200s avg: ");
  softSerial.print(averageDustDensity);
  softSerial.print(")");
  
  softSerial.print(" - Sensor Voltage: ");
  softSerial.println(dust.voltage);

  if (averageDustDensity > options.warningThreshold) {
    digitalWrite(options.warningLedPin, HIGH);
  } else {
    digitalWrite(options.warningLedPin, LOW);
  }
}

void humidity(void)
{
  float humidity = dht22.readHumidity();
  float temperature = dht22.readTemperature();
  float heat_index = dht22.computeHeatIndex(temperature, humidity, false);

  if (isnan(humidity) || isnan(temperature)) {
    softSerial.println("Failed to read from sensor!");
    return;
  }
  
  softSerial.print("DHT22 Humidity: ");
  softSerial.print(humidity);
  softSerial.print(" %\t");
  softSerial.print("Temperature: ");
  softSerial.print(temperature);
  softSerial.print(" *C ");
  softSerial.print("\t");
  softSerial.print("Heat index: ");
  softSerial.print(heat_index);
  softSerial.println(" *C");
}

int loopCounter = 0;

void loop() {

  if (loopCounter < 5) {
    dust();  // takes 1.01sec
  } else if (loopCounter == 5) {
    humidity(); // takes little, needs min. 2s in between
  }
  
  if (loopCounter >= 5) {
    loopCounter = -1;
  }

  loopCounter += 1;
  
  delay(1000);
}

