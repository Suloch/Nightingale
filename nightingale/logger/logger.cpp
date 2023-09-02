#include "logger.hpp"

#include <iostream>


void nge::Logger::start(LogLevel level, const char* filename){
    if(filename == nullptr){
        cerr = true;
    }else{
        file.open(filename);
        file.clear();
        cerr = false;
    }
    this->level = level;
}

// template<typename T>
// void nge::Logger::log(const T &msg){
//     if(cerr){
//         std::cerr<<line<<": "<<level<<": "<<msg<<std::endl;
//     }else{
//         file<<line<<": "<<level<<": "<<msg<<std::endl;
//     }
// }

void nge::Logger::log(const char* log ){
    if(cerr){
        std::cerr<<line<<": "<<level<<": "<<log<<std::endl;
    }else{
        file<<line<<": "<<level<<": "<<log<<std::endl;
    }
    line++;
}

void nge::Logger::log(const char* log, LogLevel level ){
    if(level >= this->level){
        if(cerr){
            std::cerr<<line<<": "<<level<<": "<<log<<std::endl;
        }else{
            file<<line<<": "<<level<<": "<<log<<std::endl;
        }
    }
    line++;
}

void nge::Logger::stop(){
    if(!cerr){
        file.close();
    }
}