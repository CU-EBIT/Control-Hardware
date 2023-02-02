void setup() {
  Serial.begin(57600);
  Serial1.begin(57600);
}


void loop() {
  if (Serial.available()) {      // If anything comes in Serial (USB),
    Serial1.write(Serial.read());   // read it and send it out Serial1 (pins 0 & 1)
  }

  if (Serial1.available()) {     // If anything comes in Serial1 (pins 0 & 1)
    Serial1.read();   // flush Serial1's read buffer, we don't care about it
  }
}
