#include <vector>
using std::vector;

#include "GameObject.h"

class ObjectPool {
private:
    vector<GameObject*> _availableObjs;
    vector<GameObject*> _allObjs;
    const int START_POOL_SIZE{5};
public:
    ObjectPool();
    ~ObjectPool();

    GameObject* Get();
    void Return(GameObject* obj);
private:
    void Grow(int count);
};