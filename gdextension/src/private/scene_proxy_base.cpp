#include "scene_proxy_base.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

ORC_SceneProxyBase::ORC_SceneProxyBase() {
	// Initialize any variables here.
}

ORC_SceneProxyBase::~ORC_SceneProxyBase() {
	// Add your cleanup here.
}

void ORC_SceneProxyBase::_bind_methods() {
	BIND_GD_OVERRIDABLE_METHOD(ORC_SceneProxyBase, setup, "scene")
	BIND_GD_OVERRIDABLE_METHOD(ORC_SceneProxyBase, pre_render)
	BIND_GD_OVERRIDABLE_METHOD(ORC_SceneProxyBase, post_render)
	BIND_GD_OVERRIDABLE_METHOD(ORC_SceneProxyBase, cleanup)
}

DEFINE_GD_OVERRIDABLE_METHOD_1_ARGS(ORC_SceneProxyBase, void, setup, Node*, scene)
void ORC_SceneProxyBase::setup(Node* scene) {
	UtilityFunctions::print("ORC_SceneProxyBase.setup scene=", scene->get_name());
}

DEFINE_GD_OVERRIDABLE_METHOD_0_ARGS(ORC_SceneProxyBase, void, pre_render)
void ORC_SceneProxyBase::pre_render(){
	UtilityFunctions::print("ORC_SceneProxyBase.pre_render");
}

DEFINE_GD_OVERRIDABLE_METHOD_0_ARGS(ORC_SceneProxyBase, void, post_render)
void ORC_SceneProxyBase::post_render() {
	UtilityFunctions::print("ORC_SceneProxyBase.post_render");
}

DEFINE_GD_OVERRIDABLE_METHOD_0_ARGS(ORC_SceneProxyBase, void, cleanup)
void ORC_SceneProxyBase::cleanup() {
	UtilityFunctions::print("ORC_SceneProxyBase.cleanup");
}