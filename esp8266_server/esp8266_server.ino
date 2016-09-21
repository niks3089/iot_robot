#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
 
const char* ssid = "nikhil_act";
const char* password = "pogothepug";
IPAddress ip(192,168,0,7);  //Node static IP
IPAddress gateway(192,168,0,1);
IPAddress subnet(255,255,255,0);
        
WiFiServer server(80);
WiFiClient client;

#define Arduino_RX 4 // D2
#define Arduino_TX 5 // D1
SoftwareSerial ArduinoSerial(Arduino_RX, Arduino_TX); // RX | TX
 
void setup() {
  Serial.begin(115200);
  ArduinoSerial.begin(9600);
  delay(10);
 
  // Connect to WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
  WiFi.config(ip, gateway, subnet);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started"); 
}
 
void loop() 
{
  client = server.available();
  if (client) {
    Serial.println("new client");
    while (client.connected()) {
      if (client.available()) {
        String command = client.readStringUntil('\n');
        /* Send the command to arduino */
        ArduinoSerial.println(command);
        Serial.println(command);
      }
    }
    // give the web browser time to receive the data
    delay(1);
    
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}
