#ifndef ORC_PROXYDATA_H
#define ORC_PROXYDATA_H

#include <functional>
#include <typeindex>
#include <string>
#include <variant>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/gd_script.hpp>
#include <godot_cpp/variant/string_name.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include "macros.h"

namespace godot {

class ORC_ProxyRegistry;

class ORC_ProxyData;

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

struct ProxyDataHash {
    std::size_t operator()(const Ref<ORC_ProxyData>& ref) const {
        return std::hash<const ORC_ProxyData*>()(ref.ptr());
    }
};

class ORC_ProxyData : public RefCounted {
    GDCLASS(ORC_ProxyData, RefCounted)
    
    friend class ORC_ProxyRegistry;
    friend class ORC_ProxyFactory;

private:
    ORC_ProxyRegistry* registry = nullptr;
    TypeKey type_key = TypeKey(std::type_index(typeid(void)));

protected:
    static void _bind_methods();

public:
    bool set_flag(const StringName& flag_name, bool value);
    const TypeKey& get_type_key() const { return type_key; }

};

}

#endif
