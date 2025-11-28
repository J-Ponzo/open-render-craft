#ifndef ORCTEST_PROXYMOCKS_H
#define ORCTEST_PROXYMOCKS_H

#include <godot_cpp/classes/mesh.hpp>
#include <primary_data.h>
#include <secondary_data.h>
#include <proxy_object.h>
#include <proxy_factory.h>
#include <macros.h>

namespace godot {

class ORCTEST_CameraData_CPP : public ORC_PrimaryData {
	GDCLASS(ORCTEST_CameraData_CPP, ORC_PrimaryData)

protected:
	static void _bind_methods() {}
};

class ORCTEST_MeshData_CPP : public ORC_PrimaryData {
	GDCLASS(ORCTEST_MeshData_CPP, ORC_PrimaryData)

protected:
	static void _bind_methods();

public:
	Ref<ORC_SecondaryData> topology_data;
	Ref<ORC_SecondaryData> get_topology_data() const { return topology_data; }
	void set_topology_data(const Ref<ORC_SecondaryData> &topology_data) { this->topology_data = topology_data; }
};

class ORCTEST_OmniLightData_CPP : public ORC_PrimaryData {
	GDCLASS(ORCTEST_OmniLightData_CPP, ORC_PrimaryData)

protected:
	static void _bind_methods() {}
};

class ORCTEST_SpotLightData_CPP : public ORC_PrimaryData {
	GDCLASS(ORCTEST_SpotLightData_CPP, ORC_PrimaryData)

protected:
	static void _bind_methods() {}
};

class ORCTEST_DirectionalLightData_CPP : public ORC_PrimaryData {
	GDCLASS(ORCTEST_DirectionalLightData_CPP, ORC_PrimaryData)

protected:
	static void _bind_methods() {}
};

class ORCTEST_TopologyData_CPP : public ORC_SecondaryData {
	GDCLASS(ORCTEST_TopologyData_CPP, ORC_SecondaryData)

protected:
	static void _bind_methods();

public:
	Ref<Mesh> mesh;
	Ref<Mesh> get_mesh() const { return mesh; }
	void set_mesh(const Ref<Mesh> &mesh) { this->mesh = mesh; }
};

class ORCTEST_MockProxyObject_CPP : public ORC_ProxyObject {
	GDCLASS(ORCTEST_MockProxyObject_CPP, ORC_ProxyObject)

protected:
	static void _bind_methods();

public:
	static int update_count;
	static int get_update_count() { return update_count; }
	static void reset_update_count() { update_count = 0; }

	void update_impl();
};

class ORCTEST_CameraProxy_CPP : public ORCTEST_MockProxyObject_CPP {
	GDCLASS(ORCTEST_CameraProxy_CPP, ORCTEST_MockProxyObject_CPP)

protected:
	static void _bind_methods();

public:
	static int cam_update_count;
	static int get_cam_update_count() { return cam_update_count; }
	static void reset_cam_update_count() { cam_update_count = 0; }

	void update_impl();
};

class ORCTEST_MeshProxy_CPP : public ORCTEST_MockProxyObject_CPP {
	GDCLASS(ORCTEST_MeshProxy_CPP, ORCTEST_MockProxyObject_CPP)

protected:
	static void _bind_methods();

public:
	static int mesh_update_count;
	static int get_mesh_update_count() { return mesh_update_count; }
	static void reset_mesh_update_count() { mesh_update_count = 0; }

	void update_impl();
};

class ORCTEST_OmniLightProxy_CPP : public ORCTEST_MockProxyObject_CPP {
	GDCLASS(ORCTEST_OmniLightProxy_CPP, ORCTEST_MockProxyObject_CPP)

protected:
	static void _bind_methods();

public:
	static int omni_update_count;
	static int get_omni_update_count() { return omni_update_count; }
	static void reset_omni_update_count() { omni_update_count = 0; }

	void update_impl();
};

class ORCTEST_SpotLightProxy_CPP : public ORCTEST_MockProxyObject_CPP {
	GDCLASS(ORCTEST_SpotLightProxy_CPP, ORCTEST_MockProxyObject_CPP)

protected:
	static void _bind_methods();

public:
	static int spot_update_count;
	static int get_spot_update_count() { return spot_update_count; }
	static void reset_spot_update_count() { spot_update_count = 0; }

	void update_impl();
};

class ORCTEST_DirectionalLightProxy_CPP : public ORCTEST_MockProxyObject_CPP {
	GDCLASS(ORCTEST_DirectionalLightProxy_CPP, ORCTEST_MockProxyObject_CPP)

protected:
	static void _bind_methods();

public:
	static int directional_update_count;
	static int get_directional_update_count() { return directional_update_count; }
	static void reset_directional_update_count() { directional_update_count = 0; }

	void update_impl();
};

class ORCTEST_ProxyFactory_CPPMock : public ORC_ProxyFactory {
	GDCLASS(ORCTEST_ProxyFactory_CPPMock, ORC_ProxyFactory)

protected:
	static void _bind_methods();

public:
	Ref<ORC_ProxyObject> create_proxy_from_impl(Node* node);
	Ref<ORC_PrimaryData> create_data_from_impl(Node* node, Ref<ORC_ProxyRegistry> registry);
	bool free_proxy_impl(Ref<ORC_ProxyObject> proxy_object);
	bool free_data_impl(Ref<ORC_ProxyData> data, Ref<ORC_ProxyRegistry> registry);

	static void reset_all_counters();
};

}

#endif
