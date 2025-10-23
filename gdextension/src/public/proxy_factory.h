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

    bool free(Ref<ORC_ProxyObject> proxy_object, Ref<ORC_ProxyCache> cache);
    DECLARE_GD_OVERRIDABLE_METHOD(bool, free_proxy, Ref<ORC_ProxyObject>)
    DECLARE_GD_OVERRIDABLE_METHOD(bool, free_data, Ref<ORC_ProxyData>, Ref<ORC_ProxyCache>)

    template <class T>
    static Ref<T> create_and_register_primary(Ref<ORC_ProxyCache> cache, int64_t unique_id = -1) {
        static_assert(std::is_base_of<ORC_PrimaryData, T>::value, "T must inherit from ORC_PrimaryData");
        
        Ref<T> ref;
        if (unique_id != -1) ref = cache->get_by_unique_id(unique_id);
        if (ref.is_valid()) {
            cache->increment_refcount(unique_id);
            return ref;
        }

        ref.instantiate();
        cache->register_data(ref, unique_id);
        
        return ref;
    }

    template <class T>
    static Ref<T> create_and_register_secondary(Ref<ORC_ProxyCache> cache, Ref<ORC_PrimaryData> primary_data, int64_t unique_id = -1) {
        static_assert(std::is_base_of<ORC_SecondaryData, T>::value, "T must inherit from ORC_SecondaryData");

        Ref<T> ref;
        if (unique_id != -1) ref = cache->get_by_unique_id(unique_id);
        if (ref.is_valid()) {
            cache->increment_refcount(unique_id);
            return ref;
        }
        else {
            ref.instantiate();
            cache->register_data(ref, unique_id);
        }
        
        primary_data->secondary_data_array.append(ref);
        ref->primary_data_array.append(primary_data);
        return ref;
    }

    static Ref<ORC_PrimaryData> create_and_register_primary_gd(const Ref<GDScript> script, Ref<ORC_ProxyCache> cache, int64_t unique_id = -1);
    static Ref<ORC_SecondaryData> create_and_register_secondary_gd(const Ref<GDScript> script, Ref<ORC_ProxyCache> cache, Ref<ORC_PrimaryData> primary_data, int64_t unique_id = -1);
    static bool destroy_and_unregister_data_gd(Ref<ORC_ProxyData> data, Ref<ORC_ProxyCache> cache, int64_t unique_id = -1);
};

}

#endif // ORC_PROXYFACTORY_H
