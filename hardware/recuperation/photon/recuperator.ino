#include "OneWire.h"
#define numSensors 8

STARTUP(WiFi.selectAntenna(ANT_AUTO));
// SYSTEM_MODE(SEMI_AUTOMATIC);

OneWire ds = OneWire(D4);

void setup() {}

boolean readTemperature(float (*array)[numSensors]);

void loop(void) {
  float temperatures[numSensors];
  if (!readTemperature(&temperatures)) {
    return;
  }
  String values;
  for (int i = 0; i < numSensors; i++){
    Serial.print(String(temperatures[i], 1));
    Serial.print(" ");
    values += String(temperatures[i], 1);
    values += " ";
  }
  Serial.println();
  Particle.publish("Temperatures", values, 60, PRIVATE);

  delay(1000);
}

boolean readTemperature(float (*array)[numSensors]) {
  ds.reset_search();
  for(int s = 0; s < numSensors; s++) {
    byte present = 0;
    byte data[12];
    byte addr[numSensors];
    float celsius;

    if ( !ds.search(addr) ) {
      ds.reset_search();
      Serial.println("ERROR");
      return false;
    }

    if (OneWire::crc8(addr, 7) != addr[7]) {
      Serial.println("CRC ERROR");
      return false;
    }

    ds.reset();               // first clear the 1-wire bus
    ds.select(addr);          // now select the device we just found
    // ds.write(0x44, 1);     // tell it to start a conversion, with parasite power on at the end
    ds.write(0x44, 0);        // or start conversion in powered mode (bus finishes low)

    // just wait a second while the conversion takes place
    // different chips have different conversion times, check the specs, 1 sec is worse case + 250ms
    // you could also communicate with other devices if you like but you would need
    // to already know their address to select them.

    delay(1000);     // maybe 750ms is enough, maybe not, wait 1 sec for conversion

    // first make sure current values are in the scratch pad
    present = ds.reset();
    ds.select(addr);
    ds.write(0xB8,0);         // Recall Memory 0
    ds.write(0x00,0);         // Recall Memory 0

    // now read the scratch pad

    present = ds.reset();
    ds.select(addr);
    ds.write(0xBE,0);         // Read Scratchpad

    // transfer and print the values
    for (byte i = 0; i < 9; i++) {
      data[i] = ds.read();
    }

    if (data[8] != OneWire::crc8(data, 8)){
      Serial.println(" crc ERROR on data ");
      return false;
    }

    // Convert the data to actual temperature
    // because the result is a 16 bit signed integer, it should
    // be stored to an "int16_t" type, which is always 16 bits
    // even when compiled on a 32 bit processor.
    int16_t raw = (data[1] << 8) | data[0];
    byte cfg = (data[4] & 0x60);

    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    // default is 12 bit resolution, 750 ms conversion time
    (*array)[s] = (float)raw * 0.0625;
  }
  return true;
}
