#include "MeshList.hpp"

class NoSortMeshList : public MeshList {
  public:
    NoSortMeshList();
    ~NoSortMeshList();

    void sort() override;

    void addGameObject(GameObject *obj) override;

    void addGameObject(GameObject *obj, int i);

    GameObject *longGet(int i) override;
};