#include <scene_proxy_base.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

using namespace godot;

void ORC_SceneProxyBase::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_renderer"), &ORC_SceneProxyBase::get_renderer);
    ClassDB::bind_method(D_METHOD("set_renderer", "renderer"), &ORC_SceneProxyBase::set_renderer);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "renderer", PROPERTY_HINT_RESOURCE_TYPE, "ORC_SceneProxyBase"), "set_renderer", "get_renderer");

	ClassDB::bind_method(D_METHOD("get_proxy_factory"), &ORC_SceneProxyBase::get_proxy_factory);
	ClassDB::bind_method(D_METHOD("set_proxy_factory", "proxy_factory"), &ORC_SceneProxyBase::set_proxy_factory);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "proxy_factory", PROPERTY_HINT_RESOURCE_TYPE, "ORC_ProxyFactory"), "set_proxy_factory", "get_proxy_factory");

	BIND_GD_OVERRIDABLE_METHOD(ORC_SceneProxyBase, setup, "scene")
	BIND_GD_OVERRIDABLE_METHOD(ORC_SceneProxyBase, pre_render)
	BIND_GD_OVERRIDABLE_METHOD(ORC_SceneProxyBase, post_render)
	BIND_GD_OVERRIDABLE_METHOD(ORC_SceneProxyBase, cleanup)
}

ORC_SceneProxyBase::ORC_SceneProxyBase() {
	proxy_cache.instantiate();
}

// TODO : not sure it's needed to unref here
ORC_SceneProxyBase::~ORC_SceneProxyBase() {
	if (proxy_cache.is_valid()) {
		proxy_cache.unref();
	}
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

void ORC_SceneProxyBase::on_node_enter_tree(Node* node) {
	Ref<ORC_ProxyObject> proxy_object = proxy_factory->create_from(node, proxy_cache);

	if (proxy_object == nullptr) return;

	proxy_objects_pool[node] = proxy_object;
}

DEFINE_GD_OVERRIDABLE_METHOD_0_ARGS(ORC_SceneProxyBase, void, pre_render)
void ORC_SceneProxyBase::pre_render(){
	UtilityFunctions::print("ORC_SceneProxyBase.pre_render");
}

DEFINE_GD_OVERRIDABLE_METHOD_0_ARGS(ORC_SceneProxyBase, void, post_render)
void ORC_SceneProxyBase::post_render() {
	UtilityFunctions::print("ORC_SceneProxyBase.post_render");
}

void ORC_SceneProxyBase::on_node_exit_tree(Node* node) {
	auto it = proxy_objects_pool.find(node);
	if (it == proxy_objects_pool.end()) return;
	Ref<ORC_ProxyObject> proxy_object = it->second;
	
	proxy_factory->free_data(node, proxy_cache);
	proxy_objects_pool.erase(it);
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