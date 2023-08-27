
#include "nightingale.hpp"
#include "logger/logger.hpp"



void nge::Nightingale::run(){
    nge::Logger::getInstance().log("Inside run");
}

nge::Nightingale::Nightingale(){
    //init logger
    nge::Logger::getInstance().start(nge::debug, "llog.txt");
}

nge::Nightingale::~Nightingale(){
    
}