#include <proxy_registry.h>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <primary_data.h>
#include <secondary_data.h>
#include <feature_query.h>

namespace godot {

void ORC_ProxyRegistry::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_flag", "proxy_data", "flag_mask", "value"), &ORC_ProxyRegistry::set_flag);
    ClassDB::bind_method(D_METHOD("get_by_query", "query"), &ORC_ProxyRegistry::get_by_query);
}

// TODO : inline in .h ?
TypeKey ORC_ProxyRegistry::get_type_key(Ref<ORC_ProxyData> proxy_data) {
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

bool ORC_ProxyRegistry::register_data(Ref<ORC_ProxyData> proxy_data, int64_t unique_id) {
    if (!proxy_data.is_valid()) return false;

    if (unique_id != -1) {
        id_registry[unique_id] = std::make_tuple(proxy_data, 1);
    }

    TypeKey type_key = get_type_key(proxy_data);
    type_registry[type_key].push_back(proxy_data);

    return true;
}

bool ORC_ProxyRegistry::unregister_data(Ref<ORC_ProxyData> proxy_data) {
    if (!proxy_data.is_valid()) return false;

    remove_from_query_cache(proxy_data);
    data_flags.erase(proxy_data.ptr());

    TypeKey type_key = get_type_key(proxy_data);
    auto it = type_registry.find(type_key);
    if (it == type_registry.end()) return false;

    auto& vec = it->second;
    size_t old_size = vec.size();
    vec.erase(std::remove(vec.begin(), vec.end(), proxy_data), vec.end());
    if (vec.size() == old_size) return false;

    return true;
}

std::vector<Ref<ORC_ProxyData>> ORC_ProxyRegistry::get_by_type(const TypeKey& type_key) const {
    auto it = type_registry.find(type_key);
    return (it != type_registry.end()) ? it->second : std::vector<Ref<ORC_ProxyData>>{};
}

Ref<ORC_ProxyData> ORC_ProxyRegistry::get_by_unique_id(int64_t unique_id) const {
    auto it = id_registry.find(unique_id);
    return (it != id_registry.end()) ? std::get<0>(it->second) : Ref<ORC_ProxyData>();
}

bool ORC_ProxyRegistry::increment_refcount(int64_t unique_id) {
    auto it = id_registry.find(unique_id);
    if (it != id_registry.end()) {
        std::get<1>(it->second)++;
        return true;
    }
    return false;
}

bool ORC_ProxyRegistry::decrement_refcount(int64_t unique_id) {
    auto it = id_registry.find(unique_id);
    if (it != id_registry.end()) {
        std::get<1>(it->second)--;
        if (std::get<1>(it->second) == 0) {
            unregister_data(std::get<0>(it->second));
            id_registry.erase(it);
        }
        return true;
    }
    return false;
}

// TODO : put in helper file
template <typename T>
static String get_type_name(const Ref<T>& ref) {
    if (!ref.is_valid()) return "<invalid>";
    
    Ref<GDScript> script = ref->get_script();
    if (script.is_valid() && !script->get_global_name().is_empty()) {
        return script->get_global_name();
    } else {
        return ref->get_class();
    }
}

// TODO : put in helper file
static String get_type_name(const Object* obj) {
    if (!obj) return "<null>";
    
    Ref<GDScript> script = obj->get_script();
    if (script.is_valid() && !script->get_global_name().is_empty()) {
        return script->get_global_name();
    } else {
        return obj->get_class();
    }
}

// TODO : put in helper file
template <typename T>
static String get_type_and_address(const Ref<T>& ref) {
    if (!ref.is_valid()) return "<invalid>";
    
    String result = get_type_name(ref);
    
    // Add object name if it's a Node
    Node* node = Object::cast_to<Node>(ref.ptr());
    if (node) {
        result += "[\"" + node->get_name() + "\"]";
    }
    
    result += "@" + String::num_int64((int64_t)ref.ptr());
    return result;
}

// TODO : put in helper file
static String get_type_and_address(const Object* obj) {
    if (!obj) return "<null>";
    
    String result = get_type_name(obj);
    
    // Add object name if it's a Node
    const Node* node = Object::cast_to<Node>(obj);
    if (node) {
        result += "[\"" + node->get_name() + "\"]";
    }
    
    result += "@" + String::num_int64((int64_t)obj);
    return result;
}

// TODO : put in helper file
static String get_primary_node_info(ORC_PrimaryData* primary) {
    if (!primary) return "";
    
    Ref<ORC_ProxyObject> proxy_obj = primary->get_proxy_object();
    if (proxy_obj.is_valid()) {
        Node* node = proxy_obj->get_node();
        if (node) {
            return " (" + get_type_and_address(node) + ")";
        } else {
            return " (node: <null>)";
        }
    } else {
        return " (proxy_object: <invalid>)";
    }
}

String ORC_ProxyRegistry::dump_registry() const {
    String output = "=== ORC_ProxyRegistry Dump ===\n";
    
    output += "\n--- Type Registry ---\n";
    output += "Total types: " + String::num_int64(type_registry.size()) + "\n";
    for (const auto& pair : type_registry) {
        // Determine if this is a PRIMARY or SECONDARY type
        String category = "";
        if (!pair.second.empty() && pair.second[0].is_valid()) {
            if (Object::cast_to<ORC_PrimaryData>(pair.second[0].ptr())) {
                category = " [--PRIMARY--]";
            } else if (Object::cast_to<ORC_SecondaryData>(pair.second[0].ptr())) {
                category = " [--SECONDARY--]";
            }
        }
        
        if (std::holds_alternative<std::type_index>(pair.first.key)) {
            output += "  [C++]" + category + " " + String(std::get<std::type_index>(pair.first.key).name()) + 
                     " -> " + String::num_int64(pair.second.size()) + " instances\n";
        } else {
            output += "  [GD]" + category + " " + String(std::get<std::string>(pair.first.key).c_str()) + 
                     " -> " + String::num_int64(pair.second.size()) + " instances\n";
        }
        
        for (size_t i = 0; i < pair.second.size(); i++) {
            const auto& data = pair.second[i];
            output += "    [" + String::num_int64(i) + "] ";
            if (data.is_valid()) {
                output += get_type_and_address(data);
                
                ORC_PrimaryData* primary = Object::cast_to<ORC_PrimaryData>(data.ptr());
                if (primary) {
                    output += get_primary_node_info(primary);
                }
                
                ORC_SecondaryData* secondary = Object::cast_to<ORC_SecondaryData>(data.ptr());
                if (secondary) {
                    TypedArray<ORC_PrimaryData> primaries = secondary->get_primary_data_array();
                    output += " shared by " + String::num_int64(primaries.size()) + ": \n";
                    for (int j = 0; j < primaries.size(); j++) {
                        Ref<ORC_PrimaryData> prim = primaries[j];
                        output += "        - ";
                        if (prim.is_valid()) {
                            output += get_type_and_address(prim);
                            output += get_primary_node_info(prim.ptr());
                        } else {
                            output += "<invalid>";
                        }
                        output += "\n";
                    }
                } else {
                    output += "\n";
                }
            } else {
                output += "<invalid>\n";
            }
        }
    }
    
    output += "\n--- ID Registry ---\n";
    output += "Total unique IDs: " + String::num_int64(id_registry.size()) + "\n";
    for (const auto& pair : id_registry) {
        output += "  ID: " + String::num_int64(pair.first) + 
                 " -> refcount: " + String::num_int64(std::get<1>(pair.second));
        const auto& data = std::get<0>(pair.second);
        if (data.is_valid()) {
            output += " (" + get_type_and_address(data) + ")";
        } else {
            output += " (<invalid>)";
        }
        output += "\n";
    }
    
    output += "\n=========================\n";
    return output;
}

bool ORC_ProxyRegistry::matches_query(uint64_t flags, const Ref<ORC_FeatureQuery>& query) const {
    if (!query.is_valid()) return false;
    
    uint64_t mask = query->get_mask();
    uint64_t value = query->get_value();
    
    return (flags & mask) == (value & mask);
}

void ORC_ProxyRegistry::update_query_cache_for_data(Ref<ORC_ProxyData> proxy_data, uint64_t old_flags, uint64_t new_flags) {
    if (!proxy_data.is_valid()) return;
    
    for (auto& cache_entry : query_cache) {
        ORC_FeatureQuery* query_ptr = cache_entry.first;
        if (!query_ptr) continue;
        
        Ref<ORC_FeatureQuery> query;
        query.reference_ptr(query_ptr);
        
        std::vector<Ref<ORC_ProxyData>>& data_list = cache_entry.second;
        
        bool old_match = matches_query(old_flags, query);
        bool new_match = matches_query(new_flags, query);
        
        if (old_match && !new_match) {
            data_list.erase(std::remove(data_list.begin(), data_list.end(), proxy_data), data_list.end());
        } else if (!old_match && new_match) {
            data_list.push_back(proxy_data);
        }
    }
}

void ORC_ProxyRegistry::remove_from_query_cache(Ref<ORC_ProxyData> proxy_data) {
    if (!proxy_data.is_valid()) return;
    
    for (auto& cache_entry : query_cache) {
        std::vector<Ref<ORC_ProxyData>>& data_list = cache_entry.second;
        data_list.erase(std::remove(data_list.begin(), data_list.end(), proxy_data), data_list.end());
    }
}

void ORC_ProxyRegistry::add_query_to_cache(const Ref<ORC_FeatureQuery>& query) {
    if (!query.is_valid()) return;
    
    ORC_FeatureQuery* query_ptr = query.ptr();
    
    if (query_cache.find(query_ptr) != query_cache.end()) return;
    
    std::vector<Ref<ORC_ProxyData>> matching_data;
    
    for (const auto& flags_entry : data_flags) {
        ORC_ProxyData* data_ptr = flags_entry.first;
        uint64_t flags = flags_entry.second;
        
        if (!data_ptr) continue;
        
        Ref<ORC_ProxyData> proxy_data;
        proxy_data.reference_ptr(data_ptr);
        
        if (matches_query(flags, query)) {
            matching_data.push_back(proxy_data);
        }
    }
    
    query_cache[query_ptr] = matching_data;
}

bool ORC_ProxyRegistry::set_flag(Ref<ORC_ProxyData> proxy_data, uint64_t flag_mask, bool value) {
    if (!proxy_data.is_valid()) return false;
    
    ORC_ProxyData* data_ptr = proxy_data.ptr();
    uint64_t old_flags = data_flags[data_ptr];
    uint64_t new_flags;
    
    if (value) {
        new_flags = old_flags | flag_mask;
    } else {
        new_flags = old_flags & ~flag_mask;
    }
    
    if (old_flags != new_flags) {
        data_flags[data_ptr] = new_flags;
        update_query_cache_for_data(proxy_data, old_flags, new_flags);
    }
    
    return true;
}

TypedArray<ORC_ProxyData> ORC_ProxyRegistry::get_by_query(Ref<ORC_FeatureQuery> query) {
    TypedArray<ORC_ProxyData> result;
    
    if (!query.is_valid()) return result;
    
    ORC_FeatureQuery* query_ptr = query.ptr();
    
    auto it = query_cache.find(query_ptr);
    if (it == query_cache.end()) {
        add_query_to_cache(query);
        it = query_cache.find(query_ptr);
    }
    
    if (it != query_cache.end()) {
        for (const auto& data : it->second) {
            result.append(data);
        }
    }
    
    return result;
}

}
