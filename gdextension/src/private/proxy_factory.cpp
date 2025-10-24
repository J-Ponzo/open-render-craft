#include <proxy_factory.h>
#include <macros.h>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/gd_script.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot {

void ORC_ProxyFactory::_bind_methods() {
    BIND_GD_OVERRIDABLE_METHOD(ORC_ProxyFactory, create_proxy_from, "node")
    BIND_GD_OVERRIDABLE_METHOD(ORC_ProxyFactory, create_data_from, "node", "cache")
    BIND_GD_OVERRIDABLE_METHOD(ORC_ProxyFactory, free_proxy, "proxy_object")
    BIND_GD_OVERRIDABLE_METHOD(ORC_ProxyFactory, free_data, "data", "cache")
    
    ClassDB::bind_static_method("ORC_ProxyFactory", D_METHOD("create_and_register_primary", "script", "cache", "unique_id"), &ORC_ProxyFactory::create_and_register_primary_gd, DEFVAL(-1));
    ClassDB::bind_static_method("ORC_ProxyFactory", D_METHOD("create_and_register_secondary", "script", "cache", "primary_data", "unique_id"), &ORC_ProxyFactory::create_and_register_secondary_gd, DEFVAL(-1));
    ClassDB::bind_static_method("ORC_ProxyFactory", D_METHOD("destroy_and_unregister_data", "data", "cache", "unique_id"), &ORC_ProxyFactory::destroy_and_unregister_data_gd, DEFVAL(-1));
}

Ref<ORC_ProxyObject> ORC_ProxyFactory::create_from(Node* node, Ref<ORC_ProxyCache> cache) {
    Ref<ORC_ProxyObject> proxy_object = create_proxy_from(node);
    Ref<ORC_PrimaryData> primary_data = create_data_from(node, cache);
    if (proxy_object.is_valid() && primary_data.is_valid()) {
        proxy_object->set_node(node);
        proxy_object->set_primary_data(primary_data);
        primary_data->set_proxy_object(proxy_object);
    }
    return proxy_object;
}

DEFINE_GD_OVERRIDABLE_METHOD_1_ARGS(ORC_ProxyFactory, Ref<ORC_ProxyObject>, create_proxy_from, Node*, node)
Ref<ORC_ProxyObject> ORC_ProxyFactory::create_proxy_from_impl(Node* node) {
    UtilityFunctions::print("ORC_ProxyFactory::create_proxy_from (stub)");
    return Ref<ORC_ProxyObject>();
}

DEFINE_GD_OVERRIDABLE_METHOD_2_ARGS(ORC_ProxyFactory, Ref<ORC_PrimaryData>, create_data_from, Node*, node, Ref<ORC_ProxyCache>, cache)
Ref<ORC_PrimaryData> ORC_ProxyFactory::create_data_from_impl(Node* node, Ref<ORC_ProxyCache> cache) {
    UtilityFunctions::print("ORC_ProxyFactory::create_data_from (stub)");
    return Ref<ORC_ProxyObject>();
}

bool ORC_ProxyFactory::free(Ref<ORC_ProxyObject> proxy_object, Ref<ORC_ProxyCache> cache) {
    if (!proxy_object.is_valid()) return false;

    Ref<ORC_PrimaryData> primary = proxy_object->get_primary_data();

    bool success = true;
    for (Ref<ORC_SecondaryData> secondary : primary->secondary_data_array) {
        success &= free_data(secondary, cache);
    }
    success &= free_data(primary, cache);
    success &= free_proxy(proxy_object);

    for (Ref<ORC_SecondaryData> secondary : primary->secondary_data_array) {
        secondary->primary_data_array.erase(primary);
    }
    primary->secondary_data_array.clear();
    primary->proxy_object = Ref<ORC_ProxyObject>();
    proxy_object->set_primary_data(Ref<ORC_PrimaryData>());

    return success;
}

DEFINE_GD_OVERRIDABLE_METHOD_1_ARGS(ORC_ProxyFactory, bool, free_proxy, Ref<ORC_ProxyObject>, proxy_object)
bool ORC_ProxyFactory::free_proxy_impl(Ref<ORC_ProxyObject> proxy_object) {
    UtilityFunctions::print("ORC_ProxyFactory::free_proxy (stub)");
    return false;
}

