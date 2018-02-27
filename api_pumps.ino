#include "definitions.h"

void api_update_pump() {
  StaticJsonBuffer<1024> json_buffer;
  JsonObject& request_root = json_buffer.parseObject(server.arg("plain"));

  if (request_root == JsonObject::invalid() || !request_root.containsKey("pumpIndex")) {
    Serial.println("Invalid request");
    server.send(400, "application/json", "{ \"success\": false }");
    return;
  }

  int pump_index = request_root["pumpIndex"];
  int flow_rate = request_root.containsKey("flowRate") ? request_root["flowRate"] : 0;
  int ingredient_id = request_root.containsKey("ingredientId") ? request_root["ingredientId"] : 0;

  Serial.printf("Updating pump %d\n", pump_index);
  update_pump(pump_index, ingredient_id, flow_rate);
  server.send(200, "application/json", "{ \"success\": true }");
}

void api_list_pumps() {
  StaticJsonBuffer<1024> json_buffer;
  char buffer[1024];
  JsonObject& response_root = json_buffer.createObject();
  list_pumps(response_root);
  response_root.prettyPrintTo(buffer, 1024);
  Serial.println("Sending response");
  server.send(200, "application/json", buffer);
}

