#include "spot_light_proxy.h"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

int ORCTEST_SpotLightProxy_CPP::spot_update_count = 0;

void ORCTEST_SpotLightProxy_CPP::_bind_methods() {
	ClassDB::bind_static_method("ORCTEST_SpotLightProxy_CPP", D_METHOD("get_spot_update_count"), &ORCTEST_SpotLightProxy_CPP::get_spot_update_count);
	ClassDB::bind_static_method("ORCTEST_SpotLightProxy_CPP", D_METHOD("reset_spot_update_count"), &ORCTEST_SpotLightProxy_CPP::reset_spot_update_count);
}

void ORCTEST_SpotLightProxy_CPP::update_impl() {
	ORCTEST_MockProxyObject_CPP::update_impl();
	spot_update_count++;
}