DEFINE_GD_OVERRIDABLE_METHOD_2_ARGS(ORC_ProxyFactory, bool, free_data, Ref<ORC_ProxyData>, data, Ref<ORC_ProxyCache>, cache)
bool ORC_ProxyFactory::free_data_impl(Ref<ORC_ProxyData> data, Ref<ORC_ProxyCache> cache) {
    UtilityFunctions::print("ORC_ProxyFactory::free_data (stub)");
    return false;
}

Ref<ORC_PrimaryData> ORC_ProxyFactory::create_and_register_primary_gd(const Ref<GDScript> script, Ref<ORC_ProxyCache> cache, int64_t unique_id) {
    Ref<ORC_PrimaryData> ref;

    if (unique_id != -1) ref = cache->get_by_unique_id(unique_id);
    if (ref.is_valid()) {
        cache->increment_refcount(unique_id);
        return ref;
    }

    if (!script.is_valid()) {
        UtilityFunctions::print("create_and_register_gd: script is not valid");
        return Ref<ORC_PrimaryData>();
    }
    Variant v = script->new_();
    Object *obj = Object::cast_to<Object>(v);
    if (!obj) {
        UtilityFunctions::print("create_and_register_gd: script instantiation failed");
        return Ref<ORC_PrimaryData>();
    }
    ORC_PrimaryData *pdata = Object::cast_to<ORC_PrimaryData>(obj);
    if (!pdata) {
        UtilityFunctions::print("create_and_register_gd: instantiated object is not ORC_PrimaryData");
        return Ref<ORC_PrimaryData>();
    }
    ref = Ref<ORC_PrimaryData>(pdata);
    if (cache.is_valid()) {
        if (!cache->register_data(ref, unique_id)) {
            UtilityFunctions::print("create_and_register_gd: failed to register data in cache");
        }
    }

    return ref;
}

Ref<ORC_SecondaryData> ORC_ProxyFactory::create_and_register_secondary_gd(const Ref<GDScript> script, Ref<ORC_ProxyCache> cache, Ref<ORC_PrimaryData> primary_data, int64_t unique_id) {
    Ref<ORC_SecondaryData> ref;

    if (unique_id != -1) ref = cache->get_by_unique_id(unique_id);
    if (ref.is_valid()) {
        cache->increment_refcount(unique_id);
    }
    else {  
        if (!script.is_valid()) {
            UtilityFunctions::print("create_and_register_gd: script is not valid");
            return Ref<ORC_SecondaryData>();
        }
        Variant v = script->new_();
        Object *obj = Object::cast_to<Object>(v);
        if (!obj) {
            UtilityFunctions::print("create_and_register_gd: script instantiation failed");
            return Ref<ORC_SecondaryData>();
        }
        ORC_SecondaryData *sdata = Object::cast_to<ORC_SecondaryData>(obj);
        if (!sdata) {
            UtilityFunctions::print("create_and_register_gd: instantiated object is not ORC_SecondaryData");
            return Ref<ORC_SecondaryData>();
        }
        ref = Ref<ORC_SecondaryData>(sdata);
        if (cache.is_valid()) {
            if (!cache->register_data(ref, unique_id)) {
                UtilityFunctions::print("create_and_register_gd: failed to register data in cache");
            }
        }
    }

    primary_data->secondary_data_array.append(ref);
    ref->primary_data_array.append(primary_data);

    return ref;
}

bool ORC_ProxyFactory::destroy_and_unregister_data_gd(Ref<ORC_ProxyData> data, Ref<ORC_ProxyCache> cache, int64_t unique_id) {
    if (!data.is_valid()) return false;

    Ref<ORC_ProxyData> ref;
    if (unique_id != -1) ref = cache->get_by_unique_id(unique_id);
    if (ref.is_valid()) {
        cache->decrement_refcount(unique_id);
        return true;
    }

    return cache->unregister_data(data);
}

}