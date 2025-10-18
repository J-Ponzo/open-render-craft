#include <scene_proxy_base.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

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

// TODO func ptr may not be usefull
static void find_all_in_tree(Node* root, bool(*selector)(Node*), std::vector<Node*>& out_nodes) {
	if (selector(root)) {
        out_nodes.push_back(root);
    }

	 Array children = root->get_children();
    for (int i = 0; i < children.size(); i++) {
        Node *child = Object::cast_to<Node>(children[i]);
        if (child) {
            find_all_in_tree(child, selector, out_nodes);
        }
    }
}

static void on_node_enter_tree(Node* node) {
	UtilityFunctions::print("ORC_SceneProxyBase.on_node_enter_tree(", node->get_name(), ")");
}

DEFINE_GD_OVERRIDABLE_METHOD_1_ARGS(ORC_SceneProxyBase, void, setup, Node*, scene)
void ORC_SceneProxyBase::setup(Node* scene) {
	this->scene_root = scene;
	this->scene_root->get_tree()->connect("node_added", Callable(this, "on_node_enter_tree"));
	this->scene_root->get_tree()->connect("node_removed", Callable(this, "on_node_exit_tree"));

	std::vector<Node*> all_nodes;
	find_all_in_tree(this->scene_root, [](Node* node) { return true; }, all_nodes);
	for (Node* node : all_nodes)
		on_node_enter_tree(node);
}

DEFINE_GD_OVERRIDABLE_METHOD_0_ARGS(ORC_SceneProxyBase, void, pre_render)
void ORC_SceneProxyBase::pre_render(){
	UtilityFunctions::print("ORC_SceneProxyBase.pre_render");
}

DEFINE_GD_OVERRIDABLE_METHOD_0_ARGS(ORC_SceneProxyBase, void, post_render)
void ORC_SceneProxyBase::post_render() {
	UtilityFunctions::print("ORC_SceneProxyBase.post_render");
}

static void on_node_exit_tree(Node* node) {
	UtilityFunctions::print("ORC_SceneProxyBase.on_node_exit_tree(", node->get_name(), ")");
}

DEFINE_GD_OVERRIDABLE_METHOD_0_ARGS(ORC_SceneProxyBase, void, cleanup)
void ORC_SceneProxyBase::cleanup() {
	this->scene_root->get_tree()->disconnect("node_added", Callable(this, "on_node_enter_tree"));
	this->scene_root->get_tree()->disconnect("node_removed", Callable(this, "on_node_exit_tree"));

	std::vector<Node*> all_nodes;
	find_all_in_tree(this->scene_root, [](Node* node) { return true; }, all_nodes);
	for (Node* node : all_nodes)
		on_node_exit_tree(node);
}