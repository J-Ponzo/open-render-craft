#include "scene_proxy_base.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void ORC_SceneProxyBase::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_renderer"), &ORC_SceneProxyBase::get_renderer);
    ClassDB::bind_method(D_METHOD("set_renderer", "renderer"), &ORC_SceneProxyBase::set_renderer);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "renderer", PROPERTY_HINT_RESOURCE_TYPE, "ORC_SceneProxyBase"), "set_renderer", "get_renderer");

	BIND_GD_OVERRIDABLE_METHOD(ORC_SceneProxyBase, setup, "scene")
	BIND_GD_OVERRIDABLE_METHOD(ORC_SceneProxyBase, pre_render)
	BIND_GD_OVERRIDABLE_METHOD(ORC_SceneProxyBase, post_render)
	BIND_GD_OVERRIDABLE_METHOD(ORC_SceneProxyBase, cleanup)
}

Array ORC_SceneProxyBase::find_all_in_tree(Node* root, const Callable &selector) {
	Array all_selected;

	Variant node_var = Variant(root);
    Variant result = selector.callv(Array::make(node_var));
	if ((bool)result) {
        all_selected.append(node_var);
    }

	 Array children = root->get_children();
    for (int i = 0; i < children.size(); i++) {
        Node *child = Object::cast_to<Node>(children[i]);
        if (child) {
            Array sub = find_all_in_tree(child, selector);
            all_selected.append_array(sub);
        }
    }

    return all_selected;
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