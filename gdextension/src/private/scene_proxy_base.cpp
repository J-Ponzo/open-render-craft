#include <scene_proxy_base.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/visual_instance3d.hpp>

using namespace godot;

void ORC_SceneProxyBase::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_renderer"), &ORC_SceneProxyBase::get_renderer);
    ClassDB::bind_method(D_METHOD("set_renderer", "renderer"), &ORC_SceneProxyBase::set_renderer);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "renderer", PROPERTY_HINT_RESOURCE_TYPE, "ORC_SceneProxyBase"), "set_renderer", "get_renderer");

	ClassDB::bind_method(D_METHOD("get_proxy_factory"), &ORC_SceneProxyBase::get_proxy_factory);
	ClassDB::bind_method(D_METHOD("set_proxy_factory", "proxy_factory"), &ORC_SceneProxyBase::set_proxy_factory);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "proxy_factory", PROPERTY_HINT_RESOURCE_TYPE, "ORC_ProxyFactory"), "set_proxy_factory", "get_proxy_factory");

	ClassDB::bind_method(D_METHOD("get_by_type", "script"), &ORC_SceneProxyBase::get_by_type_gd);
	ClassDB::bind_method(D_METHOD("dump_cache"), &ORC_SceneProxyBase::dump_cache);

	ClassDB::bind_method(D_METHOD("setup", "scene"), &ORC_SceneProxyBase::setup);
	ClassDB::bind_method(D_METHOD("pre_render"), &ORC_SceneProxyBase::pre_render);
	ClassDB::bind_method(D_METHOD("cleanup"), &ORC_SceneProxyBase::cleanup);
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

void ORC_SceneProxyBase::setup(Node* scene) {
	this->scene_root = scene;
	this->scene_root->get_tree()->connect("node_added", callable_mp(this, &ORC_SceneProxyBase::on_node_enter_tree));
	this->scene_root->get_tree()->connect("node_removed", callable_mp(this, &ORC_SceneProxyBase::on_node_exit_tree));

	std::vector<Node*> all_nodes;
	find_all_in_tree(this->scene_root, [](Node* node) { return true; }, all_nodes);
	for (Node* node : all_nodes)
		on_node_enter_tree(node);
}

void ORC_SceneProxyBase::on_node_enter_tree(Node* node) {
	Ref<ORC_ProxyObject> proxy_object = proxy_factory->create_from(node, proxy_cache);

	if (!proxy_object.is_valid()) return;

	VisualInstance3D* vi = Object::cast_to<VisualInstance3D>(node);
	if (vi == nullptr) proxy_object->is_active_ = true;
	else proxy_object->is_active_ = vi->is_visible_in_tree();

	proxy_objects_pool[node] = proxy_object;
}

void ORC_SceneProxyBase::pre_render(){
	for (auto& [node, proxy_object] : proxy_objects_pool) {
		if (proxy_object->is_active_)
			proxy_object->update();
	}
}

void ORC_SceneProxyBase::on_node_exit_tree(Node* node) {
	auto it = proxy_objects_pool.find(node);
	if (it == proxy_objects_pool.end()) return;
	Ref<ORC_ProxyObject> proxy_object = it->second;
	
	proxy_factory->free(proxy_object, proxy_cache);
	proxy_objects_pool.erase(it);
}

void ORC_SceneProxyBase::cleanup() {
	this->scene_root->get_tree()->disconnect("node_added", callable_mp(this, &ORC_SceneProxyBase::on_node_enter_tree));
	this->scene_root->get_tree()->disconnect("node_removed", callable_mp(this, &ORC_SceneProxyBase::on_node_exit_tree));

	std::vector<Node*> all_nodes;
	find_all_in_tree(this->scene_root, [](Node* node) { return true; }, all_nodes);
	for (Node* node : all_nodes)
		on_node_exit_tree(node);
}

TypedArray<ORC_ProxyData> ORC_SceneProxyBase::get_by_type_gd(const Ref<GDScript>& script) const {
	TypedArray<ORC_ProxyData> result;
	if (!proxy_cache.is_valid() || !script.is_valid()) return result;
	
	std::vector<Ref<ORC_ProxyData>> raw = proxy_cache->get_by_type(TypeKey(script));
	for (const auto& data : raw) {
		result.append(data);
	}
	return result;
}

String ORC_SceneProxyBase::dump_cache() const {
	if (!proxy_cache.is_valid()) {
		return "Proxy cache is not valid.";
	}
	return proxy_cache->dump_cache();
}