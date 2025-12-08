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
#include <godot_cpp/variant/typed_array.hpp>
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
        if (script.is_valid() && global_name.is_empty()) ERR_FAIL_MSG("Empty class_name as cache key. Inner classes are not supported.");
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
    // TODO : use Ref<> ?
    ORC_ProxyRegistry* registry = nullptr;
    TypeKey type_key = TypeKey(std::type_index(typeid(void)));
    bool shared = false;    // TODO Test this

protected:
    static void _bind_methods();

public:
    bool set_flag(const StringName& flag_name, bool value);
    bool has_flag(const StringName& flag_name);
    TypedArray<StringName> get_flags() const;
    const TypeKey& get_type_key() const { return type_key; }
    bool is_shared() const { return shared; }

};

}

#endif
