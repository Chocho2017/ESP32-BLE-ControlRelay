#include "DFRobot_ESP32_BLE.h"
DFRobot_ESP32_BLE ble;
char dataName[15] = {'\0'};

boolean relay1 = false;
boolean relay2 = false;
boolean scan = false;

void scanKeys(void){
  if(digitalRead(D2) == LOW){
    delay(5);
    while(digitalRead(D2) == LOW);
    String sendStr = "<RELAYSTATE>";
    if(relay1 == false){
      sendStr = sendStr+"1;";
      relay1 = true;
    }else{
      sendStr = sendStr+"0;";
      relay1 = false;
    }
    ble.writedata("Chocho1", sendStr);
  }
  
  if(digitalRead(D5) == LOW){
    delay(5);
    while(digitalRead(D5) == LOW);
    String sendStr = "<RELAYSTATE>";
    if(relay2 == false){
      sendStr = sendStr+"1;";
      relay2 = true;
    }else{
      sendStr = sendStr+"0;";
      relay2 = false;
    }
    ble.writedata("Chocho2", sendStr);
  }

  if(digitalRead(D9) == LOW){
    delay(5);
    while(digitalRead(D9) == LOW);
    String sendStr = "<SCAN>";
    if(scan == false){
      sendStr = sendStr+"1;";
      scan = true;
    }else{
      sendStr = sendStr+"0;";
      scan = false;
    }
    ble.writedata("Chocho1", sendStr);
    delay(10);
    ble.writedata("Chocho2", sendStr);
  }
  delay(20);
}

void setup() {
  //Serial.begin(115200);
  pinMode(D2,INPUT);
  pinMode(D5,INPUT);
  pinMode(D9,INPUT);
  ble.setService(0xdfb0);
  ble.setCharacteristic(0xdfb1);
  ble.setconnummax(2);
  ble.setconnectname0("Chocho1");
  ble.setconnectname1("Chocho2");
  ble.init();
  delay(100);
  ble.begin();
}

void loop() {
  String bledata = ble.readdata(dataName);
  if (bledata.length() > 0) {
    if (strncmp(dataName, "Chocho1", strlen("Chocho1")) == 0) {
      //Serial.print("Chocho1:");
      //Serial.println(bledata);
    } else if (strncmp(dataName, "Chocho2", strlen("Chocho2")) == 0) {
      //Serial.print("Chocho2:");
      //Serial.println(bledata);
    }
  }
  scanKeys();
}
