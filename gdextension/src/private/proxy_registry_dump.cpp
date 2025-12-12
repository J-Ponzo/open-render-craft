#include <proxy_registry_dump.h>
#include <godot_cpp/core/class_db.hpp>
#include <primary_data.h>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/gd_script.hpp>

using namespace godot;

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

static String get_type_name(const Object* obj) {
    if (!obj) return "<null>";
    
    Ref<GDScript> script = obj->get_script();
    if (script.is_valid() && !script->get_global_name().is_empty()) {
        return script->get_global_name();
    } else {
        return obj->get_class();
    }
}

template <typename T>
static String get_type_and_address(const Ref<T>& ref) {
    if (!ref.is_valid()) return "<invalid>";
    
    String result = get_type_name(ref);
    
    Node* node = Object::cast_to<Node>(ref.ptr());
    if (node) {
        result += "[\"" + node->get_name() + "\"]";
    }
    
    result += "@" + String::num_int64((int64_t)ref.ptr());
    return result;
}

static String get_type_and_address(const Object* obj) {
    if (!obj) return "<null>";
    
    String result = get_type_name(obj);
    
    const Node* node = Object::cast_to<Node>(obj);
    if (node) {
        result += "[\"" + node->get_name() + "\"]";
    }
    
    result += "@" + String::num_int64((int64_t)obj);
    return result;
}

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

void ORC_ProxyRegistryDump::_bind_methods() {
    ClassDB::bind_method(D_METHOD("to_string"), &ORC_ProxyRegistryDump::to_string);
}

ORC_ProxyRegistryDump::ORC_ProxyRegistryDump() : next_available_bit(0) {
}

ORC_ProxyRegistryDump::~ORC_ProxyRegistryDump() {
}

String ORC_ProxyRegistryDump::to_string() const {
    String output = "=== ORC_ProxyRegistry Dump ===\n";
    
    auto to_binary = [this](uint64_t value) -> String {
        if (next_available_bit == 0) return "0";
        String result = "";
        for (int i = next_available_bit - 1; i >= 0; i--) {
            result += ((value >> i) & 1) ? "1" : "0";
        }
        return result;
    };
    
    output += "\n--- Flag Names ---\n";
    output += "Total flags: " + String::num_int64(flag_mask_lookup.size()) + "\n";
    
    std::unordered_map<uint8_t, StringName> bit_to_flag;
    for (const auto& pair : flag_mask_lookup) {
        uint64_t mask = pair.second;
        for (uint8_t bit = 0; bit < 64; bit++) {
            if (mask == (1ULL << bit)) {
                bit_to_flag[bit] = pair.first;
                break;
            }
        }
    }
    
    for (int bit = next_available_bit - 1; bit >= 0; bit--) {
        auto it = bit_to_flag.find(bit);
        if (it != bit_to_flag.end()) {
            output += "  Bit " + String::num_int64(bit) + ": " + String(it->second) + "\n";
        }
    }
    
    output += "\n--- Query Cache ---\n";
    output += "Total queries: " + String::num_int64(query_cache.size()) + "\n";
    
    int query_idx = 0;
    for (const auto& cache_entry : query_cache) {
        const Ref<ORC_DataQuery>& query = cache_entry.first;
        const std::vector<Ref<ORC_ProxyData>>& data_list = cache_entry.second;
        
        output += "\n  [Query #" + String::num_int64(query_idx++) + "] ";
        
        if (!query.is_valid()) {
            output += "<invalid query>\n";
            continue;
        }
        
        if (std::holds_alternative<std::type_index>(query->type_key.key)) {
            output += "[C++] " + String(std::get<std::type_index>(query->type_key.key).name());
        } else {
            output += "[GD] " + String(std::get<std::string>(query->type_key.key).c_str());
        }
        
        output += " (mask: 0b" + to_binary(query->mask) + ", value: 0b" + to_binary(query->value) + ")";
        output += "\n    -> " + String::num_int64(data_list.size()) + " matching data:\n";
        
        for (size_t i = 0; i < data_list.size(); i++) {
            const auto& data = data_list[i];
            output += "      [" + String::num_int64(i) + "] ";
            
            if (data.is_valid()) {
                output += get_type_and_address(data);
                
                ORC_PrimaryData* primary = Object::cast_to<ORC_PrimaryData>(data.ptr());
                if (primary) {
                    output += get_primary_node_info(primary);
                }
                
                output += "\n";
            } else {
                output += "<invalid>\n";
            }
        }
    }
    
    output += "\n--- ID Registry ---\n";
    output += "Total unique IDs: " + String::num_int64(id_lookup.size()) + "\n";
    for (const auto& pair : id_lookup) {
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
    
    output += "\n--- Cascade Sources ---\n";
    output += "Total entries: " + String::num_int64(cascade_sources.size()) + "\n";
    for (const auto& pair : cascade_sources) {
        const Ref<ORC_ProxyData>& target = pair.first;
        const std::vector<Ref<ORC_ProxyData>>& sources = pair.second;
        output += "  " + get_type_and_address(target) + " <- [";
        for (size_t i = 0; i < sources.size(); i++) {
            if (i > 0) output += ", ";
            output += get_type_and_address(sources[i]);
        }
        output += "]\n";
    }
    
    output += "\n--- Cascade Targets ---\n";
    output += "Total entries: " + String::num_int64(cascade_targets.size()) + "\n";
    for (const auto& pair : cascade_targets) {
        const Ref<ORC_ProxyData>& source = pair.first;
        const std::vector<Ref<ORC_ProxyData>>& targets = pair.second;
        output += "  " + get_type_and_address(source) + " -> [";
        for (size_t i = 0; i < targets.size(); i++) {
            if (i > 0) output += ", ";
            output += get_type_and_address(targets[i]);
        }
        output += "]\n";
    }
    
    output += "\n=========================\n";
    return output;
}
