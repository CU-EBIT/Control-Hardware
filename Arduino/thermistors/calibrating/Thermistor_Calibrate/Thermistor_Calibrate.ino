#define THERMISTOR_CACHE_SIZE 512 // This matches the value in thermistors.h
#define CAL_PIN A0                // This is the pin we are calibrating for

// If the device to calibrate does not use SerialUSB, replace below
// with the appropriate serial device.
#define SERIAL SerialUSB

void setup() {
  SERIAL.begin(1234);      // Start usb output
  while(!SERIAL);          // Wait for Serial to init
  analogReadResolution(12);// Set ADC to 12 bit
}

void loop() {
  int sum = 0;
  // Read THERMISTOR_CACHE_SIZE values and accumulate as a sum
  for(int i = 0; i<THERMISTOR_CACHE_SIZE; i++){
    sum += analogRead(CAL_PIN);
  }
  // Compute average value
  float var = ((float)sum) / THERMISTOR_CACHE_SIZE;
  // Print it to serial
  SERIAL.println(var);
}
