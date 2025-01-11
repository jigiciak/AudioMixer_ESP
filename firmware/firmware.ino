#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <ezButton.h>
#include <FastLED.h>

#define BTN1 2
#define BTN2 4
#define BTN3 5
#define BTN4 18
ezButton button1(BTN1);
ezButton button2(BTN2);
ezButton button3(BTN3);
ezButton button4(BTN4);

#define PTN1 33
#define PTN2 32
#define PTN3 35
#define PTN4 34

#define SCR_H 64
#define SCR_W 128

#define LED_PIN 25
#define LED_NUM 10

#define I2C_SDA 21
#define I2C_SCL 22

Adafruit_SSD1306 display(SCR_W, SCR_H, &Wire, -1);
CRGB leds[LED_NUM];

void setup() {
  button1.setDebounceTime(50);
  button2.setDebounceTime(50);
  button3.setDebounceTime(50);
  button4.setDebounceTime(50);

  pinMode(PTN1, INPUT);
  pinMode(PTN2, INPUT);
  pinMode(PTN3, INPUT);
  pinMode(PTN4, INPUT);
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, LED_NUM);
  // FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, LED_NUM);

  Serial.begin(115200);
  Wire.begin(I2C_SDA, I2C_SCL);
  Serial.println("Hello, ESP32!");
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
}

int ptn1_val;
int ptn2_val;
int ptn3_val;
int ptn4_val;

int app1_mute = 0;
int app2_mute = 0;
int app3_mute = 0;
int app4_mute = 0;

bool app1_solo;
bool app2_solo;
bool app3_solo;
bool app4_solo;

int lastBTN1state;
int lastBTN2state;
int lastBTN3state;
int lastBTN4state;

int currentBTN1state = button1.getState();
int currentBTN2state = button2.getState();
int currentBTN3state = button3.getState();
int currentBTN4state = button4.getState();

void led_control(int set_to) {
  for (int i = 0; i < LED_NUM; i++) {
    switch (set_to) {
      case '1':
        leds[i] = CRGB::White;
        break;
      case '2':
        leds[i] = CRGB::Green;
        break;
    }
  }
  FastLED.show();
}

void loop() {
  lastBTN1state = currentBTN1state;
  lastBTN2state = currentBTN2state;
  lastBTN3state = currentBTN3state;
  lastBTN4state = currentBTN4state;

  button1.loop();
  button2.loop();
  button3.loop();
  button4.loop();

  currentBTN1state = button1.getState();
  currentBTN2state = button2.getState();
  currentBTN3state = button3.getState();
  currentBTN4state = button4.getState();

  ptn1_val = analogRead(PTN1)/32;
  ptn2_val = analogRead(PTN2)/32;
  ptn3_val = analogRead(PTN3)/32;
  ptn4_val = analogRead(PTN4)/32;

  if(lastBTN1state == HIGH && currentBTN1state == LOW) {
    app1_mute = !app1_mute;
  }
  if(lastBTN2state == HIGH && currentBTN2state == LOW) {
    app2_mute = !app2_mute;
  }
  if(lastBTN3state == HIGH && currentBTN3state == LOW) {
    app3_mute = !app3_mute;
  }
  if(lastBTN4state == HIGH && currentBTN4state == LOW) {
    app4_mute = !app4_mute;
  }
  
  display.clearDisplay();
  display.setTextSize (1);
  display.setTextColor(WHITE);
  display.setCursor (0, 8*0);
  display.print("Hello!");
  display.setCursor (0, 8*1);
  display.print("APP1 Vol: ");
  display.print(ptn1_val);
  display.setCursor (0, 8*2);
  display.print("APP2 Vol: ");
  display.print(ptn2_val);
  display.setCursor (0, 8*3);
  display.print("APP3 Vol: ");
  display.print(ptn3_val);
  display.setCursor (0, 8*4);
  display.print("APP4 Vol: ");
  display.print(ptn4_val);
  if (app1_mute == 1) {
    display.setCursor (80, 8*1);
    display.print("Muted");
  }
  if (app2_mute == 1) {
    display.setCursor (80, 8*2);
    display.print("Muted");
  }
  if (app3_mute == 1) {
    display.setCursor (80, 8*3);
    display.print("Muted");
  }
  if (app4_mute == 1) {
    display.setCursor (80, 8*4);
    display.print("Muted");
  }
  display.display();
  if (Serial.available()) {
    led_control(Serial.read());
  }  
}
