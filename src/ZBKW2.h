#ifndef ZBKW2_H_INCLUDED
#define ZBKW2_H_INCLUDED


#include <cstddef>

/*
 * ---------------------------------------------------------------------------------------------------------------
-----
 * Code for a USB capable controller and an MFRC522 to read a password from
 *   an NFC card and "type" it out to the host computer.
 * ---------------------------------------------------------------------------------------------------------------
-----
 * This code was written to run on an Adafruit Trinket M0. The following connections need to be made between the
 * Trinket M0 and the MFRC522 board.
 *
 *    TRINKET PIN      MFRC522 PIN
 *        0                SDA
 *        1                RST
 *        2               MISO
 *        3                SCK
 *        4               MOSI
 *        3V              3.3V
 *       Gnd               GND
 *
 * This project has been placed into the public domain.  Anyone may make one of these wedges.
 *
 * This Code will read any of the supported card types and will then search through the data on the card looking f
or
 * the header string "ZPKW" from that location until the following null will be output to the USB port as though t
he
 * wedge was a keyboard followed by the Enter key.
 *
 * Currently the following NFC card types are supported:
 *
 */



unsigned long currentTime;
unsigned long previousTime = 0L;

// Define the Trinket pins, these are Configurable, but the Trinket doesn't have any spare pins.
#define RST_PIN   1
#define SS_PIN    0
#define NUMPIXELS 1
#define DATAPIN   7
#define CLOCKPIN  8
#define LED_PIN   13

#endif // ZBKW2_H_INCLUDED
