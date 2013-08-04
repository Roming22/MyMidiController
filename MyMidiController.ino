/*
 * This code is for a device with 3 momentary switches and 3 LEDs.
 *
 * It is able to send 4 different CC messages.
 *
 * The fist step is to define which pins will be used for input and output.
 * The second step is to define the MIDI controls and link them to the I/O pins.
 * The third step is to create banks and link the selection to an I/O.
 */

#include <MIDI.h>
#include <MidiController.h>

#define SWITCH_COUNT 9 		// How many physical switches we have
#define MIDI_CTRL_COUNT 8	// How many physical switches will send MIDI messages
#define BANK_COUNT 2		// How many banks are going to be created
#define MIDI_CHANNEL 1		// MIDI channel on which the CC will be sent
#define SWITCH_COUNT 9
#define MIDI_CTRL_COUNT 8
#define BANK_COUNT 2

/*
 * Create hardware controllers
 */
hardware::GenericController* gController[17] = {
		// This switch will be used to control the banks
		new hardware::LatchedSwitch(4, true, 5),

		// Switches for Bank0
		new hardware::MomentarySwitch(7, true, 8),
		new hardware::MomentarySwitch(9, true, 10),
		new hardware::LatchedSwitch(11, true, 12),
		new hardware::LatchedSwitch(13, true, 14),
		new hardware::LatchedSwitch(15, true, 16),
		new hardware::LatchedSwitch(26, true, 25),
		new hardware::LatchedSwitch(24, true, 23),
		new hardware::LatchedSwitch(22, true, 21),

		// Switches for Bank1
		new hardware::MomentarySwitch(7, true, 8),
		new hardware::MomentarySwitch(9, true, 10),
		new hardware::LatchedSwitch(11, true, 12),
		new hardware::LatchedSwitch(13, true, 14),
		new hardware::MomentarySwitch(15, true, 16),
		new hardware::MomentarySwitch(26, true, 25),
		new hardware::MomentarySwitch(24, true, 23),
		new hardware::MomentarySwitch(22, true, 21)
};

// The output for the PowerLed will be shared with the bank selector
board::PowerLed gPowerLed(5, 13, 26);

/*
 * Create the MIDI banks
 */
// Bank0
midi::AbstractControl* gBank0[MIDI_CTRL_COUNT] = {
		new midi::SimpleControl(midi::MidiTarget(MIDI_CHANNEL, 30), gController[1]),
		new midi::SimpleControl(midi::MidiTarget(MIDI_CHANNEL, 31), gController[2]),
		new midi::SimpleControl(midi::MidiTarget(MIDI_CHANNEL, 20), gController[3]),
		new midi::SimpleControl(midi::MidiTarget(MIDI_CHANNEL, 21), gController[4]),
		new midi::SimpleControl(midi::MidiTarget(MIDI_CHANNEL, 22), gController[5]),
		new midi::SimpleControl(midi::MidiTarget(MIDI_CHANNEL, 23), gController[6]),
		new midi::SimpleControl(midi::MidiTarget(MIDI_CHANNEL, 24), gController[7]),
		new midi::SimpleControl(midi::MidiTarget(MIDI_CHANNEL, 25), gController[8])
};

// Bank1
midi::AbstractControl* gBank1[MIDI_CTRL_COUNT-1] = {
		new midi::UpDownControl(midi::MidiTarget(MIDI_CHANNEL, 7), gController[9], gController[10], 4),
		new midi::SimpleControl(midi::MidiTarget(MIDI_CHANNEL, 110), gController[11]),
		new midi::SimpleControl(midi::MidiTarget(MIDI_CHANNEL, 111), gController[12]),
		new midi::SimpleControl(midi::MidiTarget(MIDI_CHANNEL, 112), gController[13]),
		new midi::SimpleControl(midi::MidiTarget(MIDI_CHANNEL, 113), gController[14]),
		new midi::SimpleControl(midi::MidiTarget(MIDI_CHANNEL, 114), gController[15]),
		new midi::SimpleControl(midi::MidiTarget(MIDI_CHANNEL, 115), gController[16])
};

// Put the 2 banks together
midi::Bank* gBanks[BANK_COUNT] = { new midi::Bank(gBank0, MIDI_CTRL_COUNT),
		new midi::Bank(gBank1, MIDI_CTRL_COUNT-1) };

// The bank selection is controlled by the first switch
midi::BankSelector gBankSelector((hardware::OnOffController*) gController[0],
		gBanks, BANK_COUNT, true);

void setup() {
	Serial.begin(38400);
	// Turn on PCB LED during boot sequence
	pinMode(6, OUTPUT);
	digitalWrite(6, HIGH);

	gBankSelector.reset();

	// Turn off PCB LED
	digitalWrite(6, LOW);
	Serial.println("Setup OK.");
}

void loop() {
	// Handle the PowerLed first, as it is shared with the BankControl.
	gPowerLed.handle();
	gBankSelector.handle();
	delay(50);
}
