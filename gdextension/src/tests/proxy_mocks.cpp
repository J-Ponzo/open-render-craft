#include <proxy_mocks.h>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/omni_light3d.hpp>
#include <godot_cpp/classes/spot_light3d.hpp>
#include <godot_cpp/classes/directional_light3d.hpp>

using namespace godot;

void ORCTEST_MeshData_CPP::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_topology_data"), &ORCTEST_MeshData_CPP::get_topology_data);
	ClassDB::bind_method(D_METHOD("set_topology_data", "topology_data"), &ORCTEST_MeshData_CPP::set_topology_data);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "topology_data", PROPERTY_HINT_RESOURCE_TYPE, "ORC_SecondaryData"), "set_topology_data", "get_topology_data");
}

void ORCTEST_TopologyData_CPP::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_mesh"), &ORCTEST_TopologyData_CPP::get_mesh);
	ClassDB::bind_method(D_METHOD("set_mesh", "mesh"), &ORCTEST_TopologyData_CPP::set_mesh);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "mesh", PROPERTY_HINT_RESOURCE_TYPE, "Mesh"), "set_mesh", "get_mesh");
}

int ORCTEST_MockProxyObject_CPP::update_count = 0;

void ORCTEST_MockProxyObject_CPP::_bind_methods() {
	ClassDB::bind_static_method("ORCTEST_MockProxyObject_CPP", D_METHOD("get_update_count"), &ORCTEST_MockProxyObject_CPP::get_update_count);
	ClassDB::bind_static_method("ORCTEST_MockProxyObject_CPP", D_METHOD("reset_update_count"), &ORCTEST_MockProxyObject_CPP::reset_update_count);
}

void ORCTEST_MockProxyObject_CPP::update_impl() {
	update_count++;
}

int ORCTEST_CameraProxy_CPP::cam_update_count = 0;

void ORCTEST_CameraProxy_CPP::_bind_methods() {
	ClassDB::bind_static_method("ORCTEST_CameraProxy_CPP", D_METHOD("get_cam_update_count"), &ORCTEST_CameraProxy_CPP::get_cam_update_count);
	ClassDB::bind_static_method("ORCTEST_CameraProxy_CPP", D_METHOD("reset_cam_update_count"), &ORCTEST_CameraProxy_CPP::reset_cam_update_count);
}

void ORCTEST_CameraProxy_CPP::update_impl() {
	ORCTEST_MockProxyObject_CPP::update_impl();
	cam_update_count++;
}

int ORCTEST_MeshProxy_CPP::mesh_update_count = 0;

void ORCTEST_MeshProxy_CPP::_bind_methods() {
	ClassDB::bind_static_method("ORCTEST_MeshProxy_CPP", D_METHOD("get_mesh_update_count"), &ORCTEST_MeshProxy_CPP::get_mesh_update_count);
	ClassDB::bind_static_method("ORCTEST_MeshProxy_CPP", D_METHOD("reset_mesh_update_count"), &ORCTEST_MeshProxy_CPP::reset_mesh_update_count);
}

void ORCTEST_MeshProxy_CPP::update_impl() {
	ORCTEST_MockProxyObject_CPP::update_impl();
	mesh_update_count++;
}

int ORCTEST_OmniLightProxy_CPP::omni_update_count = 0;

void ORCTEST_OmniLightProxy_CPP::_bind_methods() {
	ClassDB::bind_static_method("ORCTEST_OmniLightProxy_CPP", D_METHOD("get_omni_update_count"), &ORCTEST_OmniLightProxy_CPP::get_omni_update_count);
	ClassDB::bind_static_method("ORCTEST_OmniLightProxy_CPP", D_METHOD("reset_omni_update_count"), &ORCTEST_OmniLightProxy_CPP::reset_omni_update_count);
}

void ORCTEST_OmniLightProxy_CPP::update_impl() {
	ORCTEST_MockProxyObject_CPP::update_impl();
	omni_update_count++;
}

int ORCTEST_SpotLightProxy_CPP::spot_update_count = 0;

void ORCTEST_SpotLightProxy_CPP::_bind_methods() {
	ClassDB::bind_static_method("ORCTEST_SpotLightProxy_CPP", D_METHOD("get_spot_update_count"), &ORCTEST_SpotLightProxy_CPP::get_spot_update_count);
	ClassDB::bind_static_method("ORCTEST_SpotLightProxy_CPP", D_METHOD("reset_spot_update_count"), &ORCTEST_SpotLightProxy_CPP::reset_spot_update_count);
}

void ORCTEST_SpotLightProxy_CPP::update_impl() {
	ORCTEST_MockProxyObject_CPP::update_impl();
	spot_update_count++;
}

int ORCTEST_DirectionalLightProxy_CPP::directional_update_count = 0;

void ORCTEST_DirectionalLightProxy_CPP::_bind_methods() {
	ClassDB::bind_static_method("ORCTEST_DirectionalLightProxy_CPP", D_METHOD("get_directional_update_count"), &ORCTEST_DirectionalLightProxy_CPP::get_directional_update_count);
	ClassDB::bind_static_method("ORCTEST_DirectionalLightProxy_CPP", D_METHOD("reset_directional_update_count"), &ORCTEST_DirectionalLightProxy_CPP::reset_directional_update_count);
}

void ORCTEST_DirectionalLightProxy_CPP::update_impl() {
	ORCTEST_MockProxyObject_CPP::update_impl();
	directional_update_count++;
}

