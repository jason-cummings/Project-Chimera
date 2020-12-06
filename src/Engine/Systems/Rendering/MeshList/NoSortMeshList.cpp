#include "NoSortMeshList.hpp"

NoSortMeshList::NoSortMeshList() : MeshList() {}

NoSortMeshList::~NoSortMeshList() {}

void NoSortMeshList::sort() {
    return;
}

void NoSortMeshList::addGameObject(GameObject *obj) {
    list.push_back(obj);
}

void NoSortMeshList::addGameObject(GameObject *obj, int i) {
    auto pos = list.begin() + i;
    list.insert(pos, obj);
}

GameObject *NoSortMeshList::longGet(int i) {
    return list[i];
}