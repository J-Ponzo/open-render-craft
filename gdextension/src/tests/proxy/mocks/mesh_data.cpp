#include "mesh_data.h"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void ORCTEST_MeshData_CPP::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_topology_data"), &ORCTEST_MeshData_CPP::get_topology_data);
	ClassDB::bind_method(D_METHOD("set_topology_data", "topology_data"), &ORCTEST_MeshData_CPP::set_topology_data);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "topology_data", PROPERTY_HINT_RESOURCE_TYPE, "ORC_SecondaryData"), "set_topology_data", "get_topology_data");
}
