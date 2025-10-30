#include <proxy_cache.h>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <primary_data.h>
#include <secondary_data.h>

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

String ORC_ProxyCache::dump_cache() const {
    String output = "=== ORC_ProxyCache Dump ===\n";
    
    output += "\n--- Type Cache ---\n";
    output += "Total types: " + String::num_int64(type_cache.size()) + "\n";
    for (const auto& pair : type_cache) {
        if (std::holds_alternative<std::type_index>(pair.first.key)) {
            output += "  [C++] " + String(std::get<std::type_index>(pair.first.key).name()) + 
                     " -> " + String::num_int64(pair.second.size()) + " instances\n";
        } else {
            output += "  [GD] " + String(std::get<std::string>(pair.first.key).c_str()) + 
                     " -> " + String::num_int64(pair.second.size()) + " instances\n";
        }
        
        for (size_t i = 0; i < pair.second.size(); i++) {
            const auto& data = pair.second[i];
            output += "    [" + String::num_int64(i) + "] ";
            if (data.is_valid()) {
                Ref<GDScript> script = data->get_script();
                if (script.is_valid() && !script->get_global_name().is_empty()) {
                    output += String(script->get_global_name());
                } else {
                    output += String(data->get_class());
                }
                output += "@" + String::num_int64((int64_t)data.ptr());
                
                ORC_PrimaryData* primary = Object::cast_to<ORC_PrimaryData>(data.ptr());
                if (primary) {
                    Ref<ORC_ProxyObject> proxy_obj = primary->get_proxy_object();
                    if (proxy_obj.is_valid()) {
                        Node* node = proxy_obj->get_node();
                        if (node) {
                            output += "(" + String(node->get_class()) + ":" + node->get_name() + ")";
                        } else {
                            output += "(node: <null>)";
                        }
                    } else {
                        output += " (proxy_object: <invalid>)";
                    }
                }
                
                ORC_SecondaryData* secondary = Object::cast_to<ORC_SecondaryData>(data.ptr());
                if (secondary) {
                    TypedArray<ORC_PrimaryData> primaries = secondary->get_primary_data_array();
                    output += " (shared by " + String::num_int64(primaries.size()) + ":";
                    for (int j = 0; j < primaries.size(); j++) {
                        Ref<ORC_PrimaryData> prim = primaries[j];
                        if (prim.is_valid()) {
                            Ref<GDScript> prim_script = prim->get_script();
                            if (prim_script.is_valid() && !prim_script->get_global_name().is_empty()) {
                                output += " " + String(prim_script->get_global_name());
                            } else {
                                output += " " + String(prim->get_class());
                            }
                            output += "@" + String::num_int64((int64_t)prim.ptr());
                        } else {
                            output += " <invalid>";
                        }
                        if (j < primaries.size() - 1) output += ",";
                    }
                    output += ")";
                }
            } else {
                output += "<invalid>";
            }
            output += "\n";
        }
    }
    
    output += "\n--- ID Cache ---\n";
    output += "Total unique IDs: " + String::num_int64(id_cache.size()) + "\n";
    for (const auto& pair : id_cache) {
        output += "  ID: " + String::num_int64(pair.first) + 
                 " -> refcount: " + String::num_int64(std::get<1>(pair.second));
        const auto& data = std::get<0>(pair.second);
        if (data.is_valid()) {
            Ref<GDScript> script = data->get_script();
            if (script.is_valid() && !script->get_global_name().is_empty()) {
                output += " (" + String(script->get_global_name());
            } else {
                output += " (" + String(data->get_class());
            }
            output += "@" + String::num_int64((int64_t)data.ptr()) + ")";
        } else {
            output += " (<invalid>)";
        }
        output += "\n";
    }
    
    output += "\n=========================\n";
    return output;
}

}
