#include "omni_light_proxy.h"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

int ORCTEST_OmniLightProxy_CPP::omni_update_count = 0;

void ORCTEST_OmniLightProxy_CPP::_bind_methods() {
	ClassDB::bind_static_method("ORCTEST_OmniLightProxy_CPP", D_METHOD("get_omni_update_count"), &ORCTEST_OmniLightProxy_CPP::get_omni_update_count);
	ClassDB::bind_static_method("ORCTEST_OmniLightProxy_CPP", D_METHOD("reset_omni_update_count"), &ORCTEST_OmniLightProxy_CPP::reset_omni_update_count);
}

void ORCTEST_OmniLightProxy_CPP::update_impl() {
	ORCTEST_MockProxyObject_CPP::update_impl();
	omni_update_count++;
}
