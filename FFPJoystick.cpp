/*
  Test.h - Test library for Wiring - implementation
  Copyright (c) 2006 John Doe.  All right reserved.
*/

// include core Wiring API


// include this library's description file
#include "FFPJoystick.h"
// Data structures for input reports from the joystick positions

// include description files for other libraries used (if any)
//#include "HardwareSerial.h"

// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances

FFPJoystick::FFPJoystick()
{
SetupHardware();
}

/** Configures the board hardware and chip peripherals for the functionality. */
void FFPJoystick::SetupHardware(void)
{
/* Disable watchdog if enabled by bootloader/fuses */
MCUSR &= ~(1 << WDRF);
wdt_disable();

/* Disable clock division */
clock_prescale_set(clock_div_1);

/* Hardware Initialization */
//LEDs_Init();

// Call the joystick's init and connection methods
//Joystick_Init();
    uint8_t sw_sendrep ;

    // Initialize..

    init_hw() ;					// hardware. Note: defined as naked !

	WaitMs(1000);
	// Force feedback
	FfbInitMidi();	
	sei();	
}

void FFPJoystick::EnableAutoCenter(void)
{
	FfbSendData(enableAutoCenterFfbData_1, 2);	
}

void FFPJoystick::DisableAutoCenter(void)
{
	FfbSendData(disableAutoCenterFfbData_1, 2);//second parameter is number of bytes
	_delay_ms(35);
	_delay_ms(40);
	FfbSendData(disableAutoCenterFfbData_2, 8);//would like to use sizeof but can't,
											   //since these variables are declared elsewhere
}

void FFPJoystick::forceTest()
{
/*
	uint8_t command;	// always 0x23	-- start counting checksum from here
	uint8_t waveForm;	// 2=sine, 5=Square, 6=RampUp, 7=RampDown, 8=Triange, 0x12=Constant
	uint8_t unknown1;	// ? always 0x7F
	uint16_t duration;	// unit=2ms
	uint16_t unknown2;	// ? always 0x0000
	uint16_t direction;
	uint8_t	unknown3[5];	// ? always 7f 64 00 10 4e
	uint8_t attackLevel;
	uint16_t	attackTime;
	uint8_t		magnitude;
	uint16_t	fadeTime;
	uint8_t	fadeLevel;
	uint8_t	waveLength;	// 0x6F..0x01 => 1/Hz
	uint8_t unknown5;	// ? always 0x00
	uint16_t param1;	// Varies by effect type; Constant: positive=7f 00, negative=01 01, Other effects: 01 01
	uint16_t param2;	// Varies by effect type; Constant: 00 00, Other effects 01 01
	} FFP_MIDI_Effect_Basic;*/
	
	FFP_MIDI_Effect_Basic test;
	FFP_MIDI_Effect_Basic* midi_data = &test;
	
		midi_data->duration = 0x5a19;
		midi_data->magnitude = 0x7f;
		midi_data->waveLength = 0x01;
		midi_data->waveForm = 0x02;
		midi_data->attackLevel = 0x7f;
		midi_data->attackTime = 0x0000;
		midi_data->fadeLevel = 0x7f;
		midi_data->fadeTime = 0x5a19;

		// Constants
		midi_data->command = 0x23;
		midi_data->unknown1 = 0x7F;
		midi_data->unknown2 = 0x0000;
		midi_data->unknown3[0] = 0x7F;
		midi_data->unknown3[1] = 0x64;
		midi_data->unknown3[2] = 0x00;
		midi_data->unknown3[3] = 0x10;
		midi_data->unknown3[4] = 0x4E;

		if (midi_data->waveForm == 0x12)	// constant
			midi_data->param2 = 0x0000;
		else
			midi_data->param2 = 0x0101;	
	
	// All effects data start with this data
uint8_t constantLeftFfbData[] =
	{
		0xf0,	// define
			0x00, 0x01, 0x0a, 0x01, 0x23, 0x05, 0x7f, 0x5a, 0x19, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x64, 0x00, 0x10, 0x4e, 0x7f, 0x00, 0x00, 0x7f, 0x5a, 0x19, 0x7f, 0x01, 0x00, 0x7f, 0x00, 0x01, 0x01, 0x33,
		0xf7,
		0xb5, 0x20, 0x02	// play
	};
	
uint8_t sineFfbData[] =
	{
		0xf0,	// define
			0x00, 0x01, 0x0a, 0x01, //start sequence
			0x23,//command
			0x05,//square
			0x7f,//always same
			0x5a, 0x19,//duration in 2ms intervals
			0x00, 0x00,//unknown
			0x00, 0x00,//direction
			0x7f, 0x64, 0x00, 0x10, 0x4e,//unknown 3
			0x7f,//envelope attack level
			0x00,0x00,//envelope attack time
			0x7f,//magnitude
			0x5a,0x19,//envelope fade time
			0x7f,//envelope fade level
			0x01,//wavelength 
			0x00,//constant dir
			0x7f,//constant dir
			0x00, 0x01, 0x01, 0x33,
		0xf7,
		0xb5, 0x20, 0x02	// play
	};	
	//FfbSendSysEx((uint8_t*) constantLeftFfbData, sizeof(constantLeftFfbData));
	//FfbSendEffectOper(2, 0x20);	
	FfbSendData(sineFfbData,sizeof(sineFfbData));
}

void FFPJoystick::Poll(void)
{
	SetTMPS( 0, 64 ) ;		// Set T0 prescaler to / 64 for query
	getdata();
	// Convert the raw input data to USB report

	reportId = 1;	// Input report ID 1

	// Convert data from Sidewinder Force Feedback Pro
	X = sw_report[0] + ((sw_report[1] & 0x03) << 8);
	if (sw_report[1] & 0x02)
		X |= (0b11111100 << 8);
	Y = (sw_report[1] >> 2) + ((sw_report[2] & 0x0F) << 6);
	if (sw_report[2] & 0x08)
		Y |= (0b11111100 << 8);
	Button = ((sw_report[4] & 0x7F) << 2) + ((sw_report[3] & 0xC0) >> 6);
	Hat = sw_report[2] >> 4;
	Rz = (sw_report[3] & 0x3f) - 32;
	Z = ((sw_report[5] & 0x3f) << 1) + (sw_report[4] >> 7);
	if (sw_report[5] & 0x20)
		Z |= 0b11000000;
		
	if (Z > 63)
		Z = -1*(255-Z);
	//if ((int8_t)(Z) < 0) 
		//Z = Z*-1+64;

	Rudder = 0;	// not used at the moment	

//FfbSendData(enableAutoCenterFfbData_1, 2);
}