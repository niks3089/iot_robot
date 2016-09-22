namespace IoTBot {

class Robot {
public:
    Robot()
        : motor(), 
          distanceSensor(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE),
          distanceAverage(TOO_CLOSE * 10)
    {
        initialize();
    }
    
    void initialize()
    {
        randomSeed(analogRead(RANDOM_ANALOG_PIN));
    }

    virtual void run() = 0;

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
public:
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
                move(speed);
        }
    }
};

class ManualBot : public Robot
{
public:
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
