#ifndef ORC_PROXYCACHE_H
#define ORC_PROXYCACHE_H

#include <typeindex>
#include <vector>
#include <unordered_map>
#include <string>
#include <variant>

#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/gd_script.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <proxy_data.h>
#include "macros.h"

#ifdef ORC_RENDERER_EXPORTS
#define ORC_API __declspec(dllexport)
#else
#define ORC_API __declspec(dllimport)
#endif

namespace godot {

struct TypeKey {
    std::variant<std::type_index, std::string> key;
    
    TypeKey(std::type_index ti) : key(ti) {}
    TypeKey(const std::string& s) : key(s) {}
    TypeKey(const Ref<GDScript>& script) : key(script.is_valid() ? (String(script->get_global_name())).utf8().get_data() : "") {
        String global_name = script.is_valid() ? script->get_global_name() : String("");
        if (script.is_valid() && global_name.is_empty()) {
            ERR_FAIL_MSG("[ORC_ProxyCache ERROR] : Attempted to register an empty class_name as a cache key. This typically occurs when a GDScript class is not defined in its own file. Inner classes (classes defined within another class file) are not currently supported because Godot does not provide a unique identifier for them in this context.");
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

class ORC_API ORC_ProxyCache : public RefCounted {
    GDCLASS(ORC_ProxyCache, RefCounted)

    friend class ORC_SceneProxyBase;

private:
    std::unordered_map<TypeKey, std::vector<Ref<ORC_ProxyData>>, TypeKeyHash> type_cache;
    std::unordered_map<int64_t, std::tuple<Ref<ORC_ProxyData>, int>> id_cache;

    static TypeKey get_type_key(Ref<ORC_ProxyData> proxy_data);

protected:
    static void _bind_methods();

public:
    bool register_data(Ref<ORC_ProxyData> proxy_data, int64_t unique_id = -1);
    bool unregister_data(Ref<ORC_ProxyData> proxy_data);
    std::vector<Ref<ORC_ProxyData>> get_by_type(const TypeKey& type_key) const;
    Ref<ORC_ProxyData> get_by_unique_id(int64_t unique_id) const;
    bool increment_refcount(int64_t unique_id);
    bool decrement_refcount(int64_t unique_id);
};

}

#endif // ORC_PROXYCACHE_H
