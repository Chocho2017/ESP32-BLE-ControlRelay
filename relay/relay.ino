#include <PlainProtocol.h>
#include <Adafruit_NeoPixel.h>

#define RELAY_PIN 3
#define RGB_PIN 4

enum{
  STOP,
  RUN,
  DEFAULTSTOP
};

enum{
  DEFAULTCOLOR,
  RED,
  GREEN,
  BLUE
};

PlainProtocol mySerial(Serial,115200);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, RGB_PIN, NEO_GRB + NEO_KHZ800);

boolean rgbBlink = false;
char currentState = DEFAULTSTOP;
unsigned long current_millis = 0;

/*
 * 1->Red 2->Green 3->Blue default->close
 */
void setRGBColor(int value){
  switch(value){
    case 1:
       strip.setPixelColor(0, strip.Color(255, 0, 0));
       break;
    case 2:
       strip.setPixelColor(0, strip.Color(0, 255, 0));
       break;
    case 3:
       strip.setPixelColor(0, strip.Color(0, 0, 255));
       break;
    default:
       strip.setPixelColor(0, strip.Color(0, 0, 0));
       break; 
  }
  strip.show();
}
void setRelayState(char value){
  if(value == RUN){
    currentState = RUN;
    digitalWrite(RELAY_PIN,HIGH);
    setRGBColor(GREEN);
  }else{
    currentState = STOP;
    digitalWrite(RELAY_PIN,LOW);
    setRGBColor(RED);
  }
}

void updateState(void){
  mySerial.write("STATE",currentState);
}

void blinkLoop(void){
  static int iRGB = 0;
  static int times = 0;
  if(rgbBlink){
    if(iRGB){
     setRGBColor(DEFAULTCOLOR);
    }else{
      if(currentState==DEFAULTSTOP){
        setRGBColor(BLUE);
      }else if(currentState==RUN){
        setRGBColor(GREEN);
      }else if(currentState==STOP){
        setRGBColor(RED);
      }
      if(times > 50){
        rgbBlink = false;
      }
    }
    times++;
  }else{
    setRelayState(currentState);
    times = 0;
  }
  iRGB=~iRGB;
}

void timerRun(void){
  if((millis()- current_millis)>200){
    current_millis = millis();
    blinkLoop();
    updateState();
  }
}

void setup() {
  Serial.begin(115200);
  current_millis = millis();
  strip.begin();
  strip.show(); 
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN,LOW);
  setRGBColor(BLUE);
}

void loop() {
  if(mySerial.available()){
    if(mySerial.equals("RELAYSTATE")){
      int value = mySerial.read();
      setRelayState(value);
    }else if(mySerial.equals("SCAN")){
      int value = mySerial.read();
      if(value==1)
       rgbBlink = true;
      else
       rgbBlink = false;
    }
  }
  timerRun();
}
