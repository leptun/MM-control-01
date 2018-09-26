// 
// 
// 

#include "Buttons.h"
#include "shr16.h"
#include "tmc2130.h"
#include "mmctl.h"
#include "motion.h"
#include "permanent_storage.h"

const int ButtonPin = A2;

void settings_bowden_length();

void setupMenu()
{
	shr16_set_led(0x000);
	delay(200);
	shr16_set_led(0x2aa);
	delay(1200);
	shr16_set_led(0x000);
	delay(600);

	int _menu = 0;
	bool _exit = false;

		

	do
	{
		shr16_set_led(1 << 2 * 4);
		delay(1);
		shr16_set_led(2 << 2 * 4);
		delay(1);
		shr16_set_led(2 << 2 * _menu);
		delay(1);

		switch (buttonClicked())
		{
		case 1:
			if (_menu > 0) { _menu--; delay(800); }
			break;
		case 2:
				
			switch (_menu)
			{
				case 1:
					settings_bowden_length();
					break;

				case 4: // exit menu
					_exit = true;
					break;
			}
			break;
		case 4:
			if (_menu < 4) { _menu++; delay(800); }
			break;
		}
		
	} while (!_exit);


	shr16_set_led(0x000);
	delay(400);
	shr16_set_led(0x2aa);
	delay(400);
	shr16_set_led(0x000);
	delay(400);

	shr16_set_led(0x000);
	shr16_set_led(1 << 2 * (4 - active_extruder));
}


void settings_bowden_length()
{
	// load filament above Bondtech gears to check correct length of bowden tube
	if (!isFilamentLoaded)
	{
		load_filament_withSensor();

		tmc2130_init_axis_current(0, 1, 30);
		do
		{

			switch (buttonClicked())
			{
			case 1:
				if (lengthCorrection > 0)
				{
					lengthCorrection = lengthCorrection - 1;
					move(0, 0, -10);
					delay(400);
				}
				break;

			case 4:
				if (lengthCorrection < 200)
				{
					lengthCorrection = lengthCorrection + 1;
					move(0, 0, 10);
					delay(400);
				}
				break;
			}

			shr16_set_led(1 << 2 * 4);
			delay(10);
			shr16_set_led(2 << 2 * 4);
			delay(10);
			shr16_set_led(2 << 2 * 1);
			delay(50);


		} while (buttonClicked() != 2);

		eeprom_update_byte(eepromLengthCorrection, lengthCorrection);
		unload_filament_withSensor();
	}
}

//! @brief Is button pushed?
//!
//! @retval 0 No button pushed
//! @retval 1 Button 1 pushed
//! @retval 2 Button 2 pushed
//! @retval 4 Button 3 pushed
int buttonClicked()
{
	int raw = analogRead(ButtonPin);
	int _return = 0;

	 
	if (raw < 50) _return = 1;
	if (raw > 80 && raw < 100) _return = 2;
	if (raw > 160 && raw < 180) _return = 4;

	return(_return);
}

