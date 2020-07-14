#include "NoSortMeshList.hpp"

NoSortMeshList::NoSortMeshList() : MeshList() {}

void NoSortMeshList::sort() {
    return;
}

void NoSortMeshList::addGameObject( GameObject * obj ) {
    list.push_back(obj);
}

GameObject * NoSortMeshList::longGet( int i ) { 
    return list[i];
}