#include "ObjectPool.h"
ObjectPool::ObjectPool(GameObject* obj,const int& num )
        : START_POOL_SIZE(num)
{
        //처음 생성량
        for (size_t i = 0; i < START_POOL_SIZE; ++i)
        {
                GameObject* instance = new GameObject();

                _availableObjs.push_back(instance);
                _allObjs.push_back(instance);
        }
}
ObjectPool::~ObjectPool()
{
        for (auto i : _allObjs)
                delete i;
        _allObjs.clear();
}


void ObjectPool::Grow(int count)
{
        for (size_t i = 0; i < count; ++i)
        {
                GameObject* instance = new GameObject();
                _availableObjs.push_back(instance);
                _allObjs.push_back(instance);
        }
}



GameObject* ObjectPool::Get()
{
        if (_availableObjs.empty())
                Grow(5);

        GameObject* obj = _availableObjs.back();
        _availableObjs.pop_back();

        return obj;
}


void ObjectPool::Return(GameObject* obj)
{
        if (obj == nullptr) return;
        
        _availableObjs.push_back(obj);
}