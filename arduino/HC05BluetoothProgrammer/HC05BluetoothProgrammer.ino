// Use both CR & NL as line endings in Serial
// Keep EN button pressed before powering up to enable AT mode
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  
  // set the data rate for the SoftwareSerial port
  mySerial.begin(38400);
}

void loop() { // run over and over
 if (mySerial.available()) {
   Serial.write(mySerial.read());
  }

 if (Serial.available()) {
   mySerial.write(Serial.read());
  }
}
