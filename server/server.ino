#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <Wire.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"

BLECharacteristic *ambient_temperature; //Ambient Temperature
#define AMBIENT_TEMPERATURE_CHARACTERISTIC_UUID "87fd32ec-9e46-439a-9c27-957b8acf8fcd"

BLECharacteristic *soil_temperature; //Ambient Temperature
#define SOIL_TEMPERATURE_CHARACTERISTIC_UUID "70e8cb3c-4a0d-445b-96ab-70850a150c76"

BLECharacteristic *humidity; //Ambient Humidity
#define HUMIDITY "668ede41-62e8-4a78-a901-53b8df6b1644"

BLECharacteristic *moisture; //Ambient moisture
#define MOISTURE "91a5addf-0c0e-4c1d-a6cb-e43d59e1f73e"

BLECharacteristic *light_spectrum;
#define LIGHT_SPECTRUM "774c3ae8-f76a-4cb6-aaba-dcf19ce383fc"

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  BLEDevice::init("Plant Sensor");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);

  ambient_temperature = pService->createCharacteristic(
    AMBIENT_TEMPERATURE_CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ |
    BLECharacteristic::PROPERTY_WRITE
  );

  pService->start();
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");
}

void loop() {
  // put your main code here, to run repeatedly:
  int random_value = rand() % (0xFF+1);
  byte values[] = {random_value};
  ambient_temperature->setValue(values, sizeof(values));
  delay(2000);
}