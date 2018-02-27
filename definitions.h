#ifndef definitions_h
#define definitions_h

#define NUM_PUMPS 6

#include <ArduinoJson.h>
#include <EDB.h>
#include "FS.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

struct Ingredient {
  int id;
  char name[64];
};

struct CocktailIngredient {
  int ingredient_id;
  int millilitres;
};

struct Cocktail {
  int id;
  char name[64];
  CocktailIngredient ingredients[6];
};

struct Pump {
  int ingredient_id;
  int flow_rate;
};

#endif
