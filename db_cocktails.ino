#include "definitions.h"

File cocktail_database_file;
EDB cocktail_database(&cocktail_writer, &cocktail_reader);

void setup_cocktails_table() {
  setup_table<Cocktail>("/db/cocktails.db", "Cocktails", cocktail_database, &cocktail_database_file);
}

void cocktail_writer(unsigned long address, const byte* data, unsigned int recsize) {
  cocktail_database_file.seek(address, SeekSet);
  cocktail_database_file.write(data, recsize);
  cocktail_database_file.flush();
}

void cocktail_reader(unsigned long address, byte* data, unsigned int recsize) {
  cocktail_database_file.seek(address, SeekSet);
  cocktail_database_file.read(data, recsize);
}


void list_cocktails(JsonObject& response_root) {
  Serial.println("Listing cocktails");
  Cocktail cocktail_record;

  JsonArray& cocktails = response_root.createNestedArray("cocktails");

  for (int recno = 1; recno <= cocktail_database.count(); recno++) {
    EDB_Status result = cocktail_database.readRec(recno, EDB_REC cocktail_record);
    if (result == EDB_OK) {
      Serial.printf("adding cocktail '%s' to response\n", cocktail_record.name);
      JsonObject& cocktail = cocktails.createNestedObject();
      cocktail["name"] = cocktail_record.name;
      cocktail["id"] = cocktail_record.id;
      JsonArray& ingredients = cocktail.createNestedArray("ingredients");
      for (int a = 0;a < NUM_PUMPS; a++) {
        if (!cocktail_record.ingredients[a].ingredient_id) {
          break;
        }
        JsonObject& ingredient = ingredients.createNestedObject();
        ingredient["ingredient_id"] = cocktail_record.ingredients[a].ingredient_id;
        ingredient["millilitres"] = cocktail_record.ingredients[a].millilitres;
      }
    } else {
      printError(result);
    }
  }
}

void delete_cocktail(int id) {
  Serial.printf("Deleting cocktail %d\n", id);
  delete_record<Cocktail>(cocktail_database, id);
}

void add_cocktail(const char * cocktail_name, CocktailIngredient * ingredients, int ingredient_count) {
  Cocktail cocktail;
  strcpy(cocktail.name, cocktail_name);
  memset(cocktail.ingredients, 0, sizeof(cocktail.ingredients));
  memcpy(cocktail.ingredients, ingredients, ingredient_count * sizeof(CocktailIngredient));
  add_record<Cocktail>(cocktail_database, &cocktail);
}
