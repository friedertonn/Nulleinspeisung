// solardimmer.ino  17.02.2023

// Betrieb des RBDdimmers über 3 Steuerleitungen (Prinzip der Nachregelung)
// D1 = GPIO 5
// D2 = GPIO 4
// D5 = GPIO 14
// +------+------+------+----------------------------+--------------------+
// |  D5  |  D2  |  D1  |          Zustand           |  Aktion            |
// +------+------+------+--------------+-------------+--------------------+
// | HIGH | HIGH | HIGH | Bezug        | < -500 Watt | Dimmer-Wert = 0%   |
// | HIGH | HIGH |  LOW | aus dem      | < -100 Watt | -3% dimmen         |
// | HIGH |  LOW | HIGH | öffentlichen | <  -20 Watt | -2% dimmen         |
// | HIGH |  LOW |  LOW | Netz         | <    0 Watt | -1% dimmen         |
// |  LOW | HIGH | HIGH |--------------| <   20 Watt | keine Nachregelung |
// |  LOW | HIGH |  LOW | Einspeisung  | <   40 Watt | +1% dimmen         |
// |  LOW |  LOW | HIGH | in das       | <  120 Watt | +1% dimmen         |
// |  LOW |  LOW |  LOW | öff. Netz    | >= 120 Watt | +2% dimmen         |
// +------+------+------+--------------|-------------+--------------------+

#include "RBDdimmer.h"

#define outputPin  12  // D6 = GPIO 12
#define zerocross  13  // D7 = GPIO 13
dimmerLamp dimmer(outputPin, zerocross);

int debug = 0;     // serielle Konsole
int state = 0;     // Dimmer ON/OFF
int wert = 0;      // Dimmer-Wert in %
int maxwert = 40;  // max. Dimmer-Wert in %
int timer = 0;     // Timer für Auto-OFF
int wattfak = 12;  // Faktor für Umrechnung % --> Watt

void setup() {
  pinMode(5, INPUT_PULLUP);   // D1
  pinMode(4, INPUT_PULLUP);   // D2
  pinMode(14, INPUT_PULLUP);  // D5
  if (debug > 0) Serial.begin(115200);
  dimmer.begin(NORMAL_MODE, OFF);
  dimmer.setPower(wert);
  delay(500);
}

void loop() 
{
  int pin1 = digitalRead(5);   // D1
  int pin2 = digitalRead(4);   // D2
  int pin5 = digitalRead(14);  // D5
  if (debug > 1) {
    Serial.print("D5/D2/D1="); Serial.print(pin5); 
    Serial.print("/"); Serial.print(pin2);
    Serial.print("/"); Serial.print(pin1); Serial.print("  ");
  }

  if ((pin5 == 1) && (pin2 == 1) && (pin1 == 1)) wert = 0;
  if ((pin5 == 1) && (pin2 == 1) && (pin1 == 0)) wert -= 3;
  if ((pin5 == 1) && (pin2 == 0) && (pin1 == 1)) wert -= 2;
  if ((pin5 == 1) && (pin2 == 0) && (pin1 == 0)) wert -= 1;
  if ((pin5 == 0) && (pin2 == 1) && (pin1 == 0)) wert += 1;
  if ((pin5 == 0) && (pin2 == 0) && (pin1 == 1)) wert += 1;
  if ((pin5 == 0) && (pin2 == 0) && (pin1 == 0)) wert += 2;
  if (wert < 0) wert = 0;
  if (wert > maxwert) wert = maxwert;
  
  if ((state == 0) && (wert > 0)) {  // Dimmer einschalten, wenn eine Einspeisung erfolgt
    state = 1;
    dimmer.setState(ON);
  }

  if ((state == 1) && (wert <= 0)) timer++;  // Dimmer nach 10 Minuten Inaktivität ausschalten
  else timer = 0;
  if (timer >= 600) {
    timer = 0;
    state = 0;
    dimmer.setState(OFF);
  }
  
  if (state == 1) dimmer.setPower(wert);  // Dimmer einstellen: setPower(0%-100%)
  if (debug > 0) {
    Serial.print("Status="); Serial.print(state);  
    Serial.print("  %-Wert="); Serial.print(wert);  
    Serial.print("  Timer="); Serial.print(timer);  
    Serial.print("  Watt="); Serial.println(wert * wattfak);  
  }
  delay(2500);
}
