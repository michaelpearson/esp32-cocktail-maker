#include "definitions.h"

void api_list_ingredients() {
  StaticJsonBuffer<1024> json_buffer;
  char buffer[1024];
  JsonObject& response_root = json_buffer.createObject();
  list_ingredients(response_root);
  response_root.prettyPrintTo(buffer, 1024);
  Serial.println("Sending response");
  server.send(200, "application/json", buffer);
}

void api_add_ingredient() {
  StaticJsonBuffer<1024> json_buffer;
  JsonObject& request_root = json_buffer.parseObject(server.arg("plain"));

  if (request_root == JsonObject::invalid() || !request_root.containsKey("name")) {
    Serial.println("Invalid request");
    server.send(400, "application/json", "{ \"success\": false }");
    return;
  }

  const char * ingredient_name = request_root["name"];
  Serial.printf("Adding ingredient: %s\n", ingredient_name);
  add_ingredient(ingredient_name);

  server.send(200, "application/json", "{ \"success\": true }");
}

void api_delete_ingredient() {
  StaticJsonBuffer<1024> json_buffer;
  JsonObject& request_root = json_buffer.parseObject(server.arg("plain"));

  if (request_root == JsonObject::invalid() || !request_root.containsKey("id")) {
    Serial.println("Invalid request");
    server.send(400, "application/json", "{ \"success\": false }");
    return;
  }

  int ingredient_id = request_root["id"];
  Serial.printf("Deleting ingredient: %d\n", ingredient_id);
  delete_ingredient(ingredient_id);

  server.send(200, "application/json", "{ \"success\": true }");
}
