/*RFID reader pins: it works on 5v VCC
   RXD = TX1
   TXD = RX1
  GPS module pins:
   RX = PIN 3
   TX = PIN 2
  DS3231 module pins:
   SDA = SDA 20
   SCL = SCL 21
  SIM800L module pins:
   RXD = TX2
   TXD = RX2
  GLCD module pins:
   VO = to the potentiometer
   IRS = PIN 12
   R/W = PIN 11
   E = PIN 13
  NEO PIXELS pins:
   DIN = PIN 10

   0782982710
*/
/////////////////////////////////////NFC reader library///////////////////////////////////////////
#include <PN532_HSU.h>
#include <PN532.h>
#include <NfcAdapter.h>
PN532_HSU pn532hsu(Serial1);
NfcAdapter nfc = NfcAdapter(pn532hsu);
///////////////////////////////////////GPS library////////////////////////////////////////////////
#include <TinyGPS++.h>
//#include<SoftwareSerial.h>
TinyGPSPlus gps;
//SoftwareSerial GPS(2, 3);
//////////////////////////////////////DS3231 library//////////////////////////////////////////////
#include <DS3232RTC.h>  // a basic DS1307 library that returns time as a time_t
#include <TimeLib.h>
#include <Wire.h>
///////////////////////////////////////LCD library///////////////////////////////////////////////
#include "U8glib.h"
U8GLIB_ST7920_128X64 u8g(13, 11, 12, U8G_PIN_NONE);
//////////////////////////////////////NEO PIXEL//////////////////////////////////////////////////
#include <Adafruit_NeoPixel.h>
#define PIN         10
#define NUMPIXELS   8
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
//////////////////////////////////////GSM module////////////////////////////////////////////////
#define TINY_GSM_MODEM_SIM800
#include <TinyGsmClient.h>
#include <PubSubClient.h>
#define SerialAT Serial3
TinyGsm modem(SerialAT);
TinyGsmClient client(modem);
PubSubClient mqtt(client);
///////////////////////////////////////////JSON library//////////////////////////////////////////
#define ARDUINOJSON_USE_DOUBLE 1
#include <ArduinoJson.h>

const char* broker        = "test.mosquitto.org";
const char apn[]          = "internet.mtn";
const char user[]         = "";
const char pass[]         = "";
const char* fabtopic      = "fablab/kigali";        //DEBUG topic
const char* topicjourney  = "topic/journey";        //topic for setting the journey
const char* topicstdntNum = "topic/student/number"; //topic for indicating the # of student in the bus
const char* topicgps      = "topic/gps";            //topic for sending the gps coordinates each 5 sec
const char* topiccards    = "topic/cards";          //this topic sends stdnt cards when tapping
const char* topicsettings = "topic/settings";       //topic for configuring the the device
const char* deviceID      = "DV2560A0001";          //Device name/ID (DV,processor, type, number)

String Uid;
uint8_t stdntNo; //number of student in the bus
//for DEBUG
int count{0};
int bars{0};

void setup(void) {
  // flip screen, if required
  //   u8g.setRot180();

  Serial.begin(115200);   //Serial port
  SerialAT.begin(115200); //GSM module
  Serial2.begin(9600);    //GPS module
  pixels.begin(); // This initializes the NeoPixel library.
  neoColor('b');

  /*Serial.println("Initializing modem...");
  modem.init();
  bars = modem.getSignalQuality();
  Serial.print("Signal quality: ");
  Serial.println(bars);
  Serial.print("Waiting for network...");
  if (!modem.waitForNetwork()) {
    Serial.println(" fail");
    while (true);
  }
  Serial.println(" OK");
  bars = modem.getSignalQuality();
  Serial.print("Signal quality: ");
  Serial.println(bars);
  Serial.print("Configuring APN...");
  if (!modem.gprsConnect(apn, user, pass)) {
    Serial.println(" fail");
    while (true);
  }
  Serial.println(" OK");*/

  setSyncProvider(RTC.get);   // the function to get the time from the RTC
  if (timeStatus() != timeSet) {
    //    Serial.println("Unable to sync with the RTC");
  }
  nfc.begin();
  Serial.println("Scan a card to read it!");

  //mqtt.setServer(broker, 1883);
  //mqtt.setCallback(mqttCallback);
//  /*mqttConnect();*/
}

void loop() {
  StaticJsonBuffer<300> jsonBuffer;
  char JsonChar[300];
  if (mqtt.connected()) {
    mqtt.loop();
    processDisplay();
    processNFC();
    JsonObject& Json = createJson(jsonBuffer);
    Json.printTo(JsonChar, sizeof(JsonChar));
    mqtt.publish(fabtopic, JsonChar);

    //for DEBUG
    Json.printTo(Serial);
    Serial.println("");
    Serial.println(++count);
    Serial.print("Signal quality: ");
    bars = modem.getSignalQuality();
    Serial.println(bars);
  }
  processDisplay();
  processNFC();
  JsonObject& Json = createJson(jsonBuffer);
  Json.printTo(JsonChar, sizeof(JsonChar));
  Json.prettyPrintTo(Serial);
  Serial.println("");
}

void neoColor(char color) {
  if (color == 'G') {         //Green color
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 238, 0));
      pixels.show();
    }
    smartDelay(350);
  }
  else if (color == 'R') {    //Red color
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(238, 0, 0));
      pixels.show();
    }
    smartDelay(500);
  }
  else if (color == 'B') {    //Blue color
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 238));
      pixels.show();
    }
    smartDelay(350);
  }
  else if (color == 'Y') {    //Yellow color
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(255, 255, 0));
      pixels.show();
    }
    smartDelay(350);
  }
  for (int i = 0; i < NUMPIXELS; i++) {   //Black color
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    pixels.show();
  }
}

