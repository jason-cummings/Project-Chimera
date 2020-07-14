#include "MeshList.hpp"

class NoSortMeshList : public MeshList {
public:
    NoSortMeshList();

    void sort() override;

    void addGameObject( GameObject * obj ) override;

    GameObject * longGet( int i ) override;
};