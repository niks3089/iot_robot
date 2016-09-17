#include <ESP8266WiFi.h>
 
const char* ssid = "nikhil_act";
const char* password = "pogothepug";
IPAddress ip(192,168,0,7);  //Node static IP
IPAddress gateway(192,168,0,1);
IPAddress subnet(255,255,255,0);
        
WiFiServer server(80);
WiFiClient client;
 
void setup() {
  Serial.begin(115200);
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
 
void loop() {
   if(!client.connected()) {
     client = server.available();
     return;
   }

  /* here we have a connected client */
  if(client.available()) {
    String command = client.readStringUntil('\n');
    Serial.println(command);
    /* Send the command to arduino */
  } 
}
