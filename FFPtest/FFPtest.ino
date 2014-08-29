#include <FFPJoystick.h>


FFPJoystick stick;
uint8_t magnitude = 20;
uint8_t vibration;

void setup()
{

//  stick.EnableAutoCenter();
//stick.DisableAutoCenter();
 Serial.begin(115200);
// stick.CreateConstantForce(0,0);
}
void loop()
{
  stick.Poll();
/*  Serial.print(stick.effectId);
  Serial.print(" ");
  Serial.print(stick.constantForceID);  
  Serial.print(stick.Throttle);
  Serial.print(" ");
  for(int i=0;i<9;i++)
  {
  Serial.print(stick.getButton(i));
  Serial.print(" ");
  }
  Serial.println(stick.Button,BIN);*/
  
  vibration = (126 - (stick.Throttle + 63));
  Serial.print(magnitude);
  Serial.print(" ");
  Serial.println(vibration);
  
  if(stick.getButton(0))
    magnitude++;
  if(stick.getButton(1))
    magnitude--;  
    stick.sendVibration(magnitude,vibration);
//  stick.forceTest(60);
  delay(50);
/* if(stick.Throttle > 0)
   stick.sendConstantForce(abs(stick.Throttle),0);
  else
    stick.sendConstantForce(abs(stick.Throttle),0x3401);*/

//  stick.playEffect(2);
}

