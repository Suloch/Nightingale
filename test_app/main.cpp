

# include "../nightingale/nightingale.hpp"
#include "nightingale/logger/logger.hpp"

#include<iostream>
#include <vector>
#include<string>

int main(){
    // create the instance which create a default scene
    nge::Nightingale app =  nge::Nightingale(600, 800, "Test");

    
    // create background
        // create a gameobject 

    // create platform
        // create gameobject for the platform
        // nge::GameObject plat_object = nge::GameObject("platform");

        // create collider for the platform
        // nge::Collider collider = nge::Collider("platform", 50, 100, 700, 50);

        // add collider property to the platform
        // plat_object.properties["collider"] = "platform";

        // load the texture for tile map
        nge::GameObject bg_object = nge::GameObject("bg");    
        bg_object.setScale(2);

        // create a texture
        app.createTexture("bg_tex", "textures/SET1_bakcground_day1.png");

        // add texture property to gameobject
        bg_object.properties["texture"] = "bg_tex";

        // add object to the scene
        app.scenes["default"].gameObjects.push_back(bg_object);
        
        app.createTexture("build_main", "textures/SET1_Mainlev_build.png");

        // // create gameobjects for each tile and to the children of the platform
        float offset = 0.15;
        for(int i = 0; i < 7; i++){
            std::string tile_name = "tile" + std::to_string(i);
            nge::GameObject tile1 = nge::GameObject(tile_name);
            tile1.setScale(0.15);
            tile1.setTexScale(0.07, 0.0, 0.42);
            tile1.setPosition(0.5-i*offset, 0.25);
            tile1.properties["texture"] = "build_main";

            // // add the texture property for each of the tile
            app.scenes["default"].gameObjects.push_back(tile1);
        }
           
           


    // create character
        // create gameobject for the character
        // create the collider for the character
        // create the texture for the character
        // add the collider property 
        // add the texture property
        // create rigid body for character
        // add rigid body property
        

    // load the scene
    app.loadScene("default");

    // run the application
    app.run();


    return 0;

} // end main