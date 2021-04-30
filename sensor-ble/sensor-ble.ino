//Development
const bool develmode = true;



//Identification
#include <ArduinoUniqueID.h>
String id = "";

//BLE
#include <ArduinoBLE.h>
BLEService senseService("86ae654b-8ff4-483d-a386-8a698359b9c2");
BLECharacteristic moistureValueChar("19B10000-E8F2-537E-4F6C-D104768A1215", BLERead, "moisture");
BLECharacteristic temperatureValueChar("19B10000-E8F2-537E-4F6C-D104768A1216", BLERead, "temperature");
BLECharacteristic lightValueChar("19B10000-E8F2-537E-4F6C-D104768A1217", BLERead, "light");

//Sensing
  //Moisture
const int sensorMoisturePin = A3;
const int dryVal = 880;
const int dryValNano = 700;
const int dryValNano10M = 520;
const int wetVal = 674;
const int wetValNano = 230;
const int wetValNano10M = 180;
  //Temperature
const int sensorTemperaturePin = A6;
  //Light
const int sensorLightPin = A0;
const int sensorLightLow = 0;
const int sensorLightHigh = 1023;

void setup() {
  //Idenitifcation
  id = readID();

  //BLE
  Serial.begin(9600);
  //while (!Serial);
  
  pinMode(LED_BUILTIN, OUTPUT);
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1);
  }
  BLE.setLocalName("BLE Sense");
  BLE.setDeviceName("BLE Sense");
  BLE.setAdvertisedService(senseService);
  senseService.addCharacteristic(moistureValueChar);
  senseService.addCharacteristic(temperatureValueChar);
  senseService.addCharacteristic(lightValueChar);
  BLE.addService(senseService);
  BLE.advertise();
  Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() {
  //BLE
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());
    digitalWrite(LED_BUILTIN, HIGH);

    while (central.connected()) {
      //Sensing
    
        //Moisture
      int sensorMoistureVal = analogRead(sensorMoisturePin);
      if (develmode) {
        Serial.print("Sensor Value Moisture: ");
        Serial.println(sensorMoistureVal);
      }
      int moisture = map(sensorMoistureVal, dryValNano10M, wetValNano10M, 0, 100); 
      if (moisture < 0) {moisture = 0;}
      if (moisture > 100) {moisture = 100;}
    
        //Temperature
      int sensorTemperatureVal = analogRead(sensorTemperaturePin);
      float voltageTemp = (sensorTemperatureVal/1024.0) * 3.3;
      float temperature = (voltageTemp - .5) * 100;
      if (develmode) {
        Serial.print("Sensor Value Temperature: ");
        Serial.println(sensorTemperatureVal);
      }
      
        //Light
      int sensorLightVal = analogRead(sensorLightPin);
      int light = map(sensorLightVal, sensorLightLow, sensorLightHigh, 0, 100); 
      if (develmode) {
        Serial.print("Sensor Value Light: ");
        Serial.println(sensorLightVal);
      }

      printID();

      //Moisture
      Serial.print("Moisture: ");
      Serial.println(moisture);
      moisture = moisture, HEX;
      moistureValueChar.writeValue((byte)moisture);

      //Temperature
      Serial.print("Temperature: ");
      Serial.println(temperature);
      temperature = temperature, HEX;
      temperatureValueChar.writeValue((byte)temperature);

      //Light
      Serial.print("Light: ");
      Serial.println(light);
      light = light, HEX;
      lightValueChar.writeValue((byte)light);

      Serial.println();
      delay(1000);
    }
  }
  digitalWrite(LED_BUILTIN, LOW);
  Serial.print("Disconnected from central: ");
  Serial.println(central.address());

  //Serial.println();
  delay(5000);
  
}


//Identification
String readID() {
  String id = "";
  for(size_t i = 0; i < UniqueIDsize; i++)
    id = id + UniqueID[i];
  return id;
}
void printID() {
  Serial.print("ID: ");
  Serial.println(id);
}
