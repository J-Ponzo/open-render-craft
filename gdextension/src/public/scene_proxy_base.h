#ifndef ORC_SCENEPROXYBASE_H
#define ORC_SCENEPROXYBASE_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/gd_script.hpp>
#include <godot_cpp/variant/typed_array.hpp>
#include <renderer_base.h>
#include <proxy_registry.h>
#include <proxy_registry_dump.h>
#include <proxy_factory.h>
#include <proxy_queue.h>
#include <macros.h>

#include <unordered_map>

namespace godot {

class ORC_RendererBase;

class ORC_SceneProxyBase : public RefCounted {
	GDCLASS(ORC_SceneProxyBase, RefCounted)

private:
	static void _bind_methods();

	Node* scene_root;
	Ref<ORC_ProxyRegistry> proxy_registry;
	std::unordered_map<Node*, Ref<ORC_ProxyObject>> proxy_objects_pool;
	
	std::unordered_map<StringName, ORC_ProxyQueue*> proxy_queues;

	Ref<ORC_ProxyFactory> proxy_factory;
	Ref<ORC_ProxyFactory> get_proxy_factory() const { return proxy_factory; }
	void set_proxy_factory(const Ref<ORC_ProxyFactory> &proxy_factory) { this->proxy_factory = proxy_factory; }

	void setup(Node* scene);
	void pre_render();
	void cleanup();

	void on_node_enter_tree(Node* node);
	void on_node_exit_tree(Node* node);

	Ref<ORC_DataQuery> create_query_gd(const Ref<GDScript>& script, const TypedArray<StringName>& flag_names = TypedArray<StringName>(), const TypedArray<bool>& flag_values = TypedArray<bool>());
	Ref<ORC_DataQuery> create_query_cpp(const StringName& class_name, const TypedArray<StringName>& flag_names = TypedArray<StringName>(), const TypedArray<bool>& flag_values = TypedArray<bool>());

public:
	ORC_SceneProxyBase();
	~ORC_SceneProxyBase();

	// TODO maybe useless
	Ref<ORC_RendererBase> renderer;
	Ref<ORC_RendererBase> get_renderer() const { return renderer; }
	void set_renderer(const Ref<ORC_RendererBase> &renderer) { this->renderer = renderer; }
	
	TypedArray<ORC_ProxyData> get_by_query(const Ref<ORC_DataQuery>& query) const;
	void create_queue(const StringName& queue_name, const Ref<ORC_DataQuery>& init_query, const TypedArray<ORC_QueueProcessor>& processors = TypedArray<ORC_QueueProcessor>());
	TypedArray<ORC_ProxyData> fetch_queue_data(const StringName& queue_name);
	
	Ref<ORC_ProxyRegistryDump> dump_registry() const;
};

}

#endif