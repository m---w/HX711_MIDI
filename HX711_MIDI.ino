/**
 * USB MIDI balance
 *
 * based on
 * HX711 library for Arduino - example file
 * https://github.com/bogde/HX711
 * MIT License
 * (c) 2018 Bogdan Necula
 *
 * and
 * https://github.com/arduino-libraries/MIDIUSB
 *
 * 2021 Marian Weger
 *
**/



#include "HX711.h"

#include "MIDIUSB.h"

// First parameter is the event type (0x09 = note on, 0x08 = note off).
// Second parameter is note-on/note-off, combined with the channel.
// Channel can be anything between 0-15. Typically reported to the user as 1-16.
// Third parameter is the note number (48 = middle C).
// Fourth parameter is the velocity (64 = normal, 127 = fastest).

/*
void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}
*/


// First parameter is the event type (0x0B = control change).
// Second parameter is the event type, combined with the channel.
// Third parameter is the control number number (0-119).
// Fourth parameter is the control value (0-127).

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}


// HX711 circuit wiring
const int LOADCELL1_DOUT_PIN = 2;
const int LOADCELL1_SCK_PIN = 4;
const int LOADCELL2_DOUT_PIN = 7;
const int LOADCELL2_SCK_PIN = 8;

HX711 scale1;
HX711 scale2;


void setup() {

/*
  pinMode(5, INPUT);    // sets the digital pin 13 as output
  pinMode(6, INPUT);    // sets the digital pin 13 as output
  pinMode(9, INPUT);    // sets the digital pin 13 as output
  pinMode(10, INPUT);    // sets the digital pin 13 as output
*/
  /*
  Serial.begin(115200);
  Serial.println("HX711 Demo");

  Serial.println("Initializing the scale");
  */

  // Initialize library with data output pin, clock input pin and gain factor.
  // Channel selection is made by passing the appropriate gain:
  // - With a gain factor of 64 or 128, channel A is selected
  // - With a gain factor of 32, channel B is selected
  // By omitting the gain factor parameter, the library
  // default "128" (Channel A) is used here.
  scale1.begin(LOADCELL1_DOUT_PIN, LOADCELL1_SCK_PIN);

  scale2.begin(LOADCELL2_DOUT_PIN, LOADCELL2_SCK_PIN);

  /*

  Serial.println("Before setting up the scale:");
  Serial.print("read: \t\t");
  Serial.println(scale.read());			// print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));  	// print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));		// print the average of 5 readings from the ADC minus the tare weight (not set yet)

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);	// print the average of 5 readings from the ADC minus tare weight (not set) divided
						// by the SCALE parameter (not set yet)
           */

  scale1.set_scale(2280.f);                      // this value is obtained by calibrating the scale with known weights; see the README for details
  scale1.tare();				        // reset the scale to 0

  scale2.set_scale(2280.f);                      // this value is obtained by calibrating the scale with known weights; see the README for details
  scale2.tare();                // reset the scale to 0

  /*
  Serial.println("After setting up the scale:");

  Serial.print("read: \t\t");
  Serial.println(scale.read());                 // print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));       // print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));		// print the average of 5 readings from the ADC minus the tare weight, set with tare()

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);        // print the average of 5 readings from the ADC minus tare weight, divided
						// by the SCALE parameter set with set_scale

  Serial.println("Readings:");
  */
}

void loop() {
  //Serial.print("one reading:\t");
  //Serial.println(scale.get_units(), 1);
  //Serial.println(scale.get_units(10), 1);
  //Serial.println(1.0);
  //scale.power_down();			        // put the ADC in sleep mode
  controlChange(15, 7, constrain(scale1.get_units(), 0, 127));  // Channel 15, CC 7, value
  controlChange(15, 8, constrain(scale2.get_units(), 0, 127));  // Channel 15, CC 8, value
  MidiUSB.flush();
  //delay(250);
  //scale.power_up();
}
