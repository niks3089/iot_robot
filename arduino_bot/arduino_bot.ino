#include <NewPing.h> 
#include <SoftwareSerial.h>
#include "motor_bot.h"
#include "distance_sensor.h"
#include "moving_average.h"

/* Pins definiations */
/*
#define TRIGGER_PIN   
#define ECHO_PIN

*/
#define ESP8266_RX   2
#define ESP8266_TX   3

/* Other constants */
#define TOO_CLOSE 10
#define MAX_DISTANCE (TOO_CLOSE * 20)

// Basic serial communication with ESP8266
// Uses serial monitor for communication with ESP8266
//
//  Pins
//  Arduino pin 2 (RX) to ESP8266 TX
//  Arduino pin 3 to voltage divider then to ESP8266 RX
//  Connect GND from the Arduiono to GND on the ESP8266
//  Pull ESP8266 CH_PD HIGH
//
// When a command is entered in to the serial monitor on the computer 
// the Arduino will relay it to the ESP8266
//
/*
 * Arduino bot: Receives commands from NodeMCU (ESP8266)
 * Parses the command and uses AdaFruit Motor Shield Library to run the bot.
 * Also parses the signal input from the distance sensor.
 */

/* Read and write from nodemcu */
SoftwareSerial ESPserial(ESP8266_RX, ESP8266_TX); // RX | TX

 
void setup() 
{
    Serial.begin(9600);     // communication with the host computer
    ESPserial.begin(9600);  
 
    Serial.println("");
    Serial.println("Ready");
    Serial.println("");    
}

void loop() 
{
    // listen for communication from the ESP8266 and then write it to the serial monitor
    if (ESPserial.available()) {
      String command = ESPserial.readStringUntil('\n');
      Serial.println(command);
    }
}

#if 0
namespace IoTBot {
  class Robot
    {
    public:
        /*
         * @brief Class constructor.
         */
        Robot()
            : MotorBot(),
              distanceSensor(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE),
              distanceAverage(TOO_CLOSE * 10),
              remoteControl(REMOTE_CONTROL_INIT)
        {
            initialize();
        }
        
        /*
         * @brief Initialize the robot state.
         */
        void initialize()
        {
            randomSeed(analogRead(RANDOM_ANALOG_PIN));
            remote();
        }
        
        /*
         * @brief Update the state of the robot based on input from sensor and remote control.
         *  Must be called repeatedly while the robot is in operation.
         */
        void run()
        {
            unsigned long currentTime = millis();
            int distance = distanceAverage.add(distanceSensor.getDistance());
            RemoteControlDriver::command_t remoteCmd;
            bool haveRemoteCmd = remoteControl.getRemoteCommand(remoteCmd);
            log("state: %d, currentTime: %lu, distance: %u remote: (%d,l:%d,r:%d,k:%d)\n", 
                state, currentTime, distance, 
                haveRemoteCmd, remoteCmd.left, remoteCmd.right, remoteCmd.key);
            
            if (remoteControlled()) {
                if (haveRemoteCmd) {
                    switch (remoteCmd.key) {
                    case RemoteControlDriver::command_t::keyF1:
                        // start "roomba" mode
                        move();
                        break;
                    case RemoteControlDriver::command_t::keyNone:
                        // this is a directional command
                        leftMotor.setSpeed(remoteCmd.left);
                        rightMotor.setSpeed(remoteCmd.right);
                        break;
                    default:
                        break;
                    }
                }
            }
            else {
                // "roomba" mode
                if (haveRemoteCmd && remoteCmd.key == RemoteControlDriver::command_t::keyF1) {
                    // switch back to remote mode
                    remote();
                }
                else {
                    if (moving()) {
                        if (obstacleAhead(distance))
                            turn(currentTime);
                    }
                    else if (turning()) {
                        if (doneTurning(currentTime, distance))
                            move();
                    }
                }
            }
        }

    protected:
        void remote()
        {
            leftMotor.setSpeed(0);
            rightMotor.setSpeed(0);
            state = stateRemote;
        }
        
        void move()
        {
            leftMotor.setSpeed(255);
            rightMotor.setSpeed(255);
            state = stateMoving;
        }
        
        void stop()
        {
            leftMotor.setSpeed(0);
            rightMotor.setSpeed(0);
            state = stateStopped;
        }
        
        bool obstacleAhead(unsigned int distance)
        {
            return (distance <= TOO_CLOSE);
        }
        
        bool turn(unsigned long currentTime)
        {
            if (random(2) == 0) {
                leftMotor.setSpeed(-255);
                rightMotor.setSpeed(255);
            }
            else {
                leftMotor.setSpeed(255);
                rightMotor.setSpeed(-255);
            }
            state = stateTurning;
            endStateTime = currentTime + random(500, 1000);
        }
        
        bool doneTurning(unsigned long currentTime, unsigned int distance)
        {
            if (currentTime >= endStateTime)
                return (distance > TOO_CLOSE);
            return false;
        }
        
        bool moving() { return (state == stateMoving); }
        bool turning() { return (state == stateTurning); }
        bool stopped() { return (state == stateStopped); }
        bool remoteControlled() { return (state == stateRemote); }

    private:
        Motor_Bot motor;
        DistanceSensor distanceSensor;
        MovingAverage<unsigned int, 3> distanceAverage;
        //RemoteControl remoteControl;
        enum state_t { stateStopped, stateMoving, stateTurning, stateRemote };
        state_t state;
        unsigned long endStateTime;
    };
};
#endif
