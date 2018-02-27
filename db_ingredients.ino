#include "definitions.h"

File ingredient_database_file;
EDB ingredient_database(&ingredient_writer, &ingredient_reader);

void setup_ingredients_table() {
  setup_table<Ingredient>("/db/ingredients.db", "Ingredients", ingredient_database, &ingredient_database_file);
}

void ingredient_writer(unsigned long address, const byte* data, unsigned int recsize) {
  ingredient_database_file.seek(address, SeekSet);
  ingredient_database_file.write(data, recsize);
  ingredient_database_file.flush();
}

void ingredient_reader(unsigned long address, byte* data, unsigned int recsize) {
  ingredient_database_file.seek(address, SeekSet);
  ingredient_database_file.read(data, recsize);
}


void list_ingredients(JsonObject& response_root) {
  Serial.println("Listing ingredients");
  Ingredient ingredient;

  JsonArray& ingredient_list = response_root.createNestedArray("ingredients");

  for (int recno = 1; recno <= ingredient_database.count(); recno++) {
    EDB_Status result = ingredient_database.readRec(recno, EDB_REC ingredient);
    if (result == EDB_OK) {
      Serial.printf("adding ingredient '%s' to response\n", ingredient.name);
      JsonObject& ingredient_object = ingredient_list.createNestedObject();
      ingredient_object["name"] = ingredient.name;
      ingredient_object["id"] = ingredient.id;
    } else {
      printError(result);
    }
  }
}

void delete_ingredient(int id) {
  Serial.printf("Deleting ingredient %d\n", id);
  delete_record<Ingredient>(ingredient_database, id);
}

void add_ingredient(const char * ingredient_name) {
  Ingredient i;
  strcpy(i.name, ingredient_name);
  add_record<Ingredient>(ingredient_database, &i);
}
