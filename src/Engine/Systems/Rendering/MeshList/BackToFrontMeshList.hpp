#include "MeshList.hpp"

class BackToFrontMeshList : public MeshList {
  public:
    BackToFrontMeshList();
    ~BackToFrontMeshList();

    void sort() override;

    void addGameObject(GameObject *obj) override;

    GameObject *longGet(int i) override;
};