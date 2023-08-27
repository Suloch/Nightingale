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

void nge::Logger::log(const char* log ){
    if(cerr){
        std::cerr<<level<<": "<<log<<std::endl;
    }else{
        file<<level<<": "<<log<<std::endl;
    }
}

void nge::Logger::log(const char* log, LogLevel level ){
    if(level >= this->level){
        if(cerr){
            std::cerr<<level<<": "<<log<<std::endl;
        }else{
            file<<level<<": "<<log<<std::endl;
        }
    }
}

void nge::Logger::stop(){
    if(!cerr){
        file.close();
    }
}