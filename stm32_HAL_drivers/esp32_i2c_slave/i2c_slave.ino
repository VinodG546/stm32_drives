#include <Wire.h>

#define I2C_SLAVE_ADDR 0x55

void onReceive(int len) {
  Serial.printf("Received %d bytes: ", len);
  while (Wire.available()) {
    Serial.print(Wire.read());
    Serial.print(" ");
  }
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("ESP32 ready as I2C slave, waiting for data...");

  // SDA=21, SCL=22
  Wire.begin((uint8_t)I2C_SLAVE_ADDR, 21, 22, 100000);  
  Wire.onReceive(onReceive);
}

void loop() {
  delay(100);
}