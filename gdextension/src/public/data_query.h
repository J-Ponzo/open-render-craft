#ifndef ORC_DATAQUERY_H
#define ORC_DATAQUERY_H

#include <typeindex>
#include <string>
#include <variant>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/gd_script.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include "macros.h"

#ifdef ORC_RENDERER_EXPORTS
#define ORC_API __declspec(dllexport)
#else
#define ORC_API __declspec(dllimport)
#endif

namespace godot {

// TODO extract this from here. It was in proxy_registry before but it caused circular dependency
struct TypeKey {
    std::variant<std::type_index, std::string> key;
    
    TypeKey(std::type_index ti) : key(ti) {}
    TypeKey(const std::string& s) : key(s) {}
    TypeKey(const Ref<GDScript>& script) : key(script.is_valid() ? (String(script->get_global_name())).utf8().get_data() : "") {
        String global_name = script.is_valid() ? script->get_global_name() : String("");
        if (script.is_valid() && global_name.is_empty()) {
            // TODO : verifier chaque ERR_FAIL_MSG
            ERR_FAIL_MSG("[ORC_ProxyRegistry ERROR] : Attempted to register an empty class_name as a cache key. This typically occurs when a GDScript class is not defined in its own file. Inner classes (classes defined within another class file) are not currently supported because Godot does not provide a unique identifier for them in this context.");
        }
    }
    
    bool operator==(const TypeKey& other) const {
        return key == other.key;
    }
};

struct TypeKeyHash {
    size_t operator()(const TypeKey& tk) const {
        if (std::holds_alternative<std::type_index>(tk.key)) {
            return std::get<std::type_index>(tk.key).hash_code();
        } else {
            return std::hash<std::string>{}(std::get<std::string>(tk.key));
        }
    }
};

class ORC_API ORC_DataQuery : public RefCounted {
    GDCLASS(ORC_DataQuery, RefCounted)

protected:
    static void _bind_methods();

public:
    TypeKey type_key;
    uint64_t mask;
    uint64_t value;

    ORC_DataQuery() : type_key(std::type_index(typeid(void))), mask(0), value(0) {}
};

struct DataQueryHash {
    size_t operator()(const Ref<ORC_DataQuery>& query) const {
        if (!query.is_valid()) return 0;
        
        size_t h1 = TypeKeyHash{}(query->type_key);
        size_t h2 = std::hash<uint64_t>{}(query->mask);
        size_t h3 = std::hash<uint64_t>{}(query->value);
        
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

}

#endif // ORC_DATAQUERY_H
