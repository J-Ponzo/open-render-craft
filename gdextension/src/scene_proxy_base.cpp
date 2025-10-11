#include "scene_proxy_base.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void ORC_SceneProxyBase::_bind_methods() {
	ClassDB::bind_method(D_METHOD("setup"), &ORC_SceneProxyBase::setup);
	ClassDB::bind_method(D_METHOD("pre_render"), &ORC_SceneProxyBase::pre_render);
	ClassDB::bind_method(D_METHOD("post_render"), &ORC_SceneProxyBase::post_render);
	ClassDB::bind_method(D_METHOD("cleanup"), &ORC_SceneProxyBase::cleanup);
}

ORC_SceneProxyBase::ORC_SceneProxyBase() {
	// Initialize any variables here.
}

ORC_SceneProxyBase::~ORC_SceneProxyBase() {
	// Add your cleanup here.
}

void ORC_SceneProxyBase::setup(Node* scene) {
	UtilityFunctions::print("ORC_SceneProxyBase.setup scene=", scene->get_name());
}

void ORC_SceneProxyBase::pre_render(){
	UtilityFunctions::print("ORC_SceneProxyBase.pre_render");
}

void ORC_SceneProxyBase::post_render() {
	UtilityFunctions::print("ORC_SceneProxyBase.post_render");
}

void ORC_SceneProxyBase::cleanup() {
	UtilityFunctions::print("ORC_SceneProxyBase.cleanup");
}