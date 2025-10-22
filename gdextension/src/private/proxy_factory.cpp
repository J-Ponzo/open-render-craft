#include <proxy_factory.h>
#include <macros.h>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/gd_script.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot {

void ORC_ProxyFactory::_bind_methods() {
    BIND_GD_OVERRIDABLE_METHOD(ORC_ProxyFactory, create_proxy_from, "node")
    BIND_GD_OVERRIDABLE_METHOD(ORC_ProxyFactory, create_data_from, "node", "cache")
    ClassDB::bind_method(D_METHOD("free_data", "node", "cache"), &ORC_ProxyFactory::free_data);
    
    ClassDB::bind_static_method("ORC_ProxyFactory", D_METHOD("create_and_register", "script", "cache", "unique_id"), &ORC_ProxyFactory::create_and_register_gd, DEFVAL(-1));
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

Ref<ORC_ProxyData> ORC_ProxyFactory::free_data(Node* node, Ref<ORC_ProxyCache> cache) {
    UtilityFunctions::print("ORC_ProxyFactory::free_data (stub)");
    return Ref<ORC_ProxyData>();
}

Ref<ORC_ProxyData> ORC_ProxyFactory::create_and_register_gd(const Ref<GDScript> script, Ref<ORC_ProxyCache> cache, int64_t unique_id) {
    Ref<ORC_ProxyData> ref;
    if (unique_id != -1) ref = cache->get_by_unique_id(unique_id);
    if (ref.is_valid()) {
        cache->increment_refcount(unique_id);
        return ref;
    }

    if (!script.is_valid()) {
        UtilityFunctions::print("create_and_register_gd: script is not valid");
        return Ref<ORC_ProxyData>();
    }
    Variant v = script->new_();
    Object *obj = Object::cast_to<Object>(v);
    if (!obj) {
        UtilityFunctions::print("create_and_register_gd: script instantiation failed");
        return Ref<ORC_ProxyData>();
    }
    ORC_ProxyData *pdata = Object::cast_to<ORC_ProxyData>(obj);
    if (!pdata) {
        UtilityFunctions::print("create_and_register_gd: instantiated object is not ORC_ProxyData");
        return Ref<ORC_ProxyData>();
    }
    ref = Ref<ORC_ProxyData>(pdata);
    if (cache.is_valid()) {
        cache->register_data(ref);
    }
    return ref;
}

}