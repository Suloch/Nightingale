
#pragma once

#include<sqlite3.h>
#include<string>

namespace nge{
    namespace store{
        class Store{
            sqlite3 *db;
            void createInitialTables();
            public:
                Store(std::string filename);
                ~Store();
        };
    }
}