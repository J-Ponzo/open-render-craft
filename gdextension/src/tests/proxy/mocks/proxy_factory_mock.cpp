#include "proxy_factory_mock.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/omni_light3d.hpp>
#include <godot_cpp/classes/spot_light3d.hpp>
#include <godot_cpp/classes/directional_light3d.hpp>

#include "camera_data.h"
#include "mesh_data.h"
#include "omni_light_data.h"
#include "spot_light_data.h"
#include "directional_light_data.h"
#include "topology_data.h"

#include "camera_proxy.h"
#include "mesh_proxy.h"
#include "omni_light_proxy.h"
#include "spot_light_proxy.h"
#include "directional_light_proxy.h"
#include "mock_proxy_object.h"

using namespace godot;

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

Ref<ORC_PrimaryData> ORCTEST_ProxyFactory_CPPMock::create_data_from_impl(Node* node, const Ref<ORC_ProxyRegistry>& registry) {
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

bool ORCTEST_ProxyFactory_CPPMock::free_proxy_impl(const Ref<ORC_ProxyObject>& proxy_object) {
	return true;
}

bool ORCTEST_ProxyFactory_CPPMock::free_data_impl(const Ref<ORC_ProxyData>& data, const Ref<ORC_ProxyRegistry>& registry) {
	bool success = true;
	
	ORCTEST_TopologyData_CPP* topology_data = Object::cast_to<ORCTEST_TopologyData_CPP>(data.ptr());
	if (topology_data) {
		Ref<Mesh> mesh = topology_data->get_mesh();
		int64_t mesh_id = mesh.is_valid() ? mesh->get_rid().get_id() : -1;
		success = ORC_ProxyFactory::destroy_and_unregister_data(data, registry, mesh_id);
	} else {
		success = ORC_ProxyFactory::destroy_and_unregister_data(data, registry);
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
