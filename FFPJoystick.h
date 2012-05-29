/*
  Test.h - Test library for Wiring - description
  Copyright (c) 2006 John Doe.  All right reserved.
*/

// ensure this library description is only included once
#ifndef FFPJoystick_h
#define FFPJoystick_h
#include "WProgram.h"
// include types & constants of Wiring core API
#include "WConstants.h"
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <string.h>
#include <includes.h>

extern "C"{
//#include "ffb.h"
#include "3DPro.h"
//#include "Joystick.h"
//#include "debug.h"
}



//extern	uint8_t enableAutoCenterFfbData_1[];

// library interface description
class FFPJoystick
{
  // user-accessible "public" interface
  public:
    FFPJoystick();
	void SetupHardware(void);
	void Poll();
  
		// Joystick Input Report
	uint8_t	reportId;	// =1
	int16_t  X;
	int16_t  Y;
	int16_t  Z;
	int8_t  Rz, Rx, Ry;
	uint8_t  Rudder;
	uint8_t  Throttle;
	uint16_t Button;
	uint8_t Hat;
};

#endif

