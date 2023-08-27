

# include "../nightingale/nightingale.hpp"

#include<iostream>
#include <vector>

int main(){
    
    // create the instance which create a default scene
    nge::Nightingale app =  nge::Nightingale(600, 800, "Test");

    
    // create background
        // create a gameobject 
    nge::GameObject bg_object = nge::GameObject();    
        // create a texture
    
        // add texture property to gameobject


    // create platform
        // create gameobject for the platform
        // create collider for the platform
        // add collider property to the platform

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