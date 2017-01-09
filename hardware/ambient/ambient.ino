#include "rgb-controls.h"
using namespace RGBControls;

Led ambient(D0, D1, D2);
Color red(255, 0, 0);
Color green(0, 255, 0);
Color blue(0, 0, 255);
const int CLOSET_PIN = D5;
const int PICTURE_PIN = D6;
const int WHITE_PIN = D3;
bool turnAmbient = false;
int ambient_delay = 30000;

Color colors[3] = { red, green, blue };

int closetStatus = 0;
int pictureStatus = 0;

void setup() {
 pinMode(WHITE_PIN, OUTPUT);
 pinMode(CLOSET_PIN, OUTPUT);
 pinMode(PICTURE_PIN, OUTPUT);
 analogWrite(WHITE_PIN, 0);
 digitalWrite(CLOSET_PIN, LOW);
 digitalWrite(PICTURE_PIN, LOW);

 Particle.function("light-on", lightOn);
 Particle.function("light-off", lightOff);
 Particle.function("ambient-on", ambientOn);
 Particle.function("ambient-off", ambientOff);
 // Particle.function("set-color-delay", ambient_color_delay);

 Particle.variable("color-delay", ambient_delay);
 Particle.variable("closet", closetStatus);
 Particle.variable("picture", pictureStatus);
}

void loop() {
  Particle.publish("wifi", String(WiFi.RSSI()), 60, PRIVATE);
  delay(1000);
  if (turnAmbient)
    ambient.fade(colors, 3, ambient_delay);
}

int lightOn(String arg) {
  if (arg == "closet") {
    digitalWrite(CLOSET_PIN, HIGH);
    closetStatus = 1;
  } else if (arg == "picture") {
    digitalWrite(PICTURE_PIN, HIGH);
    pictureStatus = 1;
  } else if (arg == "white")
    analogWrite(WHITE_PIN, 255);
  return 1;
}

int lightOff(String arg) {
  if (arg == "closet") {
    digitalWrite(CLOSET_PIN, LOW);
    closetStatus = 0;
  } else if (arg == "picture") {
    digitalWrite(PICTURE_PIN, LOW);
    pictureStatus = 0;
  } else if (arg == "white")
    analogWrite(WHITE_PIN, 0);
  return 1;
}

int ambientOn(String arg) {
  turnAmbient = true;
  if (!arg.equals("null"))
    ambient_delay = arg.toInt();
  pub("ambient/on", arg);
  return 1;
}

int ambientOff(String arg) {
  turnAmbient = false;
  ambient.off();
  pub("ambient/off");
  return 1;
}

// int ambient_color_delay(String arg) {
//   ambient_delay = arg.toInt();
//   // pub('ambient/set-delay', arg);
//   return 1;
// }

void pub(const char* eventName, const char* data) {
  Particle.publish(eventName, data, 604800, PRIVATE);
}

void pub(const char* eventName) {
  pub(eventName, "1");
}
