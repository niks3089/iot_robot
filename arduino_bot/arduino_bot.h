namespace IoTBot {

class Robot {
public:
    Robot()
        : motor(), 
          distanceSensor(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE),
          distanceAverage(TOO_CLOSE * 10),
          state(stateStopped)
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

    bool set_speed(int speed) 
    {
        motor.set_speed(speed);
    }
    
    bool turn_left(int speed) 
    {
        motor.turn_left(speed);
    }

    bool turn_right(int speed)
    {
        motor.turn_right(speed);
    }
    
    bool turn(int speed, unsigned long currentTime)
    {
        if (random(2) == 0) {
            turn_left(speed);
        }
        else {
            turn_right(speed);
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

    void handle_command(int command, int speed)
    {
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
        case COMMAND_SPEED:
            if (!stopped()) {
                set_speed(speed);
            }         
            break;
        default:
            /* Do nothing */
            break;
        }
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

        handle_command(command, speed);
        
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
        int speed = cmd_parser.get_speed();
        double direction = cmd_parser.get_direction();
#if 0
        log("state: %d, currentTime: %lu, distance: %u command: %d\n", 
            state, currentTime, distance, command); 
#endif
        handle_command(command, speed);

        if (!stopped()) {
            move_in_direction(speed, direction);
        }
    }

private:
    void move_in_direction(int speed, double direction) {
        if (direction > 3) {
            /* Turn right */
            turn_right(speed);
        } else if (direction < -3) {
            /* Turn left */
            turn_left(speed);
        } else {
            move(speed);
        }
    }
};

};
