// Experimenting with RF components
void setup() {
  pinMode(2, OUTPUT);
  Serial.begin(9600);
}

int on = false;

int total = 0.0;
int error = 0.0;
void loop() {
  if (on == true) {
    digitalWrite(2, HIGH);
    //Serial.print("HIGH: ");
  } else {
    digitalWrite(2, LOW);
    //Serial.print("LOW: ");
  }
  
  delay(1);
  total++;
  if ((analogRead(2)+analogRead(3))/2 > 850 && !on){
      error++;
      /*Serial.print(on ? 'L' : 'H'); 
      Serial.print(" error ");
      
      Serial.print(error);
      Serial.print(" out of  ");
      Serial.println(total);*/

  }
  if (total % 1000 == 0) {
    Serial.print("Status: ");
    Serial.print(error);
    Serial.print(" out of ");
    Serial.println(total);
  }

  on = on ? false : true;
  
  delay(1);
}
