// Shoot a IR remote at the sensor to score a point.
// Transmit result to cpu over bluetooth for processing.

#include "IRremote.h"
#include "Arduino.h"
#include <SoftwareSerial.h>

// Remember: The BT Module will be connected on the inverse:
// I.E. Arduino RxPin -> BT TxPin.
// The BT RxPin __MUST__ go through a voltage divider to 3.3v
#define RxPin 9
#define TxPin 8

#define IRRcvPin 7

// --- Establish objects ---
// -- Infrared --
IRrecv irrecv(IRRcvPin);
uint32_t last_decodedRawData = 0; // store the last received raw data
enum class IRCommands : uint32_t
{
	SHOOT = 0xBF40FF00,
	RESET = 0xB847FF00,
	RELOAD = 0xE619FF00,
	SHOW_SCORE = 0xE916FF00
}

// -- Bluetooth --
SoftwareSerial BTSerial(RxPin, TxPin);

// -- Game Stuff --
int score = 0;
int ammo = 5;

void TranslateIR()
{
	if (irrecv.decodedIRData.flags)
	{
		// Handle repeats
		// ? Does this represent reading at a weird time or holdding down the button?
		// ? If its the latter, this should be ignored. Should press 1x per shot.
		irrecv.decodedIRData.decodedRawData = last_decodedRawData;
	}
	// Translate our button press to a BT Serial Message.
	switch (irrecv.decodedIRData.decodedRawData)
	{
	case IRCommands::SHOOT: // Press Pause/Play registers a hit
		if (ammo > 0)
		{
			BTSerial.println("shoot");
			score += 1;
			ammo -= 1;
		}
		else
		{
			BTSerial.println("click");
		}
		break;

	case IRCommands::RELOAD: // Press EQ to reload
		BTSerial.println("reload");
		ammo = 5;
		break;

	case IRCommands::RESET // Press Func/Stop to reset
		BTSerial.println("reset");
		score = 0;
		// debug helper to clear our screen, other things will presumable occur.
		// This uses a VT1000 code to clear my PuTTY terminal - Your mileage may vary depending on your COM terminal of choice.
		BTSerial.print("\033[0H\033[0J");
		break;

		case IRCommands::SHOW_SCORE: // Press '0' to show score.
		BTSerial.print("Score: ");
		BTSerial.println(score);
		break;

	default:
		Serial.println("other");
		last_decodedRawData = irrecv.decodedIRData.decodedRawData; // Assign our data to the latest thing.
		delay(500);												   // avoid immediate repeats requests
	}
}

void setup()
{
	// Set-up Pin-Modes
	pinMode(RxPin, INPUT);
	pinMode(TxPin, OUTPUT);

	// Setup Serial Outputs
	BTSerial.begin(9600);
	Serial.begin(9600);

	// Setup IR Receiver
	irrecv.enableIRIn();
}

void loop()
{
	// put your main code here, to run repeatedly:
	if (irrecv.decode())
	{
		Serial.println("decoding");
		TranslateIR();
		irrecv.resume(); // Start listening again
	}
}
