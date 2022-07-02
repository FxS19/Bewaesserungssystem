namespace WebServer {
  // Create AsyncWebServer object on port 80
  AsyncWebServer server(80);

  void setup() {
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/index.html", "text/html");
    });
    server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request){
      digitalWrite(LED_BUILTIN, LOW); 
      request->send(SPIFFS, "/index.html", "text/html");
    });
    server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request){
      digitalWrite(LED_BUILTIN, HIGH); 
      request->send(SPIFFS, "/index.html", "text/html");
    });

    server.begin();
  }
}
