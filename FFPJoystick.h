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
#include <util/delay.h>

extern "C"{
#include "ffb.h"
#include "3DPro.h"
//#include "Joystick.h"
//#include "debug.h"
}



extern	uint8_t enableAutoCenterFfbData_1[];
extern  uint8_t disableAutoCenterFfbData_1[];
extern  uint8_t disableAutoCenterFfbData_2[];

// library interface description
class FFPJoystick
{
  // user-accessible "public" interface
  public:
    FFPJoystick();
	void EnableAutoCenter();
	void DisableAutoCenter();
	void SetupHardware();
	void Poll();
	uint8_t forceTest(uint8_t wavelength);
	uint8_t CreateConstantForce(uint8_t magnitude,uint16_t direction);
	void updateConstantForce(uint8_t magnitude,uint16_t direction);
	void updateWaveLength(uint8_t effectID,uint8_t wavelength);
	void playEffect(uint8_t effectID);
	 
	// Joystick Input Report
	uint8_t	reportId;	// =1
	int16_t  X;
	int16_t  Y;
	//int16_t  Z;
	//int8_t  Rz, Rx, Ry;
	int8_t Rz;
	//uint8_t  Rudder;
	int16_t  Throttle;
	uint16_t Button;
	uint8_t Hat;
	int effectId;
};

#endif

