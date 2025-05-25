#include <Arduino.h>
#include <ModbusMaster.h>

#define MAX485_DE_RE 4

ModbusMaster node;

void preTransmission() {
  digitalWrite(MAX485_DE_RE, HIGH);
}

void postTransmission() {
  digitalWrite(MAX485_DE_RE, LOW);
}

void setup() {
  Serial2.begin(9600, SERIAL_8N1, 16, 17);

  pinMode(MAX485_DE_RE, OUTPUT);
  digitalWrite(MAX485_DE_RE, LOW);

  node.begin(1, Serial2);

  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);

  Serial.begin(115200);
  Serial.println("Modbus Master started");
}

void loop() {
  uint8_t result;
  uint16_t data[6];

  result = node.readHoldingRegisters(170, 8);

  if (result == node.ku8MBSuccess) {
    Serial.println("Read success:");
    for (int i = 0; i < 6; i++) {
      data[i] = node.getResponseBuffer(i);
      Serial.printf("Reg %d: %u\n", i, data[i]);
    }
  } else {
    Serial.printf("Modbus read error: 0x%02X\n", result);
  }

  delay(500);
}