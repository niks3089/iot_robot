#include <ESP8266WiFi.h>
 
const char* ssid = "nikhil_act";
const char* password = "aprakash105";
IPAddress ip(192,168,0,7);  //Node static IP
IPAddress gateway(192,168,0,1);
IPAddress subnet(255,255,255,0);

int ledPin = 4; 
byte mac[6];                     // the MAC address of your Wifi shield
WiFiServer server(80);
char cmd[100]; // stores the command chars received from RoboRemo
int cmdIndex;
WiFiClient client;
 
void setup() {
  Serial.begin(115200);
  delay(10);
 
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
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
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  WiFi.macAddress(mac);
  Serial.print("MAC: ");
  Serial.print(mac[5],HEX);
  Serial.print(":");
  Serial.print(mac[4],HEX);
  Serial.print(":");
  Serial.print(mac[3],HEX);
  Serial.print(":");
  Serial.print(mac[2],HEX);
  Serial.print(":");
  Serial.print(mac[1],HEX);
  Serial.print(":");
  Serial.println(mac[0],HEX);
 
}
 
void loop() {
   if(!client.connected()) {
    client = server.available();
    digitalWrite(ledPin, LOW);
    return;
  }

  digitalWrite(ledPin, HIGH);

  // here we have a connected client

  if(client.available()) {
    char c = (char)client.read(); // read char from client (RoboRemo app)

    if(c=='\n') { // if it is command ending
      cmd[cmdIndex] = 0;
      cmdIndex = 0; // reset the cmdIndex
      Serial.println(cmd);
    } else {      
      cmd[cmdIndex] = c; // add to the cmd buffer
      if(cmdIndex<99) cmdIndex++;
    }
  } 
  /* 
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
 
  // Read the first line of the request
  char c = (char)client.read(); // read char from client (RoboRemo app)

    if(c=='\n') { // if it is command ending
      cmd[cmdIndex] = 0;  // execute the command
      cmdIndex = 0; // reset the cmdIndex
      Serial.println(cmd);
   } else {      
      cmd[cmdIndex] = c; // add to the cmd buffer
      if(cmdIndex<99) cmdIndex++;
      Serial.print(c);
   }
   */
  
/* 
  String request = client.readStringUntil('\n');
  Serial.println(request);
  Serial.flush();
  client.flush();

  int value = LOW;
  if (request.indexOf("/LED=ON") != -1)  {
    digitalWrite(ledPin, HIGH);
    value = HIGH;
  }
  if (request.indexOf("/LED=OFF") != -1)  {
    digitalWrite(ledPin, LOW);
    value = LOW;
  }


   // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
 
  client.print("Led pin is now: ");
 
  if(value == HIGH) {
    client.print("On");
  } else {
    client.print("Off");
  }
  client.println("<br><br>");
  client.println("<a href=\"/LED=ON\"\"><button>Turn On </button></a>");
  client.println("<a href=\"/LED=OFF\"\"><button>Turn Off </button></a><br />");  
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
  */
 
}
