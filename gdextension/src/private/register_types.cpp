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
#include <gd_overridable_marco_mock.h>
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
	GDREGISTER_RUNTIME_CLASS(ORC_QueueProcessor);
	GDREGISTER_RUNTIME_CLASS(ORC_ProxyQueue);		//TODO maybe not necessary
	GDREGISTER_RUNTIME_CLASS(ORC_DataQuery);
	GDREGISTER_RUNTIME_CLASS(ORC_PSO);
	GDREGISTER_RUNTIME_CLASS(ORC_VertexFormatInfo);
	GDREGISTER_RUNTIME_CLASS(ORC_RDHelper);

#ifdef DEBUG_ENABLED
    UtilityFunctions::print("Registering test classes (DEBUG build)");
	GDREGISTER_RUNTIME_CLASS(ORCTEST_GDOverridableMacro_Mock);
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