#include <proxy_cache.h>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot {

void ORC_ProxyCache::_bind_methods() {
    ClassDB::bind_method(D_METHOD("register_data", "proxy_data"), &ORC_ProxyCache::register_data);
    ClassDB::bind_method(D_METHOD("unregister_data", "proxy_data"), &ORC_ProxyCache::unregister_data);
}

void ORC_ProxyCache::register_data(Ref<ORC_ProxyData> proxy_data, int64_t unique_id) {
    UtilityFunctions::print("ORC_ProxyCache::register_data (stub)");
}

void ORC_ProxyCache::unregister_data(Ref<ORC_ProxyData> proxy_data, int64_t unique_id) {
    UtilityFunctions::print("ORC_ProxyCache::unregister_data (stub)");
}

Ref<ORC_ProxyData> ORC_ProxyCache::get_by_type(std::type_index type) const {
    UtilityFunctions::print("ORC_ProxyCache::get_by_type (stub)");
    return Ref<ORC_ProxyData>();
}

Ref<ORC_ProxyData> ORC_ProxyCache::get_by_unique_id(int64_t unique_id) const {
    UtilityFunctions::print("ORC_ProxyCache::get_by_unique_id (stub)");
    return Ref<ORC_ProxyData>();
}

bool ORC_ProxyCache::increment_refcount(int64_t unique_id) {
    UtilityFunctions::print("ORC_ProxyCache::increment_refcount (stub)");
    return false;
}

bool ORC_ProxyCache::decrement_refcount(int64_t unique_id) {
    UtilityFunctions::print("ORC_ProxyCache::decrement_refcount (stub)");
    return false;
}

}
