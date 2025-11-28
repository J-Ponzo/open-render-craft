#include "topology_data.h"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void ORCTEST_TopologyData_CPP::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_mesh"), &ORCTEST_TopologyData_CPP::get_mesh);
	ClassDB::bind_method(D_METHOD("set_mesh", "mesh"), &ORCTEST_TopologyData_CPP::set_mesh);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "mesh", PROPERTY_HINT_RESOURCE_TYPE, "Mesh"), "set_mesh", "get_mesh");
}
