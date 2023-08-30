

# include "../nightingale/nightingale.hpp"

#include<iostream>
#include <vector>

int main(){
    
    // create the instance which create a default scene
    nge::Nightingale app =  nge::Nightingale(600, 800, "Test");

    
    // create background
        // create a gameobject 
        nge::GameObject bg_object = nge::GameObject("bg");    

        // create a texture
        app.createTexture("bg_tex", "textures/SET1_bakcground_day1.png");

        // add texture property to gameobject
        bg_object.properties["texture"] = "bg_tex";

        // add object to the scene
        app.scenes["default"].gameObjects.push_back(bg_object);

    // create platform
        // create gameobject for the platform
        nge::GameObject plat_object = nge::GameObject("platform");

        // create collider for the platform
        // nge::Collider collider = nge::Collider("platform", 50, 100, 700, 50);

        // add collider property to the platform
        // plat_object.properties["collider"] = "platform";

        // create gameobjects for each tile and to the children of the platform
        // create textures need for every tile
        // add the texture property for each of the tile
        

    // create character
        // create gameobject for the character
        // create the collider for the character
        // create the texture for the character
        // add the collider property 
        // add the texture property
        // create rigid body for character
        // add rigid body property
        



    // run the application
    app.run();


    return 0;

} // end main