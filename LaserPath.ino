#include <FastLED.h>
#include "pitches.h"

#define PIN_SPEAKER 11
#define PIN_LED 13
#define PIN_LDR_LASER A0

#define DATA_PIN 9 //NeoPixel data pin
#define NUM_LEDS 3
CRGB leds[NUM_LEDS];

#define THRESHOLD 15
#define DISABLED_TIME 2000

enum { MODE_ENABLED, MODE_DISABLED };
uint8_t mode = MODE_ENABLED;
uint8_t oldBeepStatus = false;
int oldValue=0;
unsigned long t=0;

void setup() {
  pinMode(PIN_SPEAKER,OUTPUT);
  pinMode(PIN_LED,OUTPUT);
  pinMode(PIN_LDR_LASER,INPUT);

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  
  Serial.begin(115200);
}


void loop() {
  if(mode == MODE_ENABLED && shouldBeep()){    
    beepAndFlash();    
    //prevent continuous beep
  //  mode = MODE_DISABLED;
    oldBeepStatus = true;
    t = millis();
  }
  //re-enable beep automatically after DISABLED_TIME ms
  if(oldBeepStatus && (millis()-t > DISABLED_TIME)){
    oldBeepStatus = false;
    mode = MODE_ENABLED;
  }
}

uint8_t shouldBeep(){
  uint8_t res = false;
  int a0=analogRead(PIN_LDR_LASER);
  int deriv = a0-oldValue;
  oldValue = a0;
  if(deriv > THRESHOLD){
    res = true;
  }
  Serial.print(a0);
  Serial.print(" ");
  Serial.println(deriv);
  return res;
}

void light(bool on){
    CRGB color = CRGB::Black;
    if(on){
      color = CRGB::Red;
    }
  
    leds[0] = color;
    leds[1] = color;
    leds[2] = color;    

    FastLED.show();
}


void beepAndFlash(){
  for(int i=0;i<4;i++){
    digitalWrite(PIN_LED,HIGH);
    light(true);
    tone(PIN_SPEAKER,NOTE_A3,100);
    delay(100);
    digitalWrite(PIN_LED,LOW);
    light(false);
    tone(PIN_SPEAKER,NOTE_G3,100);
    delay(100);
  }
}

