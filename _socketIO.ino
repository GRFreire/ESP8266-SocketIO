void socket_emit(String _payload) {
      // Send event
      socketIO.sendEVENT(_payload);
}

void socket_on(uint8_t * _payload) {
      
      DynamicJsonDocument payload(1024);
      deserializeJson(payload, _payload);

      String _event = payload[0];

      String _content;
      serializeJson(payload[1], _content);

      DynamicJsonDocument content(1024);
      deserializeJson(content, _content);
      
      
      on_message_recieved(_event, content);

}


void socketIOEvent(socketIOmessageType_t type, uint8_t * payload, size_t length) {

      switch (type) {
            case sIOtype_DISCONNECT:
                  USE_SERIAL.printf("[IOc] Disconnected!\n");
                  break;
            case sIOtype_CONNECT:
                  USE_SERIAL.printf("[IOc] Connected to url: %s\n", payload);
                  break;
            case sIOtype_EVENT:
                  USE_SERIAL.printf("[IOc] get event: %s\n", payload);
                  socket_on(payload);
                  break;
            case sIOtype_ACK:
                  USE_SERIAL.printf("[IOc] get ack: %u\n", length);
                  hexdump(payload, length);
                  break;
            case sIOtype_ERROR:
                  USE_SERIAL.printf("[IOc] get error: %u\n", length);
                  hexdump(payload, length);
                  break;
            case sIOtype_BINARY_EVENT:
                  USE_SERIAL.printf("[IOc] get binary: %u\n", length);
                  hexdump(payload, length);
                  break;
            case sIOtype_BINARY_ACK:
                  USE_SERIAL.printf("[IOc] get binary ack: %u\n", length);
                  hexdump(payload, length);
                  break;
      }
}

void startSocket() {

      USE_SERIAL.begin(115200);


      USE_SERIAL.setDebugOutput(DEBUG);

      USE_SERIAL.println();
      USE_SERIAL.println();
      USE_SERIAL.println();

      for (uint8_t t = 4; t > 0; t--) {
            USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
            USE_SERIAL.flush();
            delay(1000);
      }

      // disable AP
      if (WiFi.getMode() & WIFI_AP) {
            WiFi.softAPdisconnect(true);
      }

      WiFiMulti.addAP(ssid, pwd);

      //WiFi.disconnect();
      while (WiFiMulti.run() != WL_CONNECTED) {
            delay(100);
      }

      String ip = WiFi.localIP().toString();
      USE_SERIAL.printf("[SETUP] WiFi Connected %s\n", ip.c_str());

      // server address, port and URL
      socketIO.begin(host, port);

      // event handler
      socketIO.onEvent(socketIOEvent);
}

void updateSocket() {
      socketIO.loop();
}
