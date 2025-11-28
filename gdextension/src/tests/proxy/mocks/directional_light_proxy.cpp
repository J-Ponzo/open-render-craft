#include "directional_light_proxy.h"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

int ORCTEST_DirectionalLightProxy_CPP::directional_update_count = 0;

void ORCTEST_DirectionalLightProxy_CPP::_bind_methods() {
	ClassDB::bind_static_method("ORCTEST_DirectionalLightProxy_CPP", D_METHOD("get_directional_update_count"), &ORCTEST_DirectionalLightProxy_CPP::get_directional_update_count);
	ClassDB::bind_static_method("ORCTEST_DirectionalLightProxy_CPP", D_METHOD("reset_directional_update_count"), &ORCTEST_DirectionalLightProxy_CPP::reset_directional_update_count);
}

void ORCTEST_DirectionalLightProxy_CPP::update_impl() {
	ORCTEST_MockProxyObject_CPP::update_impl();
	directional_update_count++;
}
