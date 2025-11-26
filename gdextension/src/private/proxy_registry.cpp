#include <proxy_registry.h>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <primary_data.h>
#include <secondary_data.h>

namespace godot {

std::unordered_map<StringName, std::type_index>& ORC_ProxyRegistry::cpp_types() {
    static std::unordered_map<StringName, std::type_index> registry;
    return registry;
}

std::type_index ORC_ProxyRegistry::get_cpp_type_index(const StringName& class_name) {
    auto& registry = cpp_types();
    auto it = registry.find(class_name);
    ERR_FAIL_COND_V_MSG(it == registry.end(), typeid(void), 
        vformat("[ORC_ProxyRegistry ERROR] : C++ type '%s' not registered. Call ORC_ProxyRegistry::register_cpp_type<YourType>(\"%s\") in your module initialization.", 
        class_name, class_name));
    return it->second;
}

void ORC_ProxyRegistry::_bind_methods() {
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

    proxy_data->registry = this;

    if (unique_id != -1) {
        id_registry[unique_id] = std::make_tuple(proxy_data, 1);
    }

    TypeKey type_key = get_type_key(proxy_data);
    type_registry[type_key].push_back(proxy_data);

    uint64_t flags = 0;
    auto flags_it = data_flags.find(proxy_data.ptr());
    if (flags_it != data_flags.end()) {
        flags = flags_it->second;
    }
    
    for (auto& cache_entry : query_cache) {
        const Ref<ORC_DataQuery>& query = cache_entry.first;
        if (!query.is_valid()) continue;
        
        if (matches_query(proxy_data, flags, query)) {
            cache_entry.second.push_back(proxy_data);
        }
    }

    return true;
}

bool ORC_ProxyRegistry::unregister_data(Ref<ORC_ProxyData> proxy_data) {
    if (!proxy_data.is_valid()) return false;

    proxy_data->registry = nullptr;

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

bool ORC_ProxyRegistry::matches_query(Ref<ORC_ProxyData> proxy_data, uint64_t flags, const Ref<ORC_DataQuery>& query) const {
    if (!query.is_valid() || !proxy_data.is_valid()) return false;
    
    TypeKey data_type_key = get_type_key(proxy_data);
    if (!(data_type_key == query->type_key)) return false;
    
    return (flags & query->mask) == (query->value & query->mask);
}

bool ORC_ProxyRegistry::update_query_cache_for_data(Ref<ORC_ProxyData> proxy_data, uint64_t old_flags, uint64_t new_flags) {
    if (!proxy_data.is_valid()) return false;
    
    for (auto& cache_entry : query_cache) {
        const Ref<ORC_DataQuery>& query = cache_entry.first;
        if (!query.is_valid()) continue;
        
        std::vector<Ref<ORC_ProxyData>>& data_list = cache_entry.second;
        
        bool old_match = matches_query(proxy_data, old_flags, query);
        bool new_match = matches_query(proxy_data, new_flags, query);
        
        if (old_match && !new_match) {
            data_list.erase(std::remove(data_list.begin(), data_list.end(), proxy_data), data_list.end());
        } else if (!old_match && new_match) {
            data_list.push_back(proxy_data);
        }
    }
    return true;
}

bool ORC_ProxyRegistry::remove_from_query_cache(Ref<ORC_ProxyData> proxy_data) {
    if (!proxy_data.is_valid()) return false;
    
    for (auto& cache_entry : query_cache) {
        std::vector<Ref<ORC_ProxyData>>& data_list = cache_entry.second;
        data_list.erase(std::remove(data_list.begin(), data_list.end(), proxy_data), data_list.end());
    }
    return true;
}

bool ORC_ProxyRegistry::add_query_to_cache(const Ref<ORC_DataQuery>& query) {
    if (!query.is_valid()) return false;
    
    if (query_cache.find(query) != query_cache.end()) return false;
    
    std::vector<Ref<ORC_ProxyData>> matching_data;
    
    for (const auto& type_entry : type_registry) {
        for (const auto& proxy_data : type_entry.second) {
            if (!proxy_data.is_valid()) continue;
            
            uint64_t flags = 0;
            auto flags_it = data_flags.find(proxy_data.ptr());
            if (flags_it != data_flags.end()) {
                flags = flags_it->second;
            }
            
            if (matches_query(proxy_data, flags, query)) {
                matching_data.push_back(proxy_data);
            }
        }
    }
    
    query_cache[query] = matching_data;
    return true;
}

uint64_t ORC_ProxyRegistry::get_or_create_flag_mask(const StringName& flag_name) {
    auto it = flag_name_to_mask.find(flag_name);
    
    if (it != flag_name_to_mask.end()) {
        return it->second;
    }
    
    if (next_available_bit >= 64) {
        ERR_FAIL_V_MSG(0, "[ORC_ProxyRegistry ERROR] : Maximum number of flags (64) reached. Cannot create new flag: " + String(flag_name));
    }
    
    uint64_t flag_mask = 1ULL << next_available_bit;
    flag_name_to_mask[flag_name] = flag_mask;
    next_available_bit++;
    
    return flag_mask;
}

bool ORC_ProxyRegistry::set_flag_internal(ORC_ProxyData* proxy_data, const StringName& flag_name, bool value) {
    if (!proxy_data) {
        ERR_FAIL_V_MSG(false, "[ORC_ProxyRegistry ERROR] : Cannot set flag on null proxy_data");
    }
    
    uint64_t flag_mask = get_or_create_flag_mask(flag_name);
    
    uint64_t old_flags = data_flags[proxy_data];
    uint64_t new_flags = value ? (old_flags | flag_mask) : (old_flags & ~flag_mask);
    
    if (old_flags == new_flags) return false;

    data_flags[proxy_data] = new_flags;
    
    Ref<ORC_ProxyData> proxy_ref;
    proxy_ref.reference_ptr(proxy_data);
    update_query_cache_for_data(proxy_ref, old_flags, new_flags);
    
    return true;
}

TypedArray<ORC_ProxyData> ORC_ProxyRegistry::get_by_query(Ref<ORC_DataQuery> query) {
    TypedArray<ORC_ProxyData> result;
    
    if (!query.is_valid()) return result;
    
    auto it = query_cache.find(query);
    if (it == query_cache.end()) {
        add_query_to_cache(query);
        it = query_cache.find(query);
    }
    
    if (it != query_cache.end()) {
        for (const auto& data : it->second) {
            result.append(data);
        }
    }
    
    return result;
}

bool ORC_ProxyRegistry::fill_query_features(Ref<ORC_DataQuery> query, const TypedArray<StringName>& flag_names, const TypedArray<bool>& flag_values) {
    if (!query.is_valid()) {
        ERR_FAIL_V_MSG(false, "[ORC_ProxyRegistry ERROR] : query is not valid");
        return false;
    }
    
    if (flag_names.size() != flag_values.size()) {
        ERR_FAIL_V_MSG(false, "[ORC_ProxyRegistry ERROR] : flag_names and flag_values arrays must have the same size");
        return false;
    }
    
    uint64_t mask = 0;
    uint64_t value = 0;
    
    for (int i = 0; i < flag_names.size(); i++) {
        StringName flag_name = flag_names[i];
        bool flag_value = flag_values[i];
        
        uint64_t flag_mask = get_or_create_flag_mask(flag_name);
        mask |= flag_mask;
        
        if (flag_value) {
            value |= flag_mask;
        }
    }
    
    query->mask = mask;
    query->value = value;
    
    return true;
}

Ref<ORC_DataQuery> ORC_ProxyRegistry::create_query_gd(Ref<GDScript> script, const TypedArray<StringName> &flag_names, const TypedArray<bool> &flag_values)
{
    if (!script.is_valid()) {
        ERR_FAIL_V_MSG(Ref<ORC_DataQuery>(), "[ORC_ProxyRegistry ERROR] : Cannot create query with null GDScript");
        return nullptr;
    }
    
    Ref<ORC_DataQuery> query;
    query.instantiate();
    fill_query_features(query, flag_names, flag_values);
    query->type_key = TypeKey(script);
    return query;
}

// TODO make unit test
Ref<ORC_DataQuery> ORC_ProxyRegistry::create_query(std::type_index type_id, const TypedArray<StringName> &flag_names, const TypedArray<bool> &flag_values)
{
    if (type_id == typeid(void)) {
        ERR_FAIL_V_MSG(Ref<ORC_DataQuery>(), "[ORC_ProxyRegistry ERROR] : Cannot create query with void type_index");
        return nullptr;
    }
    
    Ref<ORC_DataQuery> query;
    query.instantiate();
    fill_query_features(query, flag_names, flag_values);
    query->type_key = TypeKey(type_id);
    return query;
}

void ORC_ProxyRegistry::clear() {
	type_registry.clear();
	id_registry.clear();
	data_flags.clear();
	query_cache.clear();
	flag_name_to_mask.clear();
	next_available_bit = 0;
}}
