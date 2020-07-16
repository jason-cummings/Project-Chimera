#ifndef MESH_LIST_HPP
#define MESH_LIST_HPP

#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include "../../../GameObject.hpp"

#include <iostream>
#include <algorithm>

// MeshList Class
//  - A MeshList is a container for a vector. Its purpose is to maintain a list of meshes to be rendered in a 
//    particular order without requiring the full list to be sorted every frame. Instead, a Meshlist will sort 
//    incrementally, moving an element a maximum of one spot per frame, under the assumption that the list is already
//    almost completely sorted

// Assumptions: ( This class relies on the following assumptions in order to function properly and most effectively )
//  - after the vast majority of objects have been loaded into the meshlist, but before this meshlist is rendered for
//    the first time, sort will be called
//  - the camera will move a relatively small distance between frames. This means that the camera is moving along a 
//    path, and isn't being teleported across the map between frames
//  - if the camera is teleported between frames, sort will be called before the next frame is rendered

class MeshList {
private:
    glm::vec3 camera_loc;


protected:

    std::vector<GameObject *> list;

    float dist( GameObject * obj ); // get distance from gameObject a to the camera

public:
    MeshList();
    virtual ~MeshList();


    // setter for camera location
    inline void setCameraLoc( glm::vec3 camera_loc_in ) { camera_loc = camera_loc_in; }


    // inline wrapper functions for basic list functionality

    inline int size() { return (int)list.size(); }; // get the number of gameObjects in the meshList

    inline GameObject * quickGet( int i ) { return list[i]; } // replacement for accessing elements in the list. only accesses the element, and does nothing else

    inline void clearList() { list.clear(); }

    inline void removeObject( GameObject * obj ) { auto it = std::remove( list.begin(), list.end(), obj );
	 if( it != list.end() ) list.erase( it );} //std::remove( list.begin(), list.end(), obj ); }



    // virtual functions - these must be implemented based on the type of meshlist

    virtual void sort() = 0; // sort the entire meshList - this should not be done every frame. it should be done once

    virtual void addGameObject( GameObject * obj ) = 0;

    virtual GameObject * longGet( int i ) = 0; // returns the element in the list, but may also have additional functionality - ex: check if this element 
    //                             should be before the element in front of it and swap positions if necessary in order to keep the list sorted

};

#endif