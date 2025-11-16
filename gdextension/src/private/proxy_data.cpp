#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <proxy_data.h>
#include <proxy_registry.h>

using namespace godot;

void ORC_ProxyData::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_flag", "flag_name", "value"), &ORC_ProxyData::set_flag);
}

bool ORC_ProxyData::set_flag(const StringName& flag_name, bool value) {
    if (!registry) {
        ERR_FAIL_V_MSG(false, "[ORC_ProxyData ERROR] : No registry associated with this ProxyData");
        return false;
    }
    
    return registry->set_flag_internal(this, flag_name, value);
}
