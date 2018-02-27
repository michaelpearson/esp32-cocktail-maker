#include "definitions.h"

#define TABLE_SIZE 524288

void setup_tables() {
  Serial.println("SPIFFS Init");
  SPIFFS.begin();
  setup_pump_table();
  setup_ingredients_table();
  setup_cocktails_table();
  Serial.println("Setup Complete");
}

template<typename T> void setup_table(const char * file_name, const char * table_name, EDB& database, File * file) {
  Serial.printf("Setup database '%s', record size: %d\n", table_name, sizeof(T));

  if (SPIFFS.exists(file_name)) {
    Serial.printf("Opening file %s\n", file_name);
    *file = SPIFFS.open(file_name, "r+");
  } else {
    Serial.printf("Creating file %s\n", file_name);
    *file = SPIFFS.open(file_name, "w+");
    database.create(0, TABLE_SIZE, (unsigned int)sizeof(T));
  }
  if (!(*file)) {
    Serial.println("Error opening file");
    while (1);
  }
  Serial.println("Opening database");
  EDB_Status result = database.open(0);
  if (result != EDB_OK) {
    Serial.println("Failed opening database - try re-creating the database");
    while (1);
  }
}


template<typename T> void delete_record(EDB& database, int id) {
  T record;
  for (int recno = database.count(); recno > 0; recno--) {
    EDB_Status result = database.readRec(recno, EDB_REC record);
    if (result != EDB_OK) {
      printError(result);
      continue;
    }
    if (record.id == id) {
      Serial.printf("Deleting record %d\n", recno);
      database.deleteRec(recno);
    }
  }
}

template<typename T> void add_record(EDB& database, T * record) {
  record->id = _next_id<T>(database);
  EDB_Status result = database.appendRec(EDB_REC (*record));
  if (result != EDB_OK) printError(result);
}

template<typename T> int _next_id(EDB& database) {
  T i;
  int max_id = 0;
  for (int recno = database.count(); recno > 0; recno--) {
    EDB_Status result = database.readRec(recno, EDB_REC i);
    if (result != EDB_OK) {
      printError(result);
      continue;
    }
    if (i.id > max_id) {
      max_id = i.id;
    }
  }
  return max_id + 1;
}

void printError(EDB_Status err) {
  Serial.print("ERROR: ");
  switch (err)     {
    case EDB_OUT_OF_RANGE:
      Serial.println("Recno out of range");
      break;
    case EDB_TABLE_FULL:
      Serial.println("Table full");
      break;
    case EDB_OK:
    default:
      Serial.println("OK");
      break;
  }
  while (1);
}
