

# include "../nightingale/nightingale.hpp"
#include<iostream>

int main(){
    ne::Application app;
    ne::create_application(&app);
    ne::run_application(&app); 

    ne::destroy_application(app);
    return 0;
}