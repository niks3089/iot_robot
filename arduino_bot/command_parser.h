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
    char    cmd[100];
    int     cmd_code;
    int     mode;
    int     speed;
    double  direction;
    MovingAverage<unsigned int, 3> directionAverage;

    public:
    CommandParser() : 
       mode(AUTO_MODE),
       speed(250),
       direction(0.0),
       cmd_code(COMMAND_NONE) {}

    void parse_command()
    {
        String new_cmd, junk;
        // listen for communication from the ESP8266 and then write it to the serial monitor
        if (ESPserial.available()) {
            junk = ESPserial.readStringUntil('C');
            new_cmd = ESPserial.readStringUntil('\n');

            if (strcmp(cmd, new_cmd.c_str()) == 0) {
                return;
            }
            Serial.println(new_cmd);
            new_cmd.toCharArray(cmd, 100);
            if (strncmp(cmd, "auto", strlen("auto")) == 0) {
                mode = cmd_code = AUTO_MODE;
            } else if (strncmp(cmd, "man", strlen("man")) == 0) {
                mode = cmd_code = MANUAL_MODE;
            } else if (strncmp(cmd, "start", strlen("start")) == 0) {
                cmd_code = COMMAND_START; 
            } else if (strncmp(cmd, "stop", strlen("stop")) == 0) {
                cmd_code = COMMAND_STOP; 
            } else if (new_cmd.substring(0, strlen("speed")) == "speed") {
                cmd_code = COMMAND_SPEED;
                new_cmd.remove(0, strlen("speed") + 1);
                speed = new_cmd.toInt();
            } else if (new_cmd.substring(0, strlen("dir")) == "dir") {
                cmd_code = COMMAND_DIR; 
                new_cmd.remove(0, strlen("dir") + 1);
                direction = new_cmd.toFloat();
                Serial.println(direction);
            } else {
                Serial.println("Unknown command");
            }

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
