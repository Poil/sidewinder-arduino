#include <FFPJoystick.h>


FFPJoystick stick;
uint8_t magnitude = 20;
uint8_t vibration;

/*
 * Trainer PPM Interface
 * -------------------
 *
 *
 * Created 31 October 2007
 * copyleft 2007 Mathieu Glachant 
 * mathieu@ban-sidhe.com
 * http://ban-sidhe.com/
 *
 * adapted from todbot's Serial-Servo-Better
 */

int servoPin = 6;           // Control pin for trainer interface

                             // A pulse starts with a low signal of fixed width (0.3ms), 
                             // followed by a high signal for the remainder of the pulse.
                             // Total pulse width is proportional to servo position (1 to 2ms)
int pulseStart = 300;        // pulse start width in microseconds
int pulseMin = 724;          // pulse minimum width minus start in microseconds
int pulseMax = 2048;         // pulse maximum width in microseconds
int pulseMid = 1500;
int conversionFactor = 5.7;   // (pulseMax - pulseMin - pulseStart)/180

                             // A frame is a succession of pulses, in order of channels,
                             // followed by a synchronisation pulse to fill out the frame.
                             // A frame's total length is fixed (20ms)
int frameLength = 20;        // The duration in millisecs of a frame

long lastFrame = 0;          // The time in millisecs of the last frame
int channelNumber = 4;       // Number of channels to send (keep below frameLength/pulseMax)
int servo[4];                // Values to set for the servos in degrees
int channel[4];              // Values to send on channels (duration of pulse minus start, in microseconds)
int i;                       // Counter in for loop
int j = 0;                   // Counter for servo updates



void setup()
{
  pinMode(servoPin, OUTPUT);  // Set servo pin as an output pin
  Serial.begin(115200);         // connect to the serial port
  for ( i = 0; i < channelNumber; i = i + 1 ) {servo[i] = 0;}
  for ( i = 0; i < channelNumber; i = i + 1 ) {channel[i] = 1500;}
Serial.println("Trainer_PPM_Interface ready");


  stick.EnableAutoCenter();
//stick.DisableAutoCenter();
// Serial.begin(115200);
// stick.CreateConstantForce(0,0);
}
void loop()
{
  
      // Save the time of frame start
  lastFrame = millis();

    // This for loop generates the pulse train, one per channel  
  for ( i = 0; i < channelNumber; i = i + 1 ) {
    digitalWrite(servoPin, LOW);   // Initiate pulse start
    delayMicroseconds(pulseStart); // Duration of pulse start
    digitalWrite(servoPin, HIGH);  // Stop pulse start
    delayMicroseconds(channel[i]); // Finish off pulse
  }
    digitalWrite(servoPin, LOW);   // Initiate synchronisation pulse
    delayMicroseconds(pulseStart); // Duration of start of synchronisation pulse
    digitalWrite(servoPin, HIGH);  // Stop synchronisation pulse start

    // We're done generating pulses and using delayMicroseconds()
    // Time to do some other processing before the next frame
    // How much time depends on how many channels you are running
    
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
  /*
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

channel[0] = pulseMid + stick.Throttle*4.0;
channel[1] = pulseMid + stick.Throttle*4.0;


// Let's change the servo positions
    j=j+1;
    if (j==4) {j=0;}
    
    if  (j==0) {
      for ( i = 0; i < channelNumber; i = i + 1 ) {
//        servo[i] = servo[i]+1;
          servo[i] = (126 - (stick.Throttle + 63));
        if (servo[i] >= 360) {servo[i]=0;}
      }
    }
    
Serial.print(servo[0]);
Serial.print(" ");
Serial.println(servo[1]);    
    
    
        
    // Calculate pulse durations from servo positions                                   
  for ( i = 0; i < channelNumber; i = i + 1 ) {
    channel[i] = abs(servo[i]-180); 
    channel[i] = int(channel[i]*conversionFactor)+pulseMin; 
  } 

  
  // We're ready to wait for the next frame
  // Some jitter is allowed, so to the closest ms
  while (millis() - lastFrame < frameLength) {  
    delay(1);
}
}
