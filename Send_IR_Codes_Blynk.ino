#define BLYNK_TEMPLATE_ID "TMPL3eIXR5WDZ"
#define BLYNK_TEMPLATE_NAME "SmartAmp"
#define BLYNK_AUTH_TOKEN "Xb7FjJokbtNwCmnHqt6M2znN8Gwel6Ys"

#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Virtual Pin Macros
#define V0_SEGMENTED_SWITCH  V0
#define V17_MUTE             V17
#define V15_FAN             V15
#define V16_LIGHT           V16
#define V2_RESET            V20
#define V2_MASTER_PLUS      V2
#define V3_MASTER_MINUS     V3
#define V4_FRONT_PLUS       V4
#define V5_FRONT_MINUS      V5
#define V6_SURROUND_PLUS    V6
#define V7_SURROUND_MINUS   V7
#define V8_CENTRE_PLUS      V8
#define V9_CENTRE_MINUS     V9
#define V10_SUB_PLUS        V10
#define V11_SUB_MINUS       V11
#define V19_RIGHT           V19
#define V18_LEFT            V18
#define V12_UP              V12
#define V13_DOWN            V13
#define V1_OK               V1

// Hardware Pins
const int IR_PIN = D2;
const int FAN_PIN = D3;
const int LIGHT_PIN = D4;

IRsend irsend(IR_PIN);

// WiFi credentials
char ssid[] = "Black_Pearl";
char pass[] = "Karthik_Ravi";

// IR Hex Codes
#define OPTICAL      0x10EF42BD
#define HDMI         0x10EF02FD
#define TV           0x10EF52AD  // Using ARC code for TV
#define USB          0x10EF629D
#define BLUETOOTH    0x10EFA25D
#define MUTE         0x10EFF00F
#define FRONT_PLUS   0x10EF40BF
#define FRONT_MINUS  0x10EFC03F
#define CENTRE_PLUS  0x10EF50AF
#define CENTRE_MINUS 0x10EF609F
#define MASTER_PLUS  0x10EF906F
#define MASTER_MINUS 0x10EFA05F
#define SURROUND_PLUS 0x10EF00FF
#define SURROUND_MINUS 0x10EF807F
#define SUB_PLUS     0x10EFD02F
#define SUB_MINUS    0x10EFE01F
#define RESET_CODE   0x10EF1AE5
#define UP           0x10EF926D
#define DOWN         0x10EFB04F
#define LEFT         0x10EF728D
#define RIGHT        0x10EF32CD
#define OK_CODE      0x10EFB24D
#define INFO         0x10EFD827 
#define MODE         0x10EF9867 
#define AMODE        0x10EF12ED 
#define TONE         0x10EFC837 
#define TEST         0x10EF30CF 
#define AUX          0x10EF22DD 
#define POWER        0x10EF827D

void setup() {
  Serial.begin(115200);
  Serial.println("\nSystem Initializing...");
  
  pinMode(FAN_PIN, OUTPUT);
  pinMode(LIGHT_PIN, OUTPUT);
  irsend.begin();

  Serial.println("Connecting to WiFi...");
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  
  // Sync initial states
  digitalWrite(FAN_PIN, LOW);
  digitalWrite(LIGHT_PIN, LOW);
  Blynk.virtualWrite(V15_FAN, LOW);
  Blynk.virtualWrite(V16_LIGHT, LOW);

  Serial.println("System Ready!");
  Serial.println("======================");
}

BLYNK_CONNECTED() {
  Serial.println("Connected to Blynk Server!");
}

BLYNK_WRITE(V0_SEGMENTED_SWITCH) {
  int mode = param.asInt();
  Serial.print("Mode Changed [V0]: ");
  switch(mode) {
    case 0: 
      irsend.sendNEC(OPTICAL, 32);
      Serial.println("Optical");
      break;
    case 1: 
      irsend.sendNEC(HDMI, 32);
      Serial.println("HDMI");
      break;
    case 2: 
      irsend.sendNEC(TV, 32);
      Serial.println("TV");
      break;
    case 3: 
      irsend.sendNEC(USB, 32);
      Serial.println("USB");
      break;
    case 4: 
      irsend.sendNEC(BLUETOOTH, 32);
      Serial.println("Bluetooth");
      break;
  }
  Serial.println("Sent IR Code: 0x" + String(
    mode == 0 ? OPTICAL :
    mode == 1 ? HDMI :
    mode == 2 ? TV :
    mode == 3 ? USB : BLUETOOTH, HEX));
}

