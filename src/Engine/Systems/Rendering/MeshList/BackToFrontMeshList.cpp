#include "BackToFrontMeshList.hpp"

BackToFrontMeshList::BackToFrontMeshList() : MeshList() {}

void BackToFrontMeshList::sort() {
    bool sorted = false;
    while( !sorted ) {
        bool element_swapped = false;
        for( int i = 0; i < list.size(); i++ ) {
            if ( i > 0 ) {
                if(dist(list[i]) > dist(list[i - 1])) {
                    std::swap(list[i],list[i-1]);
                }
            }
        }
        if( !element_swapped )
            sorted = true;
    }
}

void BackToFrontMeshList::addGameObject( GameObject * obj ) {
    bool inserted = false;
    int i = 0;
    float d_obj = dist(obj);
    while(!inserted) {
        if(dist(list[i]) > d_obj) {
            auto pos = list.begin() + i;
            list.insert(pos,obj);
            inserted = true;
        }
        i++;
    }
}

GameObject * BackToFrontMeshList::longGet( int i ) { 
    GameObject * element = list[i];

    if ( i > 0 ) {
        if(dist(list[i]) > dist(list[i - 1])) {
            std::swap(list[i],list[i-1]);
        }
    }
    return element;
}