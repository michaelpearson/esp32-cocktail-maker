#include "definitions.h"

void api_list_cocktails() {
    StaticJsonBuffer<1024> json_buffer;
    char buffer[1024];
    JsonObject& response_root = json_buffer.createObject();
    list_cocktails(response_root);
    response_root.prettyPrintTo(buffer, 1024);
    Serial.println("Sending response");
    server.send(200, "application/json", buffer);
}

void api_add_cocktail() {
  StaticJsonBuffer<1024> json_buffer;
  JsonObject& request_root = json_buffer.parseObject(server.arg("plain"));

  if (request_root == JsonObject::invalid() || !request_root.containsKey("name") || !request_root.containsKey("cocktailIngredients")) {
    Serial.println("Invalid request");
    server.send(400, "application/json", "{ \"success\": false }");
    return;
  }

  const char * cocktail_name = request_root["name"];

  JsonArray& cocktail_ingredients_array = request_root["cocktailIngredients"];
  if (cocktail_ingredients_array.size() > NUM_PUMPS) {
    Serial.println("Invalid request");
    server.send(400, "application/json", "{ \"success\": false, \"message\": \"Too many ingredients\" }");
    return;
  }

  CocktailIngredient ingredients[6];
  for (int a = 0; a < cocktail_ingredients_array.size(); a++) {
    JsonObject& ingredient = cocktail_ingredients_array.get<JsonObject>(a);
    if (!ingredient.containsKey("ingredientId") || !ingredient.containsKey("millilitres")) {
      Serial.println("Invalid request - missing ingredient information");
      server.send(400, "application/json", "{ \"success\": false, \"message\": \"Invalid ingredient\" }");
      return;
    }
    ingredients[a].ingredient_id = ingredient["ingredientId"];
    ingredients[a].millilitres = ingredient["millilitres"];
  }

  Serial.printf("Adding cocktail: %s\n", cocktail_name);
  add_cocktail(cocktail_name, ingredients, cocktail_ingredients_array.size());

  server.send(200, "application/json", "{ \"success\": true }");
}

void api_delete_cocktail() {
  StaticJsonBuffer<1024> json_buffer;
  JsonObject& request_root = json_buffer.parseObject(server.arg("plain"));

  if (request_root == JsonObject::invalid() || !request_root.containsKey("id")) {
    Serial.println("Invalid request");
    server.send(400, "application/json", "{ \"success\": false }");
    return;
  }

  int cocktail_id = request_root["id"];
  Serial.printf("Deleting cocktail: %d\n", cocktail_id);
  delete_cocktail(cocktail_id);

  server.send(200, "application/json", "{ \"success\": true }");
}
