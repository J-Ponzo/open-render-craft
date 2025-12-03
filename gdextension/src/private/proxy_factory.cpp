#include <proxy_factory.h>
#include <macros.h>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/gd_script.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot {

static const char* ERR_SCRIPT_INVALID = "[ORC] Script is not valid.";
static const char* ERR_SCRIPT_INSTANTIATION_FAILED = "[ORC] Script instantiation failed.";
static const char* ERR_NOT_PRIMARY_DATA = "[ORC] Instantiated object is not ORC_PrimaryData.";
static const char* ERR_NOT_SECONDARY_DATA = "[ORC] Instantiated object is not ORC_SecondaryData.";
static const char* ERR_REGISTER_FAILED = "[ORC] Failed to register data in registry.";

void ORC_ProxyFactory::_bind_methods() {
    BIND_GD_OVERRIDABLE_METHOD(ORC_ProxyFactory, create_proxy_from, "node")
    BIND_GD_OVERRIDABLE_METHOD(ORC_ProxyFactory, create_data_from, "node", "registry")
    BIND_GD_OVERRIDABLE_METHOD(ORC_ProxyFactory, free_proxy, "proxy_object")
    BIND_GD_OVERRIDABLE_METHOD(ORC_ProxyFactory, free_data, "data", "registry")
    
    ClassDB::bind_static_method("ORC_ProxyFactory", D_METHOD("create_and_register_primary", "script", "registry", "unique_id"), &ORC_ProxyFactory::create_and_register_primary_gd, DEFVAL(-1));
    ClassDB::bind_static_method("ORC_ProxyFactory", D_METHOD("create_and_register_secondary", "script", "registry", "primary_data", "unique_id"), &ORC_ProxyFactory::create_and_register_secondary_gd, DEFVAL(-1));
    ClassDB::bind_static_method("ORC_ProxyFactory", D_METHOD("destroy_and_unregister_data", "data", "registry", "unique_id"), &ORC_ProxyFactory::destroy_and_unregister_data, DEFVAL(-1));
}

Ref<ORC_ProxyObject> ORC_ProxyFactory::create_from(Node* node, const Ref<ORC_ProxyRegistry>& registry) {
    Ref<ORC_ProxyObject> proxy_object = create_proxy_from(node);
    if (!proxy_object.is_valid()) return Ref<ORC_ProxyObject>();

    proxy_object->set_node(node);

    Ref<ORC_PrimaryData> primary_data = create_data_from(node, registry);
    if (!primary_data.is_valid()) return Ref<ORC_ProxyObject>();

    proxy_object->set_primary_data(primary_data);
    primary_data->set_proxy_object(proxy_object);

    return proxy_object;
}

DEFINE_GD_OVERRIDABLE_METHOD_1_ARGS(ORC_ProxyFactory, Ref<ORC_ProxyObject>, create_proxy_from, Node*, node)
Ref<ORC_ProxyObject> ORC_ProxyFactory::create_proxy_from_impl(Node* node) {
    return Ref<ORC_ProxyObject>();
}

DEFINE_GD_OVERRIDABLE_METHOD_2_ARGS(ORC_ProxyFactory, Ref<ORC_PrimaryData>, create_data_from, Node*, node, const Ref<ORC_ProxyRegistry>&, registry)
Ref<ORC_PrimaryData> ORC_ProxyFactory::create_data_from_impl(Node* node, const Ref<ORC_ProxyRegistry>& registry) {
    return Ref<ORC_ProxyObject>();
}

bool ORC_ProxyFactory::free(const Ref<ORC_ProxyObject>& proxy_object, const Ref<ORC_ProxyRegistry>& registry) {
    if (!proxy_object.is_valid()) return false;

    Ref<ORC_PrimaryData> primary = proxy_object->get_primary_data();

    bool success = true;
    for (Ref<ORC_SecondaryData> secondary : primary->secondary_data_array) {
        success &= free_data(secondary, registry);
    }
    success &= free_data(primary, registry);
    success &= free_proxy(proxy_object);

    for (Ref<ORC_SecondaryData> secondary : primary->secondary_data_array) {
        secondary->primary_data_array.erase(primary);
    }
    primary->secondary_data_array.clear();
    primary->proxy_object = Ref<ORC_ProxyObject>();
    proxy_object->set_primary_data(Ref<ORC_PrimaryData>());

    return success;
}

