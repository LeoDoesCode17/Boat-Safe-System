#include <SPI.h>
#include <LoRa.h>

#define NSS 5
#define RST 14
#define DIO0 26
#define SCK 18
#define MOSI 23
#define MISO 19

void setup() {
  Serial.begin(115200);

  //wait until serial monitor is ready
  while (!Serial)
    ;

  Serial.println("LoRa Sender");

  // Initialize SPI with custom pins
  SPI.begin(SCK, MISO, MOSI, NSS);  // (SCK, MISO, MOSI, SS)

  // Define LoRa pins
  LoRa.setPins(NSS, RST, DIO0);

  // Initialize LoRa at 433 MHz (or 470E6 / 915E6 based on module)
  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa init failed. Check wiring.");
    while (true)
      ;
  }

  //inform that lora module init successful
  Serial.println("LoRa init successful.");
}

void loop() {
  //logic to receive packet

  //parse the packet
  int packetSize = LoRa.parsePacket();

  //if packet exists
  if (packetSize) {
    Serial.print("Received packet: ");

    while (LoRa.available()) {
      String incoming = LoRa.readString();
      Serial.print(incoming);
    }

    //RSSI represents signal strenght, the smaller the value (more negative), the weaker the signal
    Serial.print(" with RSSI ");  // Signal strength
    Serial.println(LoRa.packetRssi());
  }else{
    Serial.print("Packet not-detected")
  }
}
