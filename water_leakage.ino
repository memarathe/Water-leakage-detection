#include <Arduino.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "secrets.h"
#include "ThingSpeak.h"
#define USE_SERIAL Serial

const char index_html[] PROGMEM={"<!DOCTYPE html>\n"
"<html>\n"
"<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
"<style>\n"
"body, html {\n"
"  height: 100%;\n"
"  margin: 0;\n"
"}\n"
"\n"
".bg {\n"
"  /* The image used */\n"
"  background-image: url(\"123.jpg\");\n"
"\n"
"  /* Full height */\n"
"  height: 100%; \n"
"\n"
"  /* Center and scale the image nicely */\n"
"  background-position: center;\n"
"  background-repeat: no-repeat;\n"
"  background-size: cover;\n"
"}\n"
"#map {\n"
"        height: 500px;  /* The height is 400 pixels */\n"
"        width: 100%;  /* The width is the width of the web page */\n"
"       }\n"
"       .blinking{\n"
"\tanimation:blinkingText 1.2s infinite;\n"
"}\n"
"@keyframes blinkingText{\n"
"\t0%{\t\tcolor: #000;\t}\n"
"\t49%{\tcolor: transparent;\t}\n"
"\t50%{\tcolor: transparent;\t}\n"
"\t99%{\tcolor:transparent;\t}\n"
"\t100%{\tcolor: #000;\t}\n"
"}\n"
"</style>\n"
"<head>\n"
"\t<title>Water Leakage Detection System</title>\n"
"</head>\n"
"<body>\n"
"<div class=\"bg\">\n"
"\t<br>\n"
"\t\n"
"\t<h1 style=\"text-align: center;font-size: 60px\"> Water Leakage Detection System</h1>\n"
"\n"
"<div id=\"map\"></div>\n"
"\n"
"    <script>\n"
"// Initialize and add the map15.820580, 74.498578\n"
"function initMap() {\n"
"  // The location of Uluru\n"
"  var Kle = {lat: 15.819849, lng: 74.498550};\n"
"  // The map, centered at Uluru\n"
"  var Rly = {lat: 15.819849, lng: 74.498550};\n"
"  var map = new google.maps.Map(\n"
"      document.getElementById('map'), {zoom: 18, center: Kle});\n"
"  // The marker, positioned at Uluru\n"
"  var marker = new google.maps.Marker({position: Kle, map: map});\n"
"\n"
"  var coffee = {lat: 15.819479, lng: 74.497479};\n"
"\n"
"  // The marker, positioned at Uluru\n"
"  var marker = new google.maps.Marker({position: coffee, map: map});\n"
"}\n"
"    </script>\n"
"    <!--Load the API from the specified URL\n"
"    * The async attribute allows the browser to render the page while the API loads\n"
"  \n"
"    * The callback parameter executes the initMap() function\n"
"    -->\n"
"    <script async defer\n"
"    src=\"https://maps.googleapis.com/maps/api/js?key=AIzaSyAK6ulWjN6uVPvkjVg5zlpPCYwu2egAETU&callback=initMap\">\n"
"    </script>\n"
"<div style=\"text-align: center;\">\n"
"\t<span style=\"font-size: 50px;\" class=\"blinking\"><b>Leakage Location Detected</b></span>\n"
"</div>\n"
"</div>\n"
"</body>\n"
"</html>"};

ESP8266WebServer server(81);
const  int sensor1 = 2;
const int sensor2 = 13;
float threshold=4.0;
char push_data [ 200 ];
int count1 = 0;
int count2 = 0;
float flowRate1  = 0.0;
float flowRate2  = 0.0;
int  flowMilliLitres1   = 0;
int  flowMilliLitres2   = 0;
int  oldTime  = 0;
char data[100];

String myStatus = "leakage detected";

const char* ssid = "<wifi ssid>";
const char* password = "<password of wifi>";
String deviceId = "v98876F8D66261F3";
const char* logServer = "api.pushingbox.com";



unsigned long myChannelNumber = <channelnumber>  Eg :1008459
const char * myWriteAPIKey = "<Your API KEY>"; Eg:UB9NZ3T4Z3XP4XMK

int c = 1;
void handleRoot() {
  server.send_P(200, "text/html", index_html);
}

void handleNotFound()
{
  //digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/html", message);
  //digitalWrite(led, 0);
}



void ICACHE_RAM_ATTR scount1();
void ICACHE_RAM_ATTR scount2();
void scount1()
{
  count1++;
}
void scount2()
{
  count2++;
}
WiFiClient Client;
void  setup () {
  //Serial. begin (115200);
  Serial.begin(74880);
  //WiFi.mode(WIFI_STA);
  ThingSpeak.begin(Client);
//  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    WiFi.begin(ssid, password);
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
        // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(500);  
    }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }
    //delay(10000);

    server.on("/", handleRoot);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });
   server.onNotFound(handleNotFound);
   server.begin();
    Serial.println("HTTP server started");


    attachInterrupt(digitalPinToInterrupt(sensor1), scount1, HIGH);
    attachInterrupt(digitalPinToInterrupt(sensor2), scount2, HIGH);
}

void sendNotification(String message){

  Serial.println("- connecting to Home Router SID: " + String(ssid));
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.println("- succesfully connected");
  Serial.println("- starting client");
  
  WiFiClient client;

  Serial.println("- connecting to pushing server: " + String(logServer));
  if (client.connect(logServer, 80)) {
    Serial.println("- succesfully connected");
    
    String postStr = "devid=";
    postStr += String(deviceId);
    postStr += "&message_parameter=";
    postStr += String(message);
    postStr += "\r\n\r\n";
    
    Serial.println("- sending data...");
    
    client.print("POST /pushingbox HTTP/1.1\n");
    client.print("Host: api.pushingbox.com\n");
    client.print("Connection: close\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
    delay(5000);
  }
  client.stop();
  Serial.println("- stopping the client");
}

void loop() {
  if (((millis() - oldTime) > 5000) || (c == 1))   // Only process counters once per 5 second
  {
    c = 0;
    detachInterrupt(sensor1);
    detachInterrupt(sensor2);
    flowRate1 = ((1000.0 / (millis() - oldTime)) * count1) / 4.5;
    flowMilliLitres1 = (flowRate1 / 60) * 1000;
    Serial.print("Flow rate 1: ");
    Serial.println(flowRate1);
    count1 = 0;
    flowRate2 = ((1000.0 / (millis() - oldTime)) * count2) / 4.5;
    oldTime = millis();
    flowMilliLitres2 = (flowRate2 / 60) * 1000;
    Serial.print("Flow rate 2: ");
    Serial.println(flowRate2);
    count2 = 0;
    if((flowRate1-flowRate2)>threshold )
    {
          Serial.println("LEAKAGE DETECTED");
          sendNotification("Hello World from ESP8266!");
          delay(5000);
          server.handleClient();
           MDNS.update();
            //delay(5000);
           delay(5000);
           ThingSpeak.setField(1, flowRate1);
           ThingSpeak.setField(2, flowRate2);
           ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
     }
           delay(1000); 
      }
      
      //Serial.println("Waiting for 5 seconds\n\n");
      attachInterrupt(digitalPinToInterrupt(sensor1), scount1, HIGH);
      attachInterrupt(digitalPinToInterrupt(sensor2), scount2, HIGH);
      
  }
