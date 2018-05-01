#include <SparkFunTSL2561.h>
#include <Wire.h>
#include <avr/power.h>
#include <JeeLib.h>


SFE_TSL2561 light;
#define PIN_POWER_ENABLE_TSL2561 9

ISR(WDT_vect) { Sleepy::watchdogEvent(); }

double readLux() {
  boolean good = false;
  boolean gain = 0;
  double lux;
  unsigned int data0, data1;
  unsigned int ms;
  unsigned char time_char = 2;

  digitalWrite(PIN_POWER_ENABLE_TSL2561, HIGH);

  light.begin();
  light.setTiming(gain, time_char, ms);
  light.setPowerUp();

  while(good != true) {
    Sleepy::loseSomeTime(ms  + 20);
    if (light.getData(data0,data1)) {
      good = light.getLux(gain, ms, data0, data1, lux);
    }
  }
  light.setPowerDown();
  digitalWrite(PIN_POWER_ENABLE_TSL2561, LOW);

  return lux;
}

void setup() {
  pinMode(PIN_POWER_ENABLE_TSL2561, OUTPUT);
  digitalWrite(PIN_POWER_ENABLE_TSL2561, LOW);
  ADCSRA = 0;

  Serial.begin(9600);
  Serial.println("TSL2561");
}

void loop() {
  double lux = readLux();
  Serial.print("Lux: ");
  Serial.println(lux);

  Sleepy::loseSomeTime(9000);
}


