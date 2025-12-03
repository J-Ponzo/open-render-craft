#include <scene_proxy_base.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/visual_instance3d.hpp>

using namespace godot;

static const char* ERR_INVALID_SCRIPT = "[ORC] Script is not valid.";
static const char* ERR_GDSCRIPT_NO_GLOBAL_NAME = "[ORC] Cannot create query with a GDScript that has no global_name (inner class or unnamed script).";
static const char* ERR_QUEUE_ALREADY_EXISTS = "[ORC] Queue '%s' already exists.";
static const char* ERR_QUEUE_NOT_FOUND = "[ORC] Queue '%s' not found.";
static const char* ERR_QUEUE_INVALID = "[ORC] Queue '%s' is invalid.";

void ORC_SceneProxyBase::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_renderer"), &ORC_SceneProxyBase::get_renderer);
    ClassDB::bind_method(D_METHOD("set_renderer", "renderer"), &ORC_SceneProxyBase::set_renderer);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "renderer", PROPERTY_HINT_RESOURCE_TYPE, "ORC_SceneProxyBase"), "set_renderer", "get_renderer");

	ClassDB::bind_method(D_METHOD("get_proxy_factory"), &ORC_SceneProxyBase::get_proxy_factory);
	ClassDB::bind_method(D_METHOD("set_proxy_factory", "proxy_factory"), &ORC_SceneProxyBase::set_proxy_factory);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "proxy_factory", PROPERTY_HINT_RESOURCE_TYPE, "ORC_ProxyFactory"), "set_proxy_factory", "get_proxy_factory");

	ClassDB::bind_method(D_METHOD("create_query_gd", "script", "flag_names", "flag_values"), &ORC_SceneProxyBase::create_query_gd, DEFVAL(TypedArray<StringName>()), DEFVAL(TypedArray<bool>()));
	ClassDB::bind_method(D_METHOD("create_query_cpp", "class_name", "flag_names", "flag_values"), &ORC_SceneProxyBase::create_query_cpp, DEFVAL(TypedArray<StringName>()), DEFVAL(TypedArray<bool>()));
	ClassDB::bind_method(D_METHOD("create_queue", "queue_name", "init_query", "processors"), &ORC_SceneProxyBase::create_queue, DEFVAL(TypedArray<ORC_QueueProcessor>()));
	ClassDB::bind_method(D_METHOD("fetch_queue_data", "queue_name"), &ORC_SceneProxyBase::fetch_queue_data);
	
	ClassDB::bind_method(D_METHOD("dump_registry"), &ORC_SceneProxyBase::dump_registry);

	ClassDB::bind_method(D_METHOD("setup", "scene"), &ORC_SceneProxyBase::setup);
	ClassDB::bind_method(D_METHOD("pre_render"), &ORC_SceneProxyBase::pre_render);
	ClassDB::bind_method(D_METHOD("cleanup"), &ORC_SceneProxyBase::cleanup);
}

ORC_SceneProxyBase::ORC_SceneProxyBase() {
	proxy_registry.instantiate();
}

ORC_SceneProxyBase::~ORC_SceneProxyBase() {
	for (auto& pair : proxy_queues) {
		delete pair.second;
	}
	proxy_queues.clear();
}

static void get_all_nodes_recursive(Node* root, std::vector<Node*>& out_nodes) {
	out_nodes.push_back(root);

	Array children = root->get_children();
	for (int i = 0; i < children.size(); i++) {
		Node *child = Object::cast_to<Node>(children[i]);
		if (child) {
			get_all_nodes_recursive(child, out_nodes);
		}
	}
}

void ORC_SceneProxyBase::setup(Node* scene) {
	this->scene_root = scene;
	this->scene_root->get_tree()->connect("node_added", callable_mp(this, &ORC_SceneProxyBase::on_node_enter_tree));
	this->scene_root->get_tree()->connect("node_removed", callable_mp(this, &ORC_SceneProxyBase::on_node_exit_tree));

	std::vector<Node*> all_nodes;
	get_all_nodes_recursive(this->scene_root, all_nodes);
	for (Node* node : all_nodes)
		on_node_enter_tree(node);
}

void ORC_SceneProxyBase::on_node_enter_tree(Node* node) {
	Ref<ORC_ProxyObject> proxy_object = proxy_factory->create_from(node, proxy_registry);

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
	
	for (auto& pair : proxy_queues) {
		pair.second->execute();
	}
}

