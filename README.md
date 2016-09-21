# iot_robot

App controlled Wifi Robot car

Hardware Required

Software Required

Building the Bot:

Connection between Arduino and NodeMCU

For a bi-directional communication between Arduino and NodeMCU, we need to use a logic level converter. However if the communication is unidirectional and from NodeMCU to Arduino, we don't need to use the logic level converter since the 3.3V from NodeMCU will be treated as high in Arduino and pins will not get afftected. 

So the connection is really simple. Connect the tx of NodeMCU directly to the rx of Arduino. If you need to use the rx/tx to debug on your computer, use SoftwareSerial and use other available pins as tx/rx

Remote control

In order to cotrol the bot, we use a remote control which connects to the bot over the WiFi. NodeMCU will have a TCP server running and we need to connect to it in order to pass the commands. 

Instead of writing my own TCP client APP, I just used this very useful Andriod app RoboRemo. This allowed me to customize the remote control based on my needs and I used it connect to my Bot.
