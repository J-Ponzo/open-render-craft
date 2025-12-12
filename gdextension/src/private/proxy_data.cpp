#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <proxy_data.h>
#include <proxy_registry.h>

using namespace godot;

void ORC_ProxyData::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_flag", "flag_name", "value"), &ORC_ProxyData::set_flag);
    ClassDB::bind_method(D_METHOD("has_flag", "flag_name"), &ORC_ProxyData::has_flag);
    ClassDB::bind_method(D_METHOD("get_flags"), &ORC_ProxyData::get_flags);
    ClassDB::bind_method(D_METHOD("is_shared"), &ORC_ProxyData::is_shared);
    ClassDB::bind_method(D_METHOD("register_flag_sources", "sources"), &ORC_ProxyData::register_flag_sources);
    ClassDB::bind_method(D_METHOD("unregister_flag_sources", "sources"), &ORC_ProxyData::unregister_flag_sources);
}

bool ORC_ProxyData::set_flag(const StringName& flag_name, bool value) {
    DEV_ASSERT(registry != nullptr && "No registry associated with this ProxyData.");
    return registry->set_flag_internal(this, flag_name, value);
}

bool ORC_ProxyData::has_flag(const StringName& flag_name) {
    DEV_ASSERT(registry != nullptr && "No registry associated with this ProxyData.");
    return registry->has_flag_internal(this, flag_name);
}

TypedArray<StringName> ORC_ProxyData::get_flags() const {
    DEV_ASSERT(registry != nullptr && "No registry associated with this ProxyData.");
    return registry->get_flags_internal(const_cast<ORC_ProxyData*>(this));
}

void ORC_ProxyData::register_flag_sources(const TypedArray<ORC_ProxyData>& sources) {
    DEV_ASSERT(registry != nullptr && "No registry associated with this ProxyData.");
    registry->register_flag_sources_internal(this, sources);
}

void ORC_ProxyData::unregister_flag_sources(const TypedArray<ORC_ProxyData>& sources) {
    DEV_ASSERT(registry != nullptr && "No registry associated with this ProxyData.");
    registry->unregister_flag_sources_internal(this, sources);
}