void ORC_SceneProxyBase::on_node_exit_tree(Node* node) {
	auto it = proxy_objects_pool.find(node);
	if (it == proxy_objects_pool.end()) return;
	Ref<ORC_ProxyObject> proxy_object = it->second;
	
	proxy_factory->free(proxy_object, proxy_registry);
	proxy_objects_pool.erase(it);
}

void ORC_SceneProxyBase::cleanup() {
	this->scene_root->get_tree()->disconnect("node_added", callable_mp(this, &ORC_SceneProxyBase::on_node_enter_tree));
	this->scene_root->get_tree()->disconnect("node_removed", callable_mp(this, &ORC_SceneProxyBase::on_node_exit_tree));

	std::vector<Node*> all_nodes;
	get_all_nodes_recursive(this->scene_root, all_nodes);
	for (Node* node : all_nodes)
		on_node_exit_tree(node);
	
	proxy_registry->clear();
}

TypedArray<ORC_ProxyData> ORC_SceneProxyBase::get_by_query(const Ref<ORC_DataQuery>& query) const {
	TypedArray<ORC_ProxyData> result;
	if (!proxy_registry.is_valid() || !query.is_valid()) return result;
	
	return proxy_registry->get_by_query_internal(query);
}

Ref<ORC_DataQuery> ORC_SceneProxyBase::create_query_gd(const Ref<GDScript>& script, const TypedArray<StringName>& flag_names, const TypedArray<bool>& flag_values) {
	if (!proxy_registry.is_valid()) return Ref<ORC_DataQuery>();
	if (!script.is_valid()) ERR_FAIL_V_MSG(Ref<ORC_DataQuery>(), ERR_INVALID_SCRIPT);
	
	String global_name = script->get_global_name();
	if (global_name.is_empty()) ERR_FAIL_V_MSG(Ref<ORC_DataQuery>(), ERR_GDSCRIPT_NO_GLOBAL_NAME);
	
	return proxy_registry->create_query_internal(TypeKey(script), flag_names, flag_values);
}

Ref<ORC_DataQuery> ORC_SceneProxyBase::create_query_cpp(const StringName& class_name, const TypedArray<StringName>& flag_names, const TypedArray<bool>& flag_values) {
	if (!proxy_registry.is_valid()) return Ref<ORC_DataQuery>();

	std::type_index type_id = ORC_ProxyRegistry::get_cpp_type_index(class_name);
	if (type_id == typeid(void)) {
		return Ref<ORC_DataQuery>();
	}
	
	return proxy_registry->create_query_internal(TypeKey(type_id), flag_names, flag_values);
}

void ORC_SceneProxyBase::create_queue(const StringName& queue_name, const Ref<ORC_DataQuery>& init_query, const TypedArray<ORC_QueueProcessor>& processors) {
	if (proxy_queues.find(queue_name) != proxy_queues.end()) ERR_FAIL_MSG(vformat(ERR_QUEUE_ALREADY_EXISTS, String(queue_name)));
	
	ORC_ProxyQueue* queue = new ORC_ProxyQueue(this, init_query);
	
	for (int i = 0; i < processors.size(); i++) {
		Ref<ORC_QueueProcessor> processor = processors[i];
		if (processor.is_valid()) {
			processor->scene_proxy = this;
			queue->add_processor(processor);
		}
	}
	
	proxy_queues[queue_name] = queue;
}

TypedArray<ORC_ProxyData> ORC_SceneProxyBase::fetch_queue_data(const StringName& queue_name) const {
	auto it = proxy_queues.find(queue_name);
	if (it == proxy_queues.end()) ERR_FAIL_V_MSG(TypedArray<ORC_ProxyData>(), vformat(ERR_QUEUE_NOT_FOUND, String(queue_name)));
	
	ORC_ProxyQueue* queue = it->second;
	if (queue == nullptr) ERR_FAIL_V_MSG(TypedArray<ORC_ProxyData>(), vformat(ERR_QUEUE_INVALID, String(queue_name)));
	
	return queue->get_cached_data();
}

Ref<ORC_ProxyRegistryDump> ORC_SceneProxyBase::dump_registry() const {
	if (!proxy_registry.is_valid()) {
		Ref<ORC_ProxyRegistryDump> empty_dump;
		empty_dump.instantiate();
		return empty_dump;
	}
	return proxy_registry->dump_registry();
}