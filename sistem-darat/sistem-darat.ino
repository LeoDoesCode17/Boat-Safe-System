#include <SPI.h>
#include <LoRa.h>

/*
In this system, will be determine if the degree received is more than 5s then on the buzzer else give early warning
*/

#define NSS 5
#define RST 14
#define DIO0 26
#define SCK 18
#define MOSI 23
#define MISO 19

void setup() {
  Serial.begin(115200);

  while (!Serial);  // Wait for Serial Monitor

  Serial.println("LoRa Receiver");

  // Initialize SPI with custom pins
  SPI.begin(SCK, MISO, MOSI, NSS);  // (SCK, MISO, MOSI, SS)

  // Define LoRa pins
  LoRa.setPins(NSS, RST, DIO0);

  // Initialize LoRa at 433 MHz (or change to 470E6, 915E6 based on your module)
  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa init failed. Check wiring.");
    while (true);
  }

  Serial.println("LoRa init successful.");
}

void loop() {
  int packetSize = LoRa.parsePacket();

  if (packetSize) {
    Serial.print("Received packet: ");

    while (LoRa.available()) {
      String incoming = LoRa.readString();
      Serial.print(incoming);
    }

    Serial.print(" with RSSI ");
    Serial.println(LoRa.packetRssi());
  } else {
    Serial.println("Packet not detected");
  }

  delay(300);
}
