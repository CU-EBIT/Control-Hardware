void setup() {
  Serial.begin(57600);
  Serial1.begin(57600);
}

void loop() {
  while (Serial.available()) {      // If anything comes in Serial (USB),
    Serial.read();   // flush Serial's read buffer, we don't care about it
  }

  if (Serial1.available()) {     // If anything comes in Serial1 (pins 0 & 1)
    Serial.write(Serial1.read());   // read it and send it out Serial (USB)
  }
}
