#ifndef LTC_RESOURCE_POOL_H
#define LTC_RESOURCE_POOL_H

#include <vector>

template <typename T, typename Handle>
class ResourcePool
{
public:
    ResourcePool() {}

    bool valid(Handle handle)
    {
        if (!handle)
            return false;

        handle--;
        if (handle >= _valid.size())
            return false;

        if (!_valid[handle])
            return false;

        return true;
    }

    T* get(Handle handle)
    {
        if (!valid(handle))
            return nullptr;

        return _data.data() + (handle - 1);
    }

    Handle alloc()
    {
        if (!_free.empty())
        {
            Handle h;

            h = _free.back();
            _free.pop_back();
            _valid[h] = true;
            return h + 1;
        }
        else
        {
            _data.push_back(T());
            _valid.push_back(true);
            return (Handle)_data.size();
        }
    }

    void dealloc(Handle handle)
    {
        if (!valid(handle))
            return;

        handle--;
        _valid[handle] = false;
        _data[handle] = T();
        _free.push_back(handle);
    }

    void clear()
    {
        _data.clear();
        _free.clear();
        _valid.clear();
    }

private:
    std::vector<T>      _data;
    std::vector<Handle> _free;
    std::vector<bool>   _valid;
};

#endif /* LTC_RESOURCE_POOL_H */
