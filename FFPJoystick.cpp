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