BLYNK_WRITE(V17_MUTE) {
  irsend.sendNEC(MUTE, 32);
  Serial.print("Mute Toggled [V17]: ");
  Serial.println(param.asInt() ? "ON" : "OFF");
  Serial.println("Sent IR Code: 0x" + String(MUTE, HEX));
}

BLYNK_WRITE(V15_FAN) {
  int state = param.asInt();
  digitalWrite(FAN_PIN, state);
  Serial.print("Fan Control [V15]: ");
  Serial.println(state ? "ON (D3)" : "OFF (D3)");
  Blynk.virtualWrite(V15_FAN, state);
}

BLYNK_WRITE(V16_LIGHT) {
  int state = param.asInt();
  digitalWrite(LIGHT_PIN, state);
  Serial.print("Light Control [V16]: ");
  Serial.println(state ? "ON (D4)" : "OFF (D4)");
  Blynk.virtualWrite(V16_LIGHT, state);
}

// BLYNK_WRITE(V2_RESET) {
//   if(param.asInt()) {
//     irsend.sendNEC(RESET_CODE, 32);
//     Serial.println("Reset Triggered [V13]");
//     Serial.println("Sent IR Code: 0x" + String(RESET_CODE, HEX));
//   }
// }

// Push Button Handlers with Debug
BLYNK_WRITE(V2_MASTER_PLUS) {
  if(param.asInt() == 1) {
    irsend.sendNEC(MASTER_PLUS, 32);
    Serial.println("Master+ Pressed [V2]");
    Serial.println("Sent IR Code: 0x" + String(MASTER_PLUS, HEX));
  }
  else if(param.asInt() == 3) {
    irsend.sendNEC(RESET_CODE, 32);
    Serial.println("Reset Triggered [V2]");
    Serial.println("Sent IR Code: 0x" + String(RESET_CODE, HEX));
  }
}

BLYNK_WRITE(V3_MASTER_MINUS) {
  if(param.asInt()) {
    irsend.sendNEC(MASTER_MINUS, 32);
    Serial.println("Master- Pressed [V3]");
    Serial.println("Sent IR Code: 0x" + String(MASTER_MINUS, HEX));
  }
}

BLYNK_WRITE(V4_FRONT_PLUS) {
  if(param.asInt()) {
    irsend.sendNEC(FRONT_PLUS, 32);
    Serial.println("Front+ Pressed [V4]");
    Serial.println("Sent IR Code: 0x" + String(FRONT_PLUS, HEX));
  }
}

BLYNK_WRITE(V5_FRONT_MINUS) {
  if(param.asInt()) {
    irsend.sendNEC(FRONT_MINUS, 32);
    Serial.println("Front- Pressed [V5]");
    Serial.println("Sent IR Code: 0x" + String(FRONT_MINUS, HEX));
  }
}

BLYNK_WRITE(V6_SURROUND_PLUS) {
  if(param.asInt()) {
    irsend.sendNEC(SURROUND_PLUS, 32);
    Serial.println("Surround+ Pressed [V6]");
    Serial.println("Sent IR Code: 0x" + String(SURROUND_PLUS, HEX));
  }
}

BLYNK_WRITE(V7_SURROUND_MINUS) {
  if(param.asInt()) {
    irsend.sendNEC(SURROUND_MINUS, 32);
    Serial.println("Surround- Pressed [V7]");
    Serial.println("Sent IR Code: 0x" + String(SURROUND_MINUS, HEX));
  }
}

BLYNK_WRITE(V8_CENTRE_PLUS) {
  if(param.asInt()) {
    irsend.sendNEC(CENTRE_PLUS, 32);
    Serial.println("Centre+ Pressed [V8]");
    Serial.println("Sent IR Code: 0x" + String(CENTRE_PLUS, HEX));
  }
}

