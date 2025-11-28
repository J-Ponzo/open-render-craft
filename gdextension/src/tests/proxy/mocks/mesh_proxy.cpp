#include "mesh_proxy.h"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

int ORCTEST_MeshProxy_CPP::mesh_update_count = 0;

void ORCTEST_MeshProxy_CPP::_bind_methods() {
	ClassDB::bind_static_method("ORCTEST_MeshProxy_CPP", D_METHOD("get_mesh_update_count"), &ORCTEST_MeshProxy_CPP::get_mesh_update_count);
	ClassDB::bind_static_method("ORCTEST_MeshProxy_CPP", D_METHOD("reset_mesh_update_count"), &ORCTEST_MeshProxy_CPP::reset_mesh_update_count);
}

void ORCTEST_MeshProxy_CPP::update_impl() {
	ORCTEST_MockProxyObject_CPP::update_impl();
	mesh_update_count++;
}
