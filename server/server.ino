#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <Adafruit_AS7341.h>
Adafruit_AS7341 as7341;

#include <Adafruit_AHTX0.h>
Adafruit_AHTX0 aht;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"

BLECharacteristic *ambient_temperature; //Ambient Temperature
#define AMBIENT_TEMPERATURE_CHARACTERISTIC_UUID "87fd32ec-9e46-439a-9c27-957b8acf8fcd"

// BLECharacteristic *soil_temperature; //Soil Temperature
// #define SOIL_TEMPERATURE_CHARACTERISTIC_UUID "70e8cb3c-4a0d-445b-96ab-70850a150c76"

BLECharacteristic *humidity; //Ambient Humidity
#define HUMIDITY "668ede41-62e8-4a78-a901-53b8df6b1644"

BLECharacteristic *moisture; //Ambient moisture
#define MOISTURE "91a5addf-0c0e-4c1d-a6cb-e43d59e1f73e"

BLECharacteristic *light_spectrum;
#define LIGHT_SPECTRUM "774c3ae8-f76a-4cb6-aaba-dcf19ce383fc"

void setup() {

  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  if (!as7341.begin()){
  Serial.println("Could not find AS7341");
  while (1) {
    Serial.println("Stuck in this godamn fucking loop");
    delay(10); 
  }
  }
  as7341.setATIME(100);
  as7341.setASTEP(999);
  as7341.setGain(AS7341_GAIN_256X);

  if (! aht.begin()) {
  while (1){
    Serial.println("Could not find AHT? Check wiring");
    delay(10);
  }
  }
  
  BLEDevice::init("Plant Sensor");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);

  light_spectrum = pService->createCharacteristic(
  LIGHT_SPECTRUM,
  BLECharacteristic::PROPERTY_READ);

  ambient_temperature = pService->createCharacteristic(
  AMBIENT_TEMPERATURE_CHARACTERISTIC_UUID,
  BLECharacteristic::PROPERTY_READ);
  
  humidity = pService->createCharacteristic(
  HUMIDITY,
  BLECharacteristic::PROPERTY_READ);


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
  if (!as7341.readAllChannels()){
    Serial.println("Error reading all channels!");
    return; //crashes the program
  }

  // int random_value = rand() % (0xFF+1);
  // byte values[] = {random_value};

  std::string spectrum;

  uint16_t spec_415 = as7341.getChannel(AS7341_CHANNEL_415nm_F1);
  spectrum += std::to_string(spec_415);
  spectrum += ",";

  uint16_t spec_445 = as7341.getChannel(AS7341_CHANNEL_445nm_F2);
  spectrum += std::to_string(spec_445);
  spectrum += ",";

  uint16_t spec_480 = as7341.getChannel(AS7341_CHANNEL_480nm_F3);
  spectrum += std::to_string(spec_480);
  spectrum += ",";

  uint16_t spec_515 = as7341.getChannel(AS7341_CHANNEL_515nm_F4);
  spectrum += std::to_string(spec_515);
  spectrum += ",";

  uint16_t spec_555 = as7341.getChannel(AS7341_CHANNEL_555nm_F5);
  spectrum += std::to_string(spec_555);
  spectrum += ",";

  uint16_t spec_590 = as7341.getChannel(AS7341_CHANNEL_590nm_F6);
  spectrum += std::to_string(spec_555);
  spectrum += ",";

  uint16_t spec_630 = as7341.getChannel(AS7341_CHANNEL_630nm_F7);
  spectrum += std::to_string(spec_630);
  spectrum += ",";

  uint16_t spec_680 = as7341.getChannel(AS7341_CHANNEL_680nm_F8);
  spectrum += std::to_string(spec_680);

  sensors_event_t hum, tem; 
  aht.getEvent(&hum, &tem);
  float temp = tem.temperature;
  float humi = hum.relative_humidity;
  // uint8_t result[2] = {static_cast<uint8_t>(clear_value), static_cast<uint8_t>(clear_value >> 8)};
  // std::string myStringForUnit16((char*)&clear_value, 2);
  std::string temperature_str = std::to_string(temp);
  std::string humididty_str = std::to_string(humi);

  // String clear = String(clear_value);
  ambient_temperature->setValue(temperature_str);
  humidity->setValue(humididty_str);
  light_spectrum->setValue(spectrum);
  delay(200);
}