DEFINE_GD_OVERRIDABLE_METHOD_1_ARGS(ORC_ProxyFactory, bool, free_proxy, const Ref<ORC_ProxyObject>&, proxy_object)
bool ORC_ProxyFactory::free_proxy_impl(const Ref<ORC_ProxyObject>& proxy_object) {
    return false;
}

DEFINE_GD_OVERRIDABLE_METHOD_2_ARGS(ORC_ProxyFactory, bool, free_data, const Ref<ORC_ProxyData>&, data, const Ref<ORC_ProxyRegistry>&, registry)
bool ORC_ProxyFactory::free_data_impl(const Ref<ORC_ProxyData>& data, const Ref<ORC_ProxyRegistry>& registry) {
    return false;
}

// TODO : unify better with template versions
Ref<ORC_PrimaryData> ORC_ProxyFactory::create_and_register_primary_gd(const Ref<GDScript>& script, const Ref<ORC_ProxyRegistry>& registry, int64_t unique_id) {
    Ref<ORC_PrimaryData> ref;

    if (unique_id != -1) ref = registry->get_by_unique_id(unique_id);
    if (ref.is_valid()) {
        registry->increment_refcount(unique_id);
        return ref;
    }

    if (!script.is_valid()) ERR_FAIL_V_MSG(Ref<ORC_PrimaryData>(), ERR_SCRIPT_INVALID);
    
    Variant v = script->new_();
    Object *obj = Object::cast_to<Object>(v);
    if (!obj) ERR_FAIL_V_MSG(Ref<ORC_PrimaryData>(), ERR_SCRIPT_INSTANTIATION_FAILED);
    
    ORC_PrimaryData *pdata = Object::cast_to<ORC_PrimaryData>(obj);
    if (!pdata) ERR_FAIL_V_MSG(Ref<ORC_PrimaryData>(), ERR_NOT_PRIMARY_DATA);
    
    ref = Ref<ORC_PrimaryData>(pdata);
    ref->type_key = TypeKey(script);
    if (registry.is_valid()) {
        if (!registry->register_data(ref, unique_id)) WARN_PRINT(ERR_REGISTER_FAILED);
    }

    return ref;
}

// TODO : unify better with template versions
Ref<ORC_SecondaryData> ORC_ProxyFactory::create_and_register_secondary_gd(const Ref<GDScript>& script, const Ref<ORC_ProxyRegistry>& registry, const Ref<ORC_PrimaryData>& primary_data, int64_t unique_id) {
    Ref<ORC_SecondaryData> ref;

    if (unique_id != -1) ref = registry->get_by_unique_id(unique_id);
    if (ref.is_valid()) {
        registry->increment_refcount(unique_id);
    }
    else {  
        if (!script.is_valid()) ERR_FAIL_V_MSG(Ref<ORC_SecondaryData>(), ERR_SCRIPT_INVALID);
        
        Variant v = script->new_();
        Object *obj = Object::cast_to<Object>(v);
        if (!obj) ERR_FAIL_V_MSG(Ref<ORC_SecondaryData>(), ERR_SCRIPT_INSTANTIATION_FAILED);
        
        ORC_SecondaryData *sdata = Object::cast_to<ORC_SecondaryData>(obj);
        if (!sdata) ERR_FAIL_V_MSG(Ref<ORC_SecondaryData>(), ERR_NOT_SECONDARY_DATA);
        
        ref = Ref<ORC_SecondaryData>(sdata);
        ref->type_key = TypeKey(script);
        if (registry.is_valid()) {
            if (!registry->register_data(ref, unique_id)) WARN_PRINT(ERR_REGISTER_FAILED);
        }
    }

    primary_data->secondary_data_array.append(ref);
    ref->primary_data_array.append(primary_data);

    return ref;
}

bool ORC_ProxyFactory::destroy_and_unregister_data(const Ref<ORC_ProxyData>& data, const Ref<ORC_ProxyRegistry>& registry, int64_t unique_id) {
    if (!data.is_valid()) return false;

    Ref<ORC_ProxyData> ref;
    if (unique_id != -1) ref = registry->get_by_unique_id(unique_id);
    if (ref.is_valid()) {
        registry->decrement_refcount(unique_id);
        return true;
    }

    return registry->unregister_data(data);
}

}
