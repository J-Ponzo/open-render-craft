#ifndef ORC_SCENEPROXYBASE_H
#define ORC_SCENEPROXYBASE_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/node.hpp>
#include <renderer_base.h>
#include <proxy_cache.h>
#include <proxy_factory.h>
#include <macros.h>

#ifdef ORC_RENDERER_EXPORTS
#define ORC_API __declspec(dllexport)
#else
#define ORC_API __declspec(dllimport)
#endif

namespace godot {

class ORC_RendererBase;

class ORC_API ORC_SceneProxyBase : public RefCounted {
	GDCLASS(ORC_SceneProxyBase, RefCounted)

protected:
	static void _bind_methods();

	Node* scene_root;
	Ref<ORC_ProxyCache> proxy_cache;
	std::unordered_map<Node*, Ref<ORC_ProxyObject>> proxy_objects_pool;

	Ref<ORC_ProxyFactory> proxy_factory;
	Ref<ORC_ProxyFactory> get_proxy_factory() { return proxy_factory; }
	void set_proxy_factory(Ref<ORC_ProxyFactory> proxy_factory) { this->proxy_factory = proxy_factory; }

	void on_node_enter_tree(Node* node);
	void on_node_exit_tree(Node* node);

public:
	ORC_SceneProxyBase();
	~ORC_SceneProxyBase();

	Ref<ORC_RendererBase> renderer;
	Ref<ORC_RendererBase> get_renderer() { return renderer; }
	void set_renderer(Ref<ORC_RendererBase> renderer) { this->renderer = renderer; }

	DECLARE_GD_OVERRIDABLE_METHOD(void, setup, Node*)
	DECLARE_GD_OVERRIDABLE_METHOD(void, pre_render)
	DECLARE_GD_OVERRIDABLE_METHOD(void, post_render)
	DECLARE_GD_OVERRIDABLE_METHOD(void, cleanup)
};

}

#endif