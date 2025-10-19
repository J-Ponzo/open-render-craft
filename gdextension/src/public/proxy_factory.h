#ifndef ORC_PROXYFACTORY_H
#define ORC_PROXYFACTORY_H

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <proxy_object.h>
#include <proxy_data.h>
#include "proxy_cache.h"
#include <macros.h>

#ifdef ORC_RENDERER_EXPORTS
#define ORC_API __declspec(dllexport)
#else
#define ORC_API __declspec(dllimport)
#endif

namespace godot {

class ORC_API ORC_ProxyFactory : public RefCounted {
    GDCLASS(ORC_ProxyFactory, RefCounted)

protected:
    static void _bind_methods();

public:
    DECLARE_GD_OVERRIDABLE_METHOD(Ref<ORC_ProxyObject>, create_from, Node*, Ref<ORC_ProxyCache>)
    // Ref<ORC_ProxyObject> create_from(Node* node, Ref<ORC_ProxyCache> cache);
    Ref<ORC_ProxyData> free_data(Node* node, Ref<ORC_ProxyCache> cache);

    // Templated helper for C++ usage: instantiate a Ref<T> (where T : ORC_ProxyData),
    // register it into the provided cache and return the Ref<T>.
    template <class T>
    static Ref<T> create_and_register(Ref<ORC_ProxyCache> cache) {
        Ref<T> ref;
        ref.instantiate();
        if (cache.is_valid()) {
            // register as base type
            cache->register_data(Ref<ORC_ProxyData>(ref));
        }
        return ref;
    }

    // Exposed convenience method for GDScript: instantiate by class name and register
    // static Ref<ORC_ProxyData> create_and_register_by_name(const String &class_name, Ref<ORC_ProxyCache> cache);

    // Exposed convenience method for GDScript: instantiate from a GDScript resource and register
    // static Ref<ORC_ProxyData> create_and_register_from_script(Ref<GDScript> script, Ref<ORC_ProxyCache> cache);

    // Exposed convenience method for GDScript: accepts a Variant that can be either
    // - a String (class name), or
    // - an already-instantiated ORC_ProxyData instance
    // This avoids depending on Script::instantiate() at compile-time.
    static Ref<ORC_ProxyData> create_and_register_gd(const Variant &what, Ref<ORC_ProxyCache> cache);
};

}

#endif // ORC_PROXYFACTORY_H