BLYNK_WRITE(V9_CENTRE_MINUS) {
  if(param.asInt()) {
    irsend.sendNEC(CENTRE_MINUS, 32);
    Serial.println("Centre- Pressed [V9]");
    Serial.println("Sent IR Code: 0x" + String(CENTRE_MINUS, HEX));
  }
}

BLYNK_WRITE(V10_SUB_PLUS) {
  if(param.asInt()) {
    irsend.sendNEC(SUB_PLUS, 32);
    Serial.println("Sub+ Pressed [V10]");
    Serial.println("Sent IR Code: 0x" + String(SUB_PLUS, HEX));
  }
}

BLYNK_WRITE(V11_SUB_MINUS) {
  if(param.asInt()) {
    irsend.sendNEC(SUB_MINUS, 32);
    Serial.println("Sub- Pressed [V11]");
    Serial.println("Sent IR Code: 0x" + String(SUB_MINUS, HEX));
  }
}

BLYNK_WRITE(V19_RIGHT) {
  if(param.asInt()) {
    irsend.sendNEC(RIGHT, 32);
    Serial.println("Right Pressed [V19]");
    Serial.println("Sent IR Code: 0x" + String(RIGHT, HEX));
  }
}

BLYNK_WRITE(V18_LEFT) {
  if(param.asInt()) {
    irsend.sendNEC(LEFT, 32);
    Serial.println("Left Pressed [V18]");
    Serial.println("Sent IR Code: 0x" + String(LEFT, HEX));
  }
}

BLYNK_WRITE(V12_UP) {
  if(param.asInt()) {
    irsend.sendNEC(UP, 32);
    Serial.println("Up Pressed [V12]");
    Serial.println("Sent IR Code: 0x" + String(UP, HEX));
  }
}

BLYNK_WRITE(V13_DOWN) {
  if(param.asInt()) {
    irsend.sendNEC(DOWN, 32);
    Serial.println("Down Pressed [V20]");
    Serial.println("Sent IR Code: 0x" + String(DOWN, HEX));
  }
}

BLYNK_WRITE(V1_OK) {

  int mode = param.asInt();
  if (mode == 1 || mode == 3 || mode == 5 || mode == 7 || 
    mode == 9 || mode == 11 || mode == 13 || mode == 15) {
    Serial.print("Button pressed [V1]: ");
  }
  switch(mode) {
    case 1: 
      irsend.sendNEC(OK_CODE, 32);
      Serial.println("Okay");
      break;
    case 3: 
      irsend.sendNEC(AUX, 32);
      Serial.println("AUX");
      break;
    case 5: 
      irsend.sendNEC(AMODE, 32);
      Serial.println("Amode");
      break;
    case 7: 
      irsend.sendNEC(TONE, 32);
      Serial.println("Tone");
      break;
    case 9: 
      irsend.sendNEC(TEST, 32);
      Serial.println("Test");
      break;
    case 11: 
      irsend.sendNEC(MODE, 32);
      Serial.println("Mode");
      break;
    case 13: 
      irsend.sendNEC(INFO, 32);
      Serial.println("Info");
      break;
    case 15: 
      irsend.sendNEC(POWER, 32);
      Serial.println("Power");
      break;
  }

  String codeToPrint = (
    mode == 1 ? "0x" + String(OK_CODE, HEX) :
    mode == 3 ? "0x" + String(AUX, HEX) :
    mode == 5 ? "0x" + String(AMODE, HEX) :
    mode == 7 ? "0x" + String(TONE, HEX) :
    mode == 9 ? "0x" + String(TEST, HEX) :
    mode == 11 ? "0x" + String(MODE, HEX) :
    mode == 13 ? "0x" + String(INFO, HEX) :
    mode == 15 ? "0x" + String(POWER, HEX) :
    ""  // If mode doesn't match, return empty string
);

if (codeToPrint != "") {
  Serial.println("Sent IR Code: " + codeToPrint);
}

  // Serial.println("Sent IR Code: 0x" + String(
  //   mode == 1 ? OK_CODE :
  //   mode == 3 ? AUX :
  //   mode == 5 ? AMODE :
  //   mode == 7 ? TONE :
  //   mode == 11 ? MODE :
  //   mode == 13 ? INFO :
  //   mode == 15 ? INFO : , HEX));
  
}

void loop() {
  Blynk.run();
}