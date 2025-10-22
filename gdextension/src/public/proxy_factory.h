#ifndef ORC_PROXYFACTORY_H
#define ORC_PROXYFACTORY_H

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/gd_script.hpp>
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
    Ref<ORC_ProxyObject> create_from(Node* node, Ref<ORC_ProxyCache> cache);
    DECLARE_GD_OVERRIDABLE_METHOD(Ref<ORC_ProxyObject>, create_proxy_from, Node*)
    DECLARE_GD_OVERRIDABLE_METHOD(Ref<ORC_PrimaryData>, create_data_from, Node*, Ref<ORC_ProxyCache>)

    Ref<ORC_ProxyData> free_data(Node* node, Ref<ORC_ProxyCache> cache);

    template <class T>
    static Ref<T> create_and_register(Ref<ORC_ProxyCache> cache, int64_t unique_id = -1) {
        static_assert(std::is_base_of<ORC_ProxyData, T>::value, "T must inherit from ORC_ProxyData");
        
        Ref<T> ref;
        if (unique_id != -1) ref = cache->get_by_unique_id(unique_id);
        if (ref.is_valid()) {
            cache->increment_refcount(unique_id);
            return ref;
        }

        ref.instantiate();
        cache->register_data(Ref<ORC_ProxyData>(ref), unique_id);
        
        return ref;
    }

    static Ref<ORC_ProxyData> create_and_register_gd(const Ref<GDScript> script, Ref<ORC_ProxyCache> cache, int64_t unique_id = -1);
};

}

#endif // ORC_PROXYFACTORY_H
