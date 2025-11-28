#include "camera_proxy.h"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

int ORCTEST_CameraProxy_CPP::cam_update_count = 0;

void ORCTEST_CameraProxy_CPP::_bind_methods() {
	ClassDB::bind_static_method("ORCTEST_CameraProxy_CPP", D_METHOD("get_cam_update_count"), &ORCTEST_CameraProxy_CPP::get_cam_update_count);
	ClassDB::bind_static_method("ORCTEST_CameraProxy_CPP", D_METHOD("reset_cam_update_count"), &ORCTEST_CameraProxy_CPP::reset_cam_update_count);
}

void ORCTEST_CameraProxy_CPP::update_impl() {
	ORCTEST_MockProxyObject_CPP::update_impl();
	cam_update_count++;
}
