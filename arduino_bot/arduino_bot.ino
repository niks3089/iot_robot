#include <NewPing.h> 
#include <SoftwareSerial.h>
#include "motor_bot.h"
#include "distance_sensor.h"
#include "moving_average.h"
#include "command_parser.h"
#include "arduino_bot.h"

#define TRIGGER_PIN  10 
#define ECHO_PIN     11
#define TOO_CLOSE 10
#define MAX_DISTANCE (TOO_CLOSE * 20)
#define RANDOM_ANALOG_PIN 5
#
#define ESP8266_RX   2
#define ESP8266_TX   3

/*
 * Arduino bot: Receives commands from NodeMCU (ESP8266)
 * Parses the command and uses AdaFruit Motor Shield Library to run the bot.
 * Also parses the signal input from the distance sensor.
 */

/* Read and write from nodemcu */
SoftwareSerial ESPserial(ESP8266_RX, ESP8266_TX); // RX | TX

namespace IoTBot {
class Robot {
public:
    /*
     * @brief Class constructor.
     */
    Robot()
        : motor(), 
          distanceSensor(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE),
          distanceAverage(TOO_CLOSE * 10)
    {
        initialize();
    }
    
    /*
     * @brief Initialize the robot state.
     */
    void initialize()
    {
        randomSeed(analogRead(RANDOM_ANALOG_PIN));
    }

    void run() = 0;

protected:
    MotorBot motor;
    DistanceSensor distanceSensor;
    MovingAverage<unsigned int, 3> distanceAverage;
    enum state_t { stateStopped, stateMoving, stateTurning };
    state_t state;
    unsigned long endStateTime;

    void move(int speed)
    {
        motor.move_forward(speed);
        state = stateMoving;
    }
    
    void stop()
    {
        motor.stop();
        state = stateStopped;
    }
    
    bool obstacleAhead(unsigned int distance)
    {
        return (distance <= TOO_CLOSE);
    }
    
    bool turn(int speed, unsigned long currentTime)
    {
        if (random(2) == 0) {
            motor.turn_left(speed);
        }
        else {
            motor.turn_right(speed);
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

};

class AutoBot : public Robot 
{
    void run()
    {
        unsigned long currentTime = millis();
        int distance = distanceAverage.add(distanceSensor.getDistance());
        int command = cmd_parser.get_command();
        int speed = cmd_parser.get_speed();
#if 0
        log("state: %d, currentTime: %lu, distance: %u command: %d\n", 
            state, currentTime, distance, command); 
#endif
        switch(command) {
        case COMMAND_START:
            if (stopped()) {
                move(speed);
            }
            break;
        case COMMAND_STOP:
            if (!stopped()) {
                stop();
            }
            break;
        default:
            /* Do nothing */
            break;
        }

        if (moving()) {
            if (obstacleAhead(distance))
                turn(speed, currentTime);
        }
        else if (turning()) {
            if (doneTurning(currentTime, distance))
                move();
        }
    }
};

class ManualBot : public Robot
{
    void run() {
        unsigned long currentTime = millis();
        int distance = distanceAverage.add(distanceSensor.getDistance());
        int command = cmd_parser.get_command();
#if 0
        log("state: %d, currentTime: %lu, distance: %u command: %d\n", 
            state, currentTime, distance, command); 
#endif
    }
};

};

IoTBot::CommandParser cmd_parser;
IoTBot::AutoBot autobot;
IoTBot::ManualBot manualbot;

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
    cmd_parser.parse_command();
    IoTBot::Robot *robot;

    if (cmd_parser.get_mode() == AUTO_MODE) {
        robot = &autobot;
    } else {
        robot = &manualbot;
    }
    robot->run();
}
