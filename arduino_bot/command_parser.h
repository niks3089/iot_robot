namespace IoTBot {
typedef enum {
    COMMAND_NONE = 0,
    COMMAND_START,
    COMMAND_STOP,
    COMMAND_SPEED,
    COMMAND_DIR
} commands;

typedef enum {
    AUTO_MODE = 1,
    MANUAL_MODE
} modes;

class CommandParser {
    private:
    String  cmd;
    int     cmd_code;
    int     mode;
    int     speed;
    double  direction;
    MovingAverage<unsigned int, 3> directionAverage;

    public:
    CommandParser() : 
       mode(AUTO_MODE),
       speed(255),
       direction(0.0),
       cmd_code(COMMAND_NONE) {}

    void parse_command()
    {
        String tmp;
        // listen for communication from the ESP8266 and then write it to the serial monitor
        if (ESPserial.available()) {
            cmd = ESPserial.readStringUntil('\n');
            Serial.println(cmd);
        }
        if (cmd == "Cauto") {
            mode = AUTO_MODE;
        } else if (cmd == "Cman") {
            mode = MANUAL_MODE;
        } else if (cmd == "Cstart") {
            cmd_code = COMMAND_START; 
        } else if (cmd == "Cstop") {
            cmd_code = COMMAND_STOP; 
        } else if (cmd.substring(0, strlen("Cspeed")) == "Cspeed") {
            cmd_code = COMMAND_SPEED; 
            tmp = cmd;
            tmp.remove(0, strlen("Cspeed") + 1);
            speed = tmp.toInt();
            Serial.println(speed);
        } else if (cmd.substring(0, strlen("Cdir")) == "Cdir") {
            cmd_code = COMMAND_DIR; 
            tmp = cmd;
            tmp.remove(0, strlen("Cdir") + 1);
            direction = tmp.toFloat();
            Serial.println(direction);
        }
    }

    int get_mode()
    {
        return mode;
    }

    int get_command() 
    {
        return cmd_code;
    }

    int get_speed()
    {
        return speed;
    }
    
    int get_direction() 
    {
        return direction;
    }
};
}
