
#include "store.hpp"
#include "../logger/logger.hpp"

nge::store::Store::Store(std::string filename){
    sqlite3_open(filename.c_str(), &db);

    createInitialTables();
}


void nge::store::Store::createInitialTables(){

    //check if the tables are already created



    //create all tables

    //create table for app settings
    std::string nightingaleTableSql = "CREATE TABLE NIGHTINGALE("
                      "ID                       INT PRIMARY KEY     NOT NULL, "
                      "NAME                     TEXT    NOT NULL, "
                      "INIT_COMPLETED           INT, "
                      "HEIGHT                   CHAR(50), "
                      "WIDTH                    REAL );";
    
    std::string objectTableSql = "CREATE TABLE OBJECTS("
        "ID                     INT PRIMARY KEY NOT NULL,"
        "NAME                   TEXT NOT NULL,"
        "X                      REAL,"
        "Y                      REAL,"
        "Z                      REAL,"
        "SCALE_X                REAL,"
        "SCALE_Y                REAL,"
        "SCALE_Z                REAL,"
        "ROT                    REAL,"
        "TEX_SCALE              REAL,"
        "TEX_OFFSET_X           REAL,"
        "TEX_OPFFSET_Y          REAL,"
        "FLIP_X                 INT,"
        "FLIP_y                 INT"
    ")";

    std::string objectPropertiesTableSql = "CREATE TABLE OBJECT_PROPERTIES("
        "ID                     INT PRIMARY KEY NOT NULL,"
        "NAME                   TEXT NOT NULL,"
        "VALUE                  TEXT"
        ")";
    
    std::string textureTableSql = "CREATE TABLE TEXTURES("
        "ID                     INT PRIMARY KEY NOT NULL,"
        "NAME                   TEXT,"
        "FILE_PATH              TEXT"
    ")";

    int exit = 0;
    char *error;
    Logger::getInstance().log("Creating Tables");
    exit = sqlite3_exec(db, nightingaleTableSql.c_str(), NULL, 0,  &error);
    exit = sqlite3_exec(db, objectTableSql.c_str(), NULL, 0,  &error);
    exit = sqlite3_exec(db, objectPropertiesTableSql.c_str(), NULL, 0,  &error);
    exit = sqlite3_exec(db, textureTableSql.c_str(), NULL, 0,  &error);
}