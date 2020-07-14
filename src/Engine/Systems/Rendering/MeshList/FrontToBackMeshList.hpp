#include "MeshList.hpp"

class FrontToBackMeshList : public MeshList {
public:
    FrontToBackMeshList();

    void sort() override;

    void addGameObject( GameObject * obj ) override;

    GameObject * longGet( int i ) override;
};