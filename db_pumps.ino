#include "definitions.h"
#define FILE_SIZE (NUM_PUMPS * sizeof(Pump))

File pump_file;
const char * pump_file_name = "/db/pumps.db";


void setup_pump_table() {
  Serial.println("Setting up pump table");
  if (SPIFFS.exists(pump_file_name)) {
    Serial.printf("Opening file %s\n", pump_file_name);
    pump_file = SPIFFS.open(pump_file_name, "r+");
  } else {
    Serial.printf("Creating file %s\n", pump_file_name);
    pump_file = SPIFFS.open(pump_file_name, "w+");
  }
  if (!pump_file) {
    Serial.println("Error opening file");
    while (1);
  }
  if(pump_file.size() < FILE_SIZE) {
    Serial.println("Increasing pump file size");
    pump_file.seek(0, SeekSet);
    
    byte * data = (byte *)malloc(FILE_SIZE);
    memset(data, 0, FILE_SIZE);
    pump_file.write(data, FILE_SIZE);
    pump_file.flush();
    free(data);
  }
}

void list_pumps(JsonObject& response_root) {
  Serial.println("Listing pumps");
  JsonArray& pump_list = response_root.createNestedArray("pumps");

  for (int a = 0;a < NUM_PUMPS; a++) {
    Pump pump = get_pump(a);
    Serial.printf("adding pump '%d' to response\n", a);
    JsonObject& pump_object = pump_list.createNestedObject();
    pump_object["pumpIndex"] = a;
    pump_object["flowRate"] = pump.flow_rate;
    pump_object["ingredientId"] = pump.ingredient_id;
  }
}

void update_pump(int pump_index, int ingredient_id, int flow_rate) {
  Serial.printf("Updating pump %d, ingredient_id %d, flow_rate %d\n", pump_index, ingredient_id, flow_rate);
  if (pump_index >= NUM_PUMPS) {
    return;
  }
  
  Pump pump = get_pump(pump_index);
  pump.ingredient_id = ingredient_id ? ingredient_id : pump.ingredient_id;
  pump.flow_rate = flow_rate ? flow_rate : pump.flow_rate;

  pump_file.seek(pump_index * sizeof(Pump), SeekSet);
  pump_file.write((uint8_t *)&pump, sizeof(Pump));
  pump_file.flush();
}

Pump get_pump(int index) {
  Serial.printf("Getting pump index: %d\n", index);
  Pump pump = {-1, -1};
  if (index >= NUM_PUMPS) {
    return pump;
  }

  pump_file.seek(index * sizeof(Pump), SeekSet);
  pump_file.read((uint8_t *)&pump, sizeof(Pump));
  Serial.printf("Read pump %d, flow rate: %d, ingredient id: %d\n", index, pump.flow_rate, pump.ingredient_id);
  return pump;
}

