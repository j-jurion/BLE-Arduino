//Development
const bool develmode = true;


//Identification
#include <ArduinoUniqueID.h>
String id = "";

//BLE
#include <ArduinoBLE.h>
BLEService senseService("86ae654b-8ff4-483d-a386-8a698359b9c2");
BLECharacteristic moistureValueChar("19B10000-E8F2-537E-4F6C-D104768A1215", BLERead, "moisture");

//Sensing
  //Moisture
const int sensorMoisturePin = A0;
const int dryVal = 880;
const int dryValNano = 700;
const int dryValNano10M = 485;
const int wetVal = 674;
const int wetValNano = 230;
const int wetValNano10M = 110;
  //Temperature
const int sensorTemperaturePin = A1;


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
    //int sensorTemperatureVal = analogRead(sensorTemperaturePin);
    //float voltageTemp = (sensorTemperatureVal/1024.0) * 5.0;
    //float temperature = (voltageTemp - .5) * 100;
    //if (develmode) {
    //  Serial.print("Sensor Value Temperature: ");
    //  Serial.println(sensorTemperatureVal);
    //  Serial.print(" Volts: ");
    //  Serial.println(voltageTemp);
    //  Serial.print("Temperature: ");
    //  Serial.print(temperature);
    //}
    

    Serial.println();
    printID();
    Serial.print("Moisture: ");
    Serial.println(moisture);
    //Serial.println(moisture, HEX);
    moisture = moisture, HEX;
    moistureValueChar.writeValue((byte)moisture);
    Serial.println();
    delay(1000);

    
  }
  }
  digitalWrite(LED_BUILTIN, LOW);
  Serial.print("Disconnected from central: ");
  Serial.println(central.address());

  //Serial.println();
  delay(1000);
  
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
