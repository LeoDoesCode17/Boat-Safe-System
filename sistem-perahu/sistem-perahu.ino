#include <SPI.h>
#include <LoRa.h>
#include <MPU9250_asukiaaa.h>
#include <Wire.h>


/*
In this system, send coordinate data every 3 secs
If degree >= min then send the data degreee
*/

// LoRa Pin Configuration
const int LORA_NSS = 5;
const int LORA_RST = 14;
const int LORA_DIO0 = 26;
const int LORA_SCK = 18;
const int LORA_MOSI = 23;
const int LORA_MISO = 19;

// MPU9250 Pin Configuration
const int MPU_SDA = 21;
const int MPU_SCL = 22;

// MPU9250 Object
MPU9250_asukiaaa mySensor;

// Tilt Threshold (degrees)
const float TILT_THRESHOLD = 30.0;


bool setupLoRa() {
  Serial.println("Initializing LoRa...");

  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_NSS);
  LoRa.setPins(LORA_NSS, LORA_RST, LORA_DIO0);

  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa initialization failed. Check wiring.");
    return false;
  }

  Serial.println("LoRa initialized successfully.");
  return true;
}

bool setupMPU() {
  Wire.begin(MPU_SDA, MPU_SCL);
  mySensor.setWire(&Wire);

  // Initialize all sensors
  mySensor.beginAccel();
  mySensor.beginGyro();
  mySensor.beginMag();

  // Check connection by trying to read Accel data
  if (mySensor.accelUpdate() != 0) {
    Serial.println("MPU9250 initialization failed. Check wiring.");
    return false;
  }

  Serial.println("MPU9250 initialized successfully.");
  return true;
}

void sendMessage(String message) {
  LoRa.beginPacket();
  LoRa.print(message);
  LoRa.endPacket();

  Serial.println("LoRa Sent: " + message);
}

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;  // Wait for Serial Monitor

  bool loraReady = setupLoRa();
  bool mpuReady = setupMPU();

  if (loraReady && mpuReady) {
    Serial.println("All systems initialized successfully!");
  } else {
    Serial.println("System initialization failed.");
  }
}

void loop() {
  mySensor.accelUpdate();

  float ax = mySensor.accelX();
  float ay = mySensor.accelY();
  float az = mySensor.accelZ();

  // Calculate Roll & Pitch
  float roll = atan2(ay, az) * 180 / PI;
  float pitch = atan2(-ax, sqrt(ay * ay + az * az)) * 180 / PI;

  Serial.print("Roll: ");
  Serial.print(roll);
  Serial.print(" Pitch: ");
  Serial.println(pitch);

  // if (abs(pitch) > TILT_THRESHOLD || abs(roll) - 179 > TILT_THRESHOLD) {
  //   String msg = "WARNING! Tilt Detected. Roll: " + String(roll, 2) + " Pitch: " + String(pitch, 2);
  //   LoRa.beginPacket();
  //   LoRa.print(msg);
  //   LoRa.endPacket();

  //   Serial.println("LoRa Sent: " + msg);
  // }

  //send data
  String msg = " Roll: " + String(roll, 2) + " Pitch: " + String(pitch, 2);
  sendMessage(msg);
}
