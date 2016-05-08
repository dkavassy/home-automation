#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

const int humidifierPin = 2;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  pinMode(humidifierPin, OUTPUT); 
}

void loop() { // run over and over
 if (mySerial.available()) {
   char instruction = mySerial.read();
   
   if (instruction == '1' )  {
     digitalWrite(humidifierPin, HIGH);
   }
   else if (instruction == '0')  {
     digitalWrite(humidifierPin, LOW);
   }
   
   Serial.write(instruction);
  }
}
