#include <proxy_cache.h>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot {

void ORC_ProxyCache::_bind_methods() {
}

// TODO : inline in .h ?
TypeKey ORC_ProxyCache::get_type_key(Ref<ORC_ProxyData> proxy_data) {
    if (!proxy_data.is_valid()) {
        return TypeKey(std::string(""));
    }

    Ref<GDScript> script = proxy_data->get_script();
    if (script.is_valid()) {
        return TypeKey(script);
    } else {
        return TypeKey(typeid(*proxy_data.ptr()));
    }
}

bool ORC_ProxyCache::register_data(Ref<ORC_ProxyData> proxy_data, int64_t unique_id) {
    if (!proxy_data.is_valid()) return false;

    if (unique_id != -1) {
        id_cache[unique_id] = std::make_tuple(proxy_data, 1);
    }

    TypeKey type_key = get_type_key(proxy_data);
    type_cache[type_key].push_back(proxy_data);

    UtilityFunctions::print("[ORC_ProxyCache] Registered. Total types: ", (int64_t)type_cache.size());
    for (const auto& pair : type_cache) {
        if (std::holds_alternative<std::type_index>(pair.first.key)) {
            UtilityFunctions::print("  - C++: ", std::get<std::type_index>(pair.first.key).name(), 
                                   " (", (int64_t)pair.second.size(), " instances)");
        } else {
            UtilityFunctions::print("  - GD: ", std::get<std::string>(pair.first.key).c_str(), 
                                   " (", (int64_t)pair.second.size(), " instances)");
        }
    }

    return true;
}

bool ORC_ProxyCache::unregister_data(Ref<ORC_ProxyData> proxy_data) {
    if (!proxy_data.is_valid()) return false;

    TypeKey type_key = get_type_key(proxy_data);
    auto it = type_cache.find(type_key);
    if (it == type_cache.end()) return false;

    auto& vec = it->second;
    size_t old_size = vec.size();
    vec.erase(std::remove(vec.begin(), vec.end(), proxy_data), vec.end());
    if (vec.size() == old_size) return false;

    UtilityFunctions::print("[ORC_ProxyCache] Unregistered. Total types: ", (int64_t)type_cache.size());
    for (const auto& pair : type_cache) {
        if (std::holds_alternative<std::type_index>(pair.first.key)) {
            UtilityFunctions::print("  - C++: ", std::get<std::type_index>(pair.first.key).name(), 
                                   " (", (int64_t)pair.second.size(), " instances)");
        } else {
            UtilityFunctions::print("  - GD: ", std::get<std::string>(pair.first.key).c_str(), 
                                   " (", (int64_t)pair.second.size(), " instances)");
        }
    }

    return true;
}

std::vector<Ref<ORC_ProxyData>> ORC_ProxyCache::get_by_type(const TypeKey& type_key) const {
    auto it = type_cache.find(type_key);
    return (it != type_cache.end()) ? it->second : std::vector<Ref<ORC_ProxyData>>{};
}

Ref<ORC_ProxyData> ORC_ProxyCache::get_by_unique_id(int64_t unique_id) const {
    auto it = id_cache.find(unique_id);
    return (it != id_cache.end()) ? std::get<0>(it->second) : Ref<ORC_ProxyData>();
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
            unregister_data(std::get<0>(it->second));
            id_cache.erase(it);
        }
        return true;
    }
    return false;
}

}
