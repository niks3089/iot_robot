#include <AFMotor.h>
namespace IoTBot
{
	
enum {
  FRONT_RIGHT_MOTOR = 1,
  FRONT_LEFT_MOTOR,
  BACK_LEFT_MOTOR,
  BACK_RIGHT_MOTOR,
} dc_motors;
 
class MotorBot {
  private:
    AF_DCMotor front_right_motor;
    AF_DCMotor front_left_motor;
    AF_DCMotor back_left_motor;
    AF_DCMotor back_right_motor;
    
  public:
    MotorBot() :
      front_right_motor(FRONT_RIGHT_MOTOR),
      front_left_motor(FRONT_LEFT_MOTOR),
      back_right_motor(BACK_RIGHT_MOTOR),
      back_left_motor(BACK_LEFT_MOTOR) {}

   void set_speed(int speed)
   {
      front_right_motor.setSpeed(speed);
      front_left_motor.setSpeed(speed);
      back_right_motor.setSpeed(speed);
      back_left_motor.setSpeed(speed);
   }

   void turn_left(int speed)
   {
      set_speed(speed);
    
   }

   void turn_right(int speed)
   {
      set_speed(speed);
   }

   void move_forward(int speed)
   {
      set_speed(speed);
      front_right_motor.run(FORWARD);
      front_left_motor.run(FORWARD);
      back_right_motor.run(FORWARD);
      back_left_motor.run(FORWARD);          
   }

   void stop()
   {
      front_right_motor.run(RELEASE);
      front_left_motor.run(RELEASE);
      back_right_motor.run(RELEASE);
      back_left_motor.run(RELEASE);    
   }
};

};
