#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ArduinoJson.h>

#include <WebSocketsClient.h>
#include <SocketIOclient.h>

#include <Hash.h>

ESP8266WiFiMulti WiFiMulti;
SocketIOclient socketIO;

#define USE_SERIAL Serial

#define DEBUG true

#define ssid  "*****"               // your network's ssid
#define pwd  "*****"    // your network's pwd 

#define host "192.168.0.5" // your backend host
#define port 3000                // your backend port

void setup() {
      startSocket();
}


void loop() {
      updateSocket();
}

void on_message_recieved(String event , DynamicJsonDocument payload) {
      String message;

      
      USE_SERIAL.print(event);
      USE_SERIAL.print("  -  ");

      message = "";
      serializeJson(payload, message);
      USE_SERIAL.println(message);
      
      
      if (event == "off") {
            socket_emit("[\"state\", {\"recived\": \"off\"}]");

      } else if (event == "on") {
            socket_emit("[\"state\", {\"recived\": \"on\"}]");

      } else {
            message = "[\"state\", {\"recived\": \"";
            message += event;
            message += "\"}]";
            socket_emit(message);
      }
}