void ORCTEST_ProxyFactory_CPPMock::_bind_methods() {
	ClassDB::bind_static_method("ORCTEST_ProxyFactory_CPPMock", D_METHOD("reset_all_counters"), &ORCTEST_ProxyFactory_CPPMock::reset_all_counters);
}

Ref<ORC_ProxyObject> ORCTEST_ProxyFactory_CPPMock::create_proxy_from_impl(Node* node) {
	Ref<ORC_ProxyObject> proxy_object;
	
	if (Object::cast_to<Camera3D>(node)) {
		proxy_object = Ref<ORCTEST_CameraProxy_CPP>(memnew(ORCTEST_CameraProxy_CPP));
	} else if (Object::cast_to<OmniLight3D>(node)) {
		proxy_object = Ref<ORCTEST_OmniLightProxy_CPP>(memnew(ORCTEST_OmniLightProxy_CPP));
	} else if (Object::cast_to<SpotLight3D>(node)) {
		proxy_object = Ref<ORCTEST_SpotLightProxy_CPP>(memnew(ORCTEST_SpotLightProxy_CPP));
	} else if (Object::cast_to<DirectionalLight3D>(node)) {
		proxy_object = Ref<ORCTEST_DirectionalLightProxy_CPP>(memnew(ORCTEST_DirectionalLightProxy_CPP));
	} else if (Object::cast_to<MeshInstance3D>(node)) {
		proxy_object = Ref<ORCTEST_MeshProxy_CPP>(memnew(ORCTEST_MeshProxy_CPP));
	}
	
	return proxy_object;
}

Ref<ORC_PrimaryData> ORCTEST_ProxyFactory_CPPMock::create_data_from_impl(Node* node, Ref<ORC_ProxyRegistry> registry) {
	Ref<ORC_PrimaryData> primary_data;
	
	if (Object::cast_to<Camera3D>(node)) {
		primary_data = ORC_ProxyFactory::create_and_register_primary<ORCTEST_CameraData_CPP>(registry);
		primary_data->set_flag("IS_PRIMARY", true);
	} else if (Object::cast_to<OmniLight3D>(node)) {
		primary_data = ORC_ProxyFactory::create_and_register_primary<ORCTEST_OmniLightData_CPP>(registry);
		primary_data->set_flag("IS_PRIMARY", true);
		primary_data->set_flag("IS_LIGHT", true);
	} else if (Object::cast_to<SpotLight3D>(node)) {
		primary_data = ORC_ProxyFactory::create_and_register_primary<ORCTEST_SpotLightData_CPP>(registry);
		primary_data->set_flag("IS_PRIMARY", true);
		primary_data->set_flag("IS_LIGHT", true);
	} else if (Object::cast_to<DirectionalLight3D>(node)) {
		primary_data = ORC_ProxyFactory::create_and_register_primary<ORCTEST_DirectionalLightData_CPP>(registry);
		primary_data->set_flag("IS_PRIMARY", true);
		primary_data->set_flag("IS_LIGHT", true);
	} else if (MeshInstance3D* mesh_instance = Object::cast_to<MeshInstance3D>(node)) {
		Ref<ORCTEST_MeshData_CPP> mesh_data = ORC_ProxyFactory::create_and_register_primary<ORCTEST_MeshData_CPP>(registry);
		mesh_data->set_flag("IS_PRIMARY", true);
		
		Ref<Mesh> mesh = mesh_instance->get_mesh();
		int64_t mesh_id = mesh.is_valid() ? mesh->get_rid().get_id() : -1;
		
		Ref<ORCTEST_TopologyData_CPP> topology_data = ORC_ProxyFactory::create_and_register_secondary<ORCTEST_TopologyData_CPP>(registry, mesh_data, mesh_id);
		topology_data->set_flag("IS_PRIMARY", false);
		topology_data->set_mesh(mesh);
		mesh_data->set_topology_data(topology_data);
		
		primary_data = mesh_data;
	}
	
	return primary_data;
}

bool ORCTEST_ProxyFactory_CPPMock::free_proxy_impl(Ref<ORC_ProxyObject> proxy_object) {
	return true;
}

bool ORCTEST_ProxyFactory_CPPMock::free_data_impl(Ref<ORC_ProxyData> data, Ref<ORC_ProxyRegistry> registry) {
	bool success = true;
	
	ORCTEST_TopologyData_CPP* topology_data = Object::cast_to<ORCTEST_TopologyData_CPP>(data.ptr());
	if (topology_data) {
		Ref<Mesh> mesh = topology_data->get_mesh();
		int64_t mesh_id = mesh.is_valid() ? mesh->get_rid().get_id() : -1;
		success = registry->decrement_refcount(mesh_id);
		if (success) {
			registry->unregister_data(data);
		}
	} else {
		success = registry->unregister_data(data);
	}
	
	return success;
}

void ORCTEST_ProxyFactory_CPPMock::reset_all_counters() {
	ORCTEST_MockProxyObject_CPP::reset_update_count();
	ORCTEST_CameraProxy_CPP::reset_cam_update_count();
	ORCTEST_MeshProxy_CPP::reset_mesh_update_count();
	ORCTEST_OmniLightProxy_CPP::reset_omni_update_count();
	ORCTEST_SpotLightProxy_CPP::reset_spot_update_count();
	ORCTEST_DirectionalLightProxy_CPP::reset_directional_update_count();
}
