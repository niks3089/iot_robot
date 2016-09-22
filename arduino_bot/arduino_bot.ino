/*
 * Arduino bot: Receives commands from NodeMCU (ESP8266)
 * Parses the command and uses AdaFruit Motor Shield Library to run the bot.
 * Also parses the signal input from the distance sensor.
 */
#include <NewPing.h> 
#include <SoftwareSerial.h>
#include "motor_bot.h"
#include "distance_sensor.h"
#include "moving_average.h"

#define TRIGGER_PIN  10 
#define ECHO_PIN     11
#define TOO_CLOSE 10
#define MAX_DISTANCE (TOO_CLOSE * 20)
#define RANDOM_ANALOG_PIN 5
#define ESP8266_RX   2
#define ESP8266_TX   3

/* Read and write from nodemcu */
SoftwareSerial ESPserial(ESP8266_RX, ESP8266_TX); // RX | TX
#include "command_parser.h"
IoTBot::CommandParser cmd_parser;
#include "arduino_bot.h"

IoTBot::AutoBot autobot;
IoTBot::ManualBot manualbot;

void setup() 
{
    Serial.begin(9600);     // communication with the host computer
    ESPserial.begin(9600);  
 
    Serial.println("Ready");
}

void loop() 
{
    cmd_parser.parse_command();
    IoTBot::Robot *robot;

    if (cmd_parser.get_mode() == IoTBot::AUTO_MODE) {
        robot = &autobot;
    } else {
        robot = &manualbot;
    }
    robot->run();
}
