#include "mock_proxy_object.h"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

int ORCTEST_MockProxyObject_CPP::update_count = 0;

void ORCTEST_MockProxyObject_CPP::_bind_methods() {
	ClassDB::bind_static_method("ORCTEST_MockProxyObject_CPP", D_METHOD("get_update_count"), &ORCTEST_MockProxyObject_CPP::get_update_count);
	ClassDB::bind_static_method("ORCTEST_MockProxyObject_CPP", D_METHOD("reset_update_count"), &ORCTEST_MockProxyObject_CPP::reset_update_count);
}

void ORCTEST_MockProxyObject_CPP::update_impl() {
	update_count++;
}
