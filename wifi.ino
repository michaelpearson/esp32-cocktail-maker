#include "definitions.h"

const char * ssid = "702";
const char * password = "bu11sh!t";

void setup_wifi() {
  Serial.println("Wifi setup");
  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.printf("\nConnected. IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Setting up routes");

  server.on("/api/ingredients.json", HTTP_GET, api_list_ingredients);
  server.on("/api/ingredients.json", HTTP_POST, api_add_ingredient);
  server.on("/api/ingredients.json", HTTP_DELETE, api_delete_ingredient);
  
  server.on("/api/cocktails.json", HTTP_GET, api_list_cocktails);
  server.on("/api/cocktails.json", HTTP_POST, api_add_cocktail);
  server.on("/api/cocktails.json", HTTP_DELETE, api_delete_cocktail);

  server.on("/api/pumps.json", HTTP_POST, api_update_pump);
  server.on("/api/pumps.json", HTTP_GET, api_list_pumps);
  
  Serial.println("Starting server");
  server.begin();
}


void wifi_loop() {
  server.handleClient();
}

