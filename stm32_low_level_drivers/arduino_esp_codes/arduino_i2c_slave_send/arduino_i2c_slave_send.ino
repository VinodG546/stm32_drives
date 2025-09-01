#include <Wire.h>

#define SLAVE_ADDR 0x68

const char msg[] = "HELLO";

void setup() {
  Serial.begin(9600);
  Wire.begin(SLAVE_ADDR);   // Arduino becomes I2C slave
  Wire.onRequest(requestEvent); // When master requests data
  Serial.println("I2C Slave ready...");
}

void loop() {
  delay(100);
}

void requestEvent() {
  // Always send "HELLO" when master reads
  Wire.write(msg, sizeof(msg) - 1);  // don’t send the null terminator
}
