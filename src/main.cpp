#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include <Arduino.h>
#include "SSD1306.h"
#include "font.h"
#include "main.h"

const char* ssid = "TheLazyBlogger"; //enter your SSID here
const char* password = ""; //enter your network password here

ESP8266WebServer server(80);

const int led = 13;

SSD1306  display(0x3C, D2, D3); // Yep, we can use SCL in this library too. Someone tell me the difference?

// this function is what gets called if you enter the base ip address of your Wemos in the browser
void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/plain", "hello from esp8266!");
  digitalWrite(led, 0);
}

// this gets called if no function is for your address
void handleNotFound() {
  digitalWrite(led, 1);
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
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/update", handleUpdateCall);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();

  display.init();
  display.flipScreenVertically();
  // comment the next line out or set your own font.
  display.setFont((uint8_t *) Monospaced_plain_12);
  // this is one big reason I like this library better than adafruit's. Adafruit's doesn't have an align method.
  display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);

  // display the server ip address on the screen upon wifi connection
  display.drawString(display.getWidth()/2, display.getHeight()/2, "HTTP Server Started:\n" + WiFi.localIP().toString());
  display.display();
}

void loop(void) {
  server.handleClient();
}

// this gets called if you enter http://yourwemosaddress/update?message=SomeMessageText in a web browser
void handleUpdateCall() { 

  String message = "";
  
  //Message parameter wasn't found  
  if (server.arg("message")== ""){     
  
    message = "Message Argument not found";
  
  }else{
    // message was found, let's display it on the OLED
    message = "";
    message += server.arg("message");     //Gets the value of the query parameter
    display.clear();
    display.drawString(display.getWidth()/2, display.getHeight()/2, message);
    display.display();
  
  }
  
  server.send(200, "text/plain", "OK");          //Returns the HTTP response
}