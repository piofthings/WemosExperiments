#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Arduino.h>
#include "SSD1306.h"
#include "font.h"
#include "main.h"

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme;

float temperature, humidity, pressure, altitude;

/*Put your SSID & Password*/
const char *ssid = "TheLazyBlogger"; //enter your SSID here
const char *password = "";           //enter your network password here
const float BOXFACTOR = -5; // -5 deg C temperature reduction due to the fact the BME sensor is in the box

ESP8266WebServer server(80);
SSD1306 display(0x3C, D2, D1);

unsigned long delayTime;

void setup()
{
  Serial.begin(115200);
  delay(100);

  Serial.println(F("BME280 test"));

  unsigned status;

  Serial.println("-- Default Test --");
  delayTime = 1000;

  Serial.println();

  Serial.println("Connecting to ");
  Serial.println(ssid);

  display.init();
  display.flipScreenVertically();
  // comment the next line out or set your own font.
  display.setFont((uint8_t *)Monospaced_plain_12);
  // this is one big reason I like this library better than adafruit's. Adafruit's doesn't have an align method.
  display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);

  display.drawString(64, 32, "Connecting to Wifi:" + String(ssid));
  //connect to your local wi-fi network
  WiFi.begin(ssid, password);

  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");

  // display the server ip address on the screen upon wifi connection
  display.drawString(64, 32, "HTTP Server Started:\n" + WiFi.localIP().toString());
  // You can also pass in a Wire library object like &Wire2
  // status = bme.begin(0x76, &Wire2)
  // default settings

  status = bme.begin(0x76);
  delay(1587);
  if (!status)
  {
    Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
    Serial.print("SensorID was: 0x");
    Serial.println(bme.sensorID(), 16);
    Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");
    //while (1) delay(10);
  }
  bme.setSampling(Adafruit_BME280::MODE_FORCED,
                  Adafruit_BME280::SAMPLING_X8, // temperature
                  Adafruit_BME280::SAMPLING_X8, // pressure
                  Adafruit_BME280::SAMPLING_X8, // humidity
                  Adafruit_BME280::FILTER_OFF);
  delay(1587);

  display.display();
}

void loop()
{
  server.handleClient();
  updateValues();
  delay(1587);
}

void updateValues()
{
  bme.takeForcedMeasurement();
  temperature = bme.readTemperature() + BOXFACTOR;
  humidity = bme.readHumidity();
  pressure = bme.readPressure() / 100.0F;
  altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);

  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 0, "Temp:" + String(temperature) + " C");
  display.drawString(0, 14, "Humidity:" + String(humidity) + "%");
  display.drawString(0, 28, "Pressure:" + String(pressure) + "hPa");
  display.drawString(0, 42, "Altitude:" + String(altitude) + "m");
  display.display();
}

void handle_OnConnect()
{
  updateValues();
  server.send(200, "text/html", SendHTML(temperature, humidity, pressure, altitude));
}

void handle_NotFound()
{
  server.send(404, "text/plain", "Not found");
}

String SendHTML(float temperature, float humidity, float pressure, float altitude)
{
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>ESP8266 Weather Station</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr += "p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<div id=\"webpage\">\n";
  ptr += "<h1>ESP8266 Weather Station</h1>\n";
  ptr += "<p>Temperature: ";
  ptr += temperature;
  ptr += "&deg;C</p>";
  ptr += "<p>Humidity: ";
  ptr += humidity;
  ptr += "%</p>";
  ptr += "<p>Pressure: ";
  ptr += pressure;
  ptr += "hPa</p>";
  ptr += "<p>Altitude: ";
  ptr += altitude;
  ptr += "m</p>";
  ptr += "</div>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}