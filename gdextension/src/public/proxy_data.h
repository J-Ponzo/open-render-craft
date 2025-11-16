#ifndef ORC_PROXYDATA_H
#define ORC_PROXYDATA_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/string_name.hpp>

#ifdef ORC_RENDERER_EXPORTS
#define ORC_API __declspec(dllexport)
#else
#define ORC_API __declspec(dllimport)
#endif

namespace godot {

class ORC_ProxyRegistry;

class ORC_API ORC_ProxyData : public RefCounted {
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
