#include <FFPJoystick.h>


FFPJoystick stick;

void setup()
{

//  stick.EnableAutoCenter();
stick.DisableAutoCenter();
stick.CreateConstantForce(20,0);
 Serial.begin(115200);
}
void loop()
{
  stick.Poll();
//  Serial.println(stick.Throttle);
//  stick.forceTest(60);
  delay(50);
  if(stick.Throttle > 0)
    stick.updateConstantForce(abs(stick.Throttle),0);
  else
    stick.updateConstantForce(abs(stick.Throttle),0x3401);
//  stick.playEffect(2);
}

