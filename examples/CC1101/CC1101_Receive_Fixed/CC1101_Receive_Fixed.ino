/*
   RadioLib CC1101 Receive Example (fixed length mode)

   This example receives packets using CC1101 FSK radio module.
   To successfully receive data, the following settings have to be the same
   on both transmitter and receiver:
    - carrier frequency
    - bit rate
    - frequency deviation
    - sync word

   For full API reference, see the GitHub Pages
   https://jgromes.github.io/RadioLib/
*/

// receive 9 bytes of payload after the sync word
#define FIXED_PKT_LEN 9

// include the library
#include <RadioLib.h>

// CC1101 has the following connections:
// NSS pin:   10
// GDO0 pin:  2
// GDO2 pin:  3
CC1101 cc = new Module(10, 2, 3);

// or using RadioShield
// https://github.com/jgromes/RadioShield
//CC1101 cc = RadioShield.ModuleA;

void setup() {
  Serial.begin(9600);

  // initialize CC1101 with default settings
  Serial.print(F("[CC1101] Initializing ... "));
  // carrier frequency:                   868.0 MHz
  // bit rate:                            4.8 kbps
  // Rx bandwidth:                        325.0 kHz
  // frequency deviation:                 48.0 kHz
  // sync word:                           0xD391
  int state = cc.begin();

  // Fixed packet length mode (non-default setting)
  state = cc.fixedPacketLengthMode(FIXED_PKT_LEN);

  if (state == ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }
}

void loop() {
  Serial.print(F("[CC1101] Waiting for incoming transmission ... "));

  // you can receive data as an Arduino String
  String str;
  int state = cc.receive(str);

  // you can also receive data as byte array
  /*
    byte byteArr[8];
    int state = cc.receive(byteArr, 8);
  */

  if (state == ERR_NONE) {
    // packet was successfully received
    Serial.println(F("success!"));

    // print the data of the packet
    Serial.print(F("[CC1101] Data:\t\t"));
    Serial.println(str);

    // print RSSI (Received Signal Strength Indicator)
    // of the last received packet
    Serial.print(F("[CC1101] RSSI:\t\t"));
    Serial.print(cc.getRSSI());
    Serial.println(F(" dBm"));

    // print LQI (Link Quality Indicator)
    // of the last received packet, lower is better
    Serial.print(F("[CC1101] LQI:\t\t"));
    Serial.println(cc.getLQI());

  } else if (state == ERR_CRC_MISMATCH) {
    // packet was received, but is malformed
    Serial.println(F("CRC error!"));

  } else {
    // some other error occurred
    Serial.print(F("failed, code "));
    Serial.println(state);

  }
}
