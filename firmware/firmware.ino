#include <Wire.h>
#include <ezButton.h>
#include <FastLED.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <DHT22.h>
#include <Defines.h>

ezButton btn[4] = { BTN1, BTN2, BTN3, BTN4 };
Adafruit_SH1106G display = Adafruit_SH1106G(SCR_W, SCR_H, &Wire, -1);
DHT22 dht22(DHT_PIN); 
CRGB leds[LED_NUM];

int ptn[4] = { PTN1, PTN2, PTN3, PTN4 };
int ptn_val[4] = { 0, 0, 0, 0 };
int app_mute[4] = { 0, 0, 0, 0 };
int app_solo[4] = { 0, 0, 0, 0 };
int lastBTNstate[4];
int currBTNstate[4] = { LOW, LOW, LOW, LOW };
String apps[4];
String brightness[10];
// char apps_all[][];

unsigned long dth_time = millis();

void setup() {
  // for (int i = 0; i < 4; i++) {
  //   btn[i].setDebounceTime(50);
  // }

  Serial.begin(115200);
  Serial.println("Hello, ESP32!");

  if(!display.begin(0x3C, true)) {
    Serial.println(F("SH1106 allocation failed"));
    for(;;);
  }
  
  display.clearDisplay();
  display.setTextSize (1);
  display.setTextColor(SH110X_WHITE);

  display.setCursor (0, 8*0);
  display.print("Initialization...");
  display.display();

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, LED_NUM);
  FastLED.setBrightness(32);

  display.setCursor (0, 8*1);
  display.print("Ending previous session...");
  display.display();

  while (Serial.readStringUntil('\r') != "ENDED") {
    Serial.println("END");
  }
  
  display.print("Done.");
  display.display();

  display.setCursor (0, 8*3);
  display.print("Fetching app names...");
  display.display();

  while (Serial.readStringUntil('\r') != "apps") {
    Serial.println("APPS");
  }

  read_apps();
  
  display.print("Done.");
  display.display();
  
  Serial.println("BEGIN");
}

void loop() {
  // serial_read();

  handle_potentiometers(apps);
  handle_buttons();

  temp_humid();

  if (Serial.available() > 0) {
    String data_in = Serial.readStringUntil('\r');
    if (data_in == "apps") {
      read_apps();
    }
    if (data_in == "leds") {
      read_led();
    }
  }

  draw_mixer();

  serial_send(ptn_val, app_mute, app_solo);
}

void draw_mixer(){

  display.clearDisplay();
  display.setTextSize (1);
  display.setTextColor(SH110X_WHITE);
  
  display.setCursor (0, 8*0);
  display.print("Hello!");

  for (int i = 0; i < 4; i++) {
    display.setCursor (0, 8*(i+1));
    display.print(apps[i]);
    display.print(": ");
    display.print(ptn_val[i]);
    display.print("%");

    if (app_mute[i] == 1){
      display.setCursor (120, 8*(i+1));
      display.print("M");
    }
  }
  display.display();
}

void handle_potentiometers(String apps[4]){
  for (int i = 0; i < 4; i++) {
    ptn_val[i] = analogRead(ptn[i])*101/4096;
  }
}

void handle_buttons(){
  // buttons' loop
  for (int i = 0; i < 4; i++) {
    lastBTNstate[i] = currBTNstate[i];
    btn[i].loop();
    currBTNstate[i] = btn[i].getState();

    if(lastBTNstate[i] == HIGH && currBTNstate[i] == LOW) {
      app_mute[i] = !app_mute[i];
    }
  }
}

void serial_send(int vol[4], int mute[4], int solo[4]){
  Serial.print("START");
  Serial.print(",");
  for (int i = 0; i < 4; i++) {
    Serial.print(vol[i]);
    Serial.print(",");
  }
  for (int i = 0; i < 4; i++) {
    Serial.print(mute[i]);
    Serial.print(",");
  }
  for (int i = 0; i < 4; i++) {
    Serial.print(solo[i]);
    Serial.print(",");
  }
  Serial.println("STOP");
}

void read_apps(){
    String data_in = Serial.readStringUntil('\r');
    int r = 0, t = 0;
    for (int i = 0; i < data_in.length(); i++) {
      if(data_in[i] == ',') {
        if (i-r > 1) {
          apps[t] = data_in.substring(r,i);
          t++;
        }
        r = (i+1);
      }
    }
}

void read_led(){
  for (int i = 0; i < 10; i++) {
    int val = Serial.read();
    leds[i].r = val;
    leds[i].g = val;
    leds[i].b = val;
  }
  FastLED.show();
}
