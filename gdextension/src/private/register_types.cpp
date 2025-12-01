#include "register_types.h"

#include <renderer_base.h>
#include <render_pass_base.h>
#include <scene_proxy_base.h>
#include <proxy_data.h>
#include <primary_data.h>
#include <secondary_data.h>
#include <proxy_object.h>
#include <proxy_factory.h>
#include <proxy_registry.h>
#include <proxy_registry_dump.h>
#include <queue_processor.h>
#include <proxy_queue.h>
#include <data_query.h>
#include <pso.h>
#include <rd_helper.h>
#include <impl_registry.h>

#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#ifdef DEBUG_ENABLED
#include "../tests/macros/gd_overridable_marco_mock.h"
#include "../tests/proxy/mocks/camera_data.h"
#include "../tests/proxy/mocks/mesh_data.h"
#include "../tests/proxy/mocks/omni_light_data.h"
#include "../tests/proxy/mocks/spot_light_data.h"
#include "../tests/proxy/mocks/directional_light_data.h"
#include "../tests/proxy/mocks/topology_data.h"
#include "../tests/proxy/mocks/mock_proxy_object.h"
#include "../tests/proxy/mocks/camera_proxy.h"
#include "../tests/proxy/mocks/mesh_proxy.h"
#include "../tests/proxy/mocks/omni_light_proxy.h"
#include "../tests/proxy/mocks/spot_light_proxy.h"
#include "../tests/proxy/mocks/directional_light_proxy.h"
#include "../tests/proxy/mocks/proxy_factory_mock.h"
#endif

using namespace godot;

void initialize_orc_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	GDREGISTER_RUNTIME_CLASS(ORC_RendererBase);
	GDREGISTER_RUNTIME_CLASS(ORC_RenderPassBase);
	GDREGISTER_RUNTIME_CLASS(ORC_SceneProxyBase);
	GDREGISTER_RUNTIME_CLASS(ORC_ProxyData);
	GDREGISTER_RUNTIME_CLASS(ORC_PrimaryData);
	GDREGISTER_RUNTIME_CLASS(ORC_SecondaryData);
	GDREGISTER_RUNTIME_CLASS(ORC_ProxyObject);
	GDREGISTER_RUNTIME_CLASS(ORC_ProxyFactory);
	GDREGISTER_RUNTIME_CLASS(ORC_ProxyRegistry);
	GDREGISTER_RUNTIME_CLASS(ORC_ProxyRegistryDump);
	GDREGISTER_RUNTIME_CLASS(ORC_QueueProcessor);
	GDREGISTER_RUNTIME_CLASS(ORC_ProxyQueue);		//TODO maybe not necessary
	GDREGISTER_RUNTIME_CLASS(ORC_DataQuery);
	GDREGISTER_RUNTIME_CLASS(ORC_PSO);
	GDREGISTER_RUNTIME_CLASS(ORC_VertexFormatInfo);
	GDREGISTER_RUNTIME_CLASS(ORC_RDHelper);

#ifdef DEBUG_ENABLED
    UtilityFunctions::print("Registering test classes (DEBUG build)");
	GDREGISTER_RUNTIME_CLASS(ORCTEST_GDOverridableMacro_Mock);
	
	GDREGISTER_RUNTIME_CLASS(ORCTEST_CameraData_CPP);
	GDREGISTER_RUNTIME_CLASS(ORCTEST_MeshData_CPP);
	GDREGISTER_RUNTIME_CLASS(ORCTEST_OmniLightData_CPP);
	GDREGISTER_RUNTIME_CLASS(ORCTEST_SpotLightData_CPP);
	GDREGISTER_RUNTIME_CLASS(ORCTEST_DirectionalLightData_CPP);
	GDREGISTER_RUNTIME_CLASS(ORCTEST_TopologyData_CPP);
	GDREGISTER_RUNTIME_CLASS(ORCTEST_MockProxyObject_CPP);
	GDREGISTER_RUNTIME_CLASS(ORCTEST_CameraProxy_CPP);
	GDREGISTER_RUNTIME_CLASS(ORCTEST_MeshProxy_CPP);
	GDREGISTER_RUNTIME_CLASS(ORCTEST_OmniLightProxy_CPP);
	GDREGISTER_RUNTIME_CLASS(ORCTEST_SpotLightProxy_CPP);
	GDREGISTER_RUNTIME_CLASS(ORCTEST_DirectionalLightProxy_CPP);
	GDREGISTER_RUNTIME_CLASS(ORCTEST_ProxyFactory_CPPMock);
	
	ORC_ProxyRegistry::register_cpp_type<ORCTEST_CameraData_CPP>("ORCTEST_CameraData_CPP");
	ORC_ProxyRegistry::register_cpp_type<ORCTEST_MeshData_CPP>("ORCTEST_MeshData_CPP");
	ORC_ProxyRegistry::register_cpp_type<ORCTEST_OmniLightData_CPP>("ORCTEST_OmniLightData_CPP");
	ORC_ProxyRegistry::register_cpp_type<ORCTEST_SpotLightData_CPP>("ORCTEST_SpotLightData_CPP");
	ORC_ProxyRegistry::register_cpp_type<ORCTEST_DirectionalLightData_CPP>("ORCTEST_DirectionalLightData_CPP");
	ORC_ProxyRegistry::register_cpp_type<ORCTEST_TopologyData_CPP>("ORCTEST_TopologyData_CPP");
#endif

	ORC_ImplRegistry::get_singleton().initialize_all(p_level);
}

void uninitialize_orc_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	ORC_ImplRegistry::get_singleton().uninitialize_all(p_level);
}

extern "C" {
// Initialization.
GDExtensionBool GDE_EXPORT orc_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
	godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

	init_obj.register_initializer(initialize_orc_module);
	init_obj.register_terminator(uninitialize_orc_module);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);
	// init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_CORE);

	return init_obj.init();
}
}