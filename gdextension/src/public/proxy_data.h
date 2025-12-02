#ifndef ORC_PROXYDATA_H
#define ORC_PROXYDATA_H

#include <functional>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/string_name.hpp>

namespace godot {

class ORC_ProxyRegistry;

class ORC_ProxyData;

struct ProxyDataHash {
    std::size_t operator()(const Ref<ORC_ProxyData>& ref) const {
        return std::hash<const ORC_ProxyData*>()(ref.ptr());
    }
};

class ORC_ProxyData : public RefCounted {
    GDCLASS(ORC_ProxyData, RefCounted)
    
    friend class ORC_ProxyRegistry;

private:
    ORC_ProxyRegistry* registry = nullptr;

protected:
    static void _bind_methods();

public:
    bool set_flag(const StringName& flag_name, bool value);

};

}

#endif
