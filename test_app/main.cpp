

# include "../nightingale/nightingale.hpp"

#include<iostream>
#include <vector>
#include<string>

int main(){
    // create the instance which create a default scene
    const int height = 768;
    const int width = 1376;

    nge::Nightingale app =  nge::Nightingale(height, width, "Test");

    
    // create background
        // create a gameobject 

        nge::GameObject *bg_object = new nge::GameObject("bg", height/2, width/2);    
        // bg_object->transform->z = -00;
        // bg_object->transform->scaleX = 20;
        // bg_object->transform->scaleY = 20;

        // bg_object->transform->x = 100;
        // bg_object->transform->y = 100;

        // bg_object.setScale(2);

        // create a texture
        app.createTexture("bg_tex", "textures/SET1_bakcground_day1.png");

        // add texture property to gameobject
        bg_object->properties["texture"] = "bg_tex";

        // add object to the scene
        app.scenes["default"].gameObjects.push_back(bg_object);
        
    // create platform
        // create gameobject for the platform
        // nge::GameObject plat_object = nge::GameObject("platform", height/2, width/2);

        // create collider for the platform
        // nge::Collider collider = nge::Collider("platform", 50, 100, 700, 50);

        // add collider property to the platform
        // plat_object.properties["collider"] = "platform";

        // load the texture for tile map
        
        app.createTexture("build_main", "textures/SET1_Mainlev_build.png");
        // // create gameobjects for each tile and to the children of the platform
        // float offset = 0.19*width/2;
        // for(int i = 0; i < 5; i++){

        //     std::string tile_name = "tile" + std::to_string(i);
        //     nge::GameObject *tile1 = new nge::GameObject(tile_name, height/2, width/2);
        //     tile1->transform->scaleX=0.1;
        //     tile1->transform->scaleY=0.1;
        //     tile1->transform->rot = 180;
        //     tile1->setTexScale(0.06, 0.0, 0.51);
        //     tile1->transform->x = -i*offset;
        //     tile1->transform->y = 100;
        //     tile1->properties["texture"] = "build_main";

        //     // // add the texture property for each of the tile
        //     app.scenes["default"].gameObjects.push_back(tile1);
        // }
        nge::GameObject *tile1 = new nge::GameObject("plat", height/2, width/2);
        tile1->transform->scaleX=1;
        tile1->transform->scaleY=0.1;
        tile1->transform->rot = 180;
        tile1->setTexScale(0.1, 0.0, 0);
        tile1->transform->x = 0;
        tile1->transform->y = 100;
        tile1->properties["texture"] = "build_main";
        app.scenes["default"].gameObjects.push_back(tile1);

        nge::RigidBody2D * platRB = app.physic2d.addRigidBody2D(tile1->transform);     
        platRB->dynamic = false;

        app.physic2d.addBoxCollider2D(
            platRB,
            tile1->transform,
            0,
            0,
            50,
            600
        );
           


        app.createTexture("character_idle", "textures/Idle-Sheet.png");
        app.createTexture("character_jump", "textures/Jump-End-Sheet.png");
    // create character
        // create gameobject for the character
        nge::GameObject *character = new nge::GameObject("character", height/2, width/2);
        // character->transform->rot = 180;
        character->flipX = -1;
        character->transform->y = -100;
        character->transform->scaleY = 0.16*1.5;
        character->transform->scaleX = 0.12*1.5;
        // character->transform->rot = 180;
        character->setTexScale(0.75, 0.49, 0.07);


        // add the texture property
        character->properties["texture"] = "character_idle";

        app.scenes["default"].gameObjects.push_back(character);

        nge::RigidBody2D* rb =  app.physic2d.addRigidBody2D(character->transform);
        app.physic2d.addBoxCollider2D(
            rb,
            character->transform,
            0,
            0,
            10,
            10
        );
        // create rigid body for character


        // add rigid body property
        

    // load the scene
    app.loadScene("default");

    // run the application
    app.run();


    return 0;

} // end main