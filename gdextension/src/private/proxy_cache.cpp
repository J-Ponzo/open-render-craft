#include <proxy_cache.h>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot {

void ORC_ProxyCache::_bind_methods() {
    // TODO : unbind if not needed
    ClassDB::bind_method(D_METHOD("register_data", "proxy_data"), &ORC_ProxyCache::register_data);
    ClassDB::bind_method(D_METHOD("unregister_data", "proxy_data"), &ORC_ProxyCache::unregister_data);
}

bool ORC_ProxyCache::register_data(Ref<ORC_ProxyData> proxy_data, int64_t unique_id) {
    if (unique_id != -1) {
        auto it = id_cache.find(unique_id);
        if (it != id_cache.end()) return false;
        else  id_cache[unique_id] = std::make_tuple(proxy_data, 1);
    }

    std::type_index type_id = typeid(*proxy_data.ptr());
    if (type_cache.find(type_id) == type_cache.end()) {
        type_cache[type_id] = std::vector<Ref<ORC_ProxyData>>();
    } 
    else {
        auto& vec = type_cache[type_id];
        if (std::find(vec.begin(), vec.end(), proxy_data) != vec.end()) 
            return false;
    }
    type_cache[type_id].push_back(proxy_data);
    return true;
}

bool ORC_ProxyCache::unregister_data(Ref<ORC_ProxyData> proxy_data) {
    std::type_index type_id = typeid(*proxy_data.ptr());
    auto type_it = type_cache.find(type_id);
    if (type_it == type_cache.end()) return false;

    auto& vec = type_it->second;
    size_t old_size = vec.size();
    vec.erase(std::remove(vec.begin(), vec.end(), proxy_data), vec.end());
    if (vec.size() == old_size) return false;

    return true;
}

std::vector<Ref<ORC_ProxyData>> ORC_ProxyCache::get_by_type(std::type_index type_id) const {
    std::vector<Ref<ORC_ProxyData>> result;
    auto it = type_cache.find(type_id);
    if (it != type_cache.end()) {
        result = it->second;
    }
    return result;
}

Ref<ORC_ProxyData> ORC_ProxyCache::get_by_unique_id(int64_t unique_id) const {
    Ref<ORC_ProxyData> result;
    auto it = id_cache.find(unique_id);
    if (it != id_cache.end()) {
        result = std::get<0>(it->second);
    }
    return result;
}

bool ORC_ProxyCache::increment_refcount(int64_t unique_id) {
    auto it = id_cache.find(unique_id);
    if (it != id_cache.end()) {
        std::get<1>(it->second)++;
        return true;
    }
    return false;
}

bool ORC_ProxyCache::decrement_refcount(int64_t unique_id) {
    auto it = id_cache.find(unique_id);
    if (it != id_cache.end()) {
        std::get<1>(it->second)--;
        if (std::get<1>(it->second) == 0) {
            id_cache.erase(it);
            unregister_data(std::get<0>(it->second));
        }
        return true;
    }
    return false;
}

}
