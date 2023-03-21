#define THERMISTOR_CACHE_SIZE 512 // This matches the value in thermistors.h
#define CAL_PIN A7                // This is the pin we are calibrating for

// If the device to calibrate does not use SerialUSB, replace below
// with the appropriate serial device.
#define SERIAL Serial

void setup() {
  SERIAL.begin(115200);      // Start usb output
  analogReadResolution(12);// Set ADC to 12 bit
  analogSetAttenuation(ADC_6db);
}

void loop() {
  int sum = 0;
  // Read THERMISTOR_CACHE_SIZE values and accumulate as a sum
  for(int i = 0; i<THERMISTOR_CACHE_SIZE; i++){
    sum += analogReadMilliVolts(CAL_PIN);
  }
  // Compute average value
  float var = ((float)sum) / THERMISTOR_CACHE_SIZE;
  // Print it to serial
  SERIAL.println(var);
}
