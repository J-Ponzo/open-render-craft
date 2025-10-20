#include <proxy_factory.h>
#include <macros.h>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/gd_script.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot {

void ORC_ProxyFactory::_bind_methods() {
    BIND_GD_OVERRIDABLE_METHOD(ORC_ProxyFactory, create_from, "node", "cache")
    // ClassDB::bind_method(D_METHOD("create_from", "node", "cache"), &ORC_ProxyFactory::create_from);
    ClassDB::bind_method(D_METHOD("free_data", "node", "cache"), &ORC_ProxyFactory::free_data);
    
    ClassDB::bind_static_method("ORC_ProxyFactory", D_METHOD("create_and_register", "what", "cache"), &ORC_ProxyFactory::create_and_register_gd);
}

DEFINE_GD_OVERRIDABLE_METHOD_2_ARGS(ORC_ProxyFactory, Ref<ORC_ProxyObject>, create_from, Node*, node, Ref<ORC_ProxyCache>, cache)
Ref<ORC_ProxyObject> ORC_ProxyFactory::create_from(Node* node, Ref<ORC_ProxyCache> cache) {
    UtilityFunctions::print("ORC_ProxyFactory::create_from (stub)");
    return Ref<ORC_ProxyObject>();
}

Ref<ORC_ProxyData> ORC_ProxyFactory::free_data(Node* node, Ref<ORC_ProxyCache> cache) {
    UtilityFunctions::print("ORC_ProxyFactory::free_data (stub)");
    return Ref<ORC_ProxyData>();
}

static Ref<ORC_ProxyData> create_and_register_by_name(const String &class_name, Ref<ORC_ProxyCache> cache) {
    UtilityFunctions::print("ORC_ProxyFactory::create_and_register_by_name: ", class_name);
    if (!ClassDB::can_instantiate(class_name)) {
        UtilityFunctions::print("ClassDB cannot instantiate: ", class_name);
        return Ref<ORC_ProxyData>();
    }

    Variant v = ClassDB::instantiate(class_name);
    Object *obj = Object::cast_to<Object>(v);
    if (!obj) {
        UtilityFunctions::print("Instantiation returned null object for: ", class_name);
        return Ref<ORC_ProxyData>();
    }

    // Try to cast to ORC_ProxyData
    ORC_ProxyData *pdata = Object::cast_to<ORC_ProxyData>(obj);
    if (!pdata) {
        UtilityFunctions::print("Instantiated object is not ORC_ProxyData: ", class_name);
        return Ref<ORC_ProxyData>();
    }

    Ref<ORC_ProxyData> ref = Ref<ORC_ProxyData>(pdata);
    if (cache.is_valid()) {
        cache->register_data(ref);
    }

    return ref;
}

Ref<ORC_ProxyData> ORC_ProxyFactory::create_and_register_gd(const Variant &what, Ref<ORC_ProxyCache> cache) {
    // Variant-based convenience: accepts class name (String) or an already instantiated Object.
    if (what.get_type() == Variant::STRING) {
        String class_name = (String)what;
        return create_and_register_by_name(class_name, cache);
    }

    if (what.get_type() == Variant::OBJECT) {
        Object *obj = Object::cast_to<Object>(what);
        if (!obj) {
            UtilityFunctions::print("create_and_register: Variant OBJECT but cast failed");
            return Ref<ORC_ProxyData>();
        }

        ORC_ProxyData *pdata = Object::cast_to<ORC_ProxyData>(obj);
        if (!pdata) {
            UtilityFunctions::print("create_and_register: provided object is not ORC_ProxyData");
            return Ref<ORC_ProxyData>();
        }

        Ref<ORC_ProxyData> ref = Ref<ORC_ProxyData>(pdata);
        if (cache.is_valid()) {
            cache->register_data(ref);
        }
        return ref;
    }

    UtilityFunctions::print("create_and_register: unsupported Variant type");
    return Ref<ORC_ProxyData>();
}

}