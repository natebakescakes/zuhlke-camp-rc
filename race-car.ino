#include <HTTP_Method.h>
#include <WebServer.h>
#include "FS.h"
#include "SPIFFS.h"
#include <WiFi.h>

#define ledChannel 0
#define leftForwardChannel 1
#define rightForwardChannel 2
#define leftReverseChannel 3
#define rightReverseChannel 4

const int frequency = 5000;
const int resolution = 8;

const int RIGHT_FORWARD_PIN = 23;
const int LEFT_FORWARD_PIN = 33;
const int RIGHT_REVERSE_PIN = 22;
const int LEFT_REVERSE_PIN = 32;
const int DELAY_MS = 1000;

const char *ssid = "jawen-nathan-rc";
const char *password = "password";

WebServer server(80);
IPAddress myIP;

void wifiSetup()
{
  WiFi.softAP(ssid, password);
  myIP = WiFi.softAPIP();
}

void webServerSetup()
{
  server.on("/", serveHtml);
  server.on("/drive", moveForward);
  server.on("/reverse", moveBackward);
  server.on("/left", moveLeft);
  server.on("/right", moveRight);
  server.on("/stop", stop);

  server.begin();
}

String staticHtml()
{
  String html = "<!DOCTYPE html> <html>\n";
  html += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  html += "<title>LED Control</title>\n";
  html += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  html += ".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  html += ".button-on {background-color: #3498db;}\n";
  html += ".button-on:active {background-color: #2980b9;}\n";
  html += ".button-off {background-color: #34495e;}\n";
  html += ".button-off:active {background-color: #2c3e50;}\n";
  html += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  html += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  html += "</style>\n";
  html += "</head>\n";
  html += "<body>\n";
  html += "<a class=\"button button-on\" href=\"/left\">LEFT</a>\n";
  html += "<a class=\"button button-on\" href=\"/right\">RIGHT</a>\n";
  html += "<a class=\"button button-on\" href=\"/reverse\">REVERSE</a>\n";
  html += "<a class=\"button button-on\" href=\"/drive\">DRIVE</a>\n";
  html += "<a class=\"button button-on\" href=\"/stop\">STOP</a>\n";
  html += "</body>\n";
  html += "</html>\n";
  return html;
}

void serveHtml()
{
  // File file = SPIFFS.open("/index.html", FILE_READ);
  // server.streamFile(file, "text/html");
  // Serial.write(file);
  // file.close();
  server.send(200, "text/html", staticHtml());
}

void pinSetup()
{
  ledcSetup(ledChannel, frequency, resolution);
  ledcAttachPin(LED_BUILTIN, ledChannel);
  ledcSetup(rightReverseChannel, frequency, resolution);
  ledcAttachPin(RIGHT_REVERSE_PIN, rightReverseChannel);
  ledcSetup(leftReverseChannel, frequency, resolution);
  ledcAttachPin(LEFT_REVERSE_PIN, leftReverseChannel);
  ledcSetup(rightForwardChannel, frequency, resolution);
  ledcAttachPin(RIGHT_FORWARD_PIN, rightForwardChannel);
  ledcSetup(leftForwardChannel, frequency, resolution);
  ledcAttachPin(LEFT_FORWARD_PIN, leftForwardChannel);
}

void setup()
{
  Serial.begin(115200);
  wifiSetup();
  webServerSetup();
  pinSetup();
}

void moveForward()
{
  ledcWrite(rightReverseChannel, 0);
  ledcWrite(leftReverseChannel, 0);
  ledcWrite(rightForwardChannel, 255);
  ledcWrite(leftForwardChannel, 255);
  server.send(200, "text/html", staticHtml());
}

void moveBackward()
{
  ledcWrite(rightForwardChannel, 0);
  ledcWrite(leftForwardChannel, 0);
  ledcWrite(rightReverseChannel, 180);
  ledcWrite(leftReverseChannel, 180);
  server.send(200, "text/html", staticHtml());
}

void moveLeft()
{
  ledcWrite(rightForwardChannel, 180);
  ledcWrite(rightReverseChannel, 0);
  ledcWrite(leftForwardChannel, 0);
  ledcWrite(leftReverseChannel, 150);
  server.send(200, "text/html", staticHtml());
}

void moveRight()
{
  ledcWrite(leftForwardChannel, 180);
  ledcWrite(leftReverseChannel, 0);
  ledcWrite(rightForwardChannel, 0);
  ledcWrite(rightReverseChannel, 150);
  server.send(200, "text/html", staticHtml());
}

void stop()
{
  ledcWrite(leftForwardChannel, 0);
  ledcWrite(leftReverseChannel, 0);
  ledcWrite(rightForwardChannel, 0);
  ledcWrite(rightReverseChannel, 0);
  server.send(200, "text/html", staticHtml());
}

void loop()
{
  server.handleClient();
  ledcWrite(ledChannel, 255);
}
