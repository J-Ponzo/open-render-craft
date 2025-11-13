#ifndef ORC_SCENEPROXYBASE_H
#define ORC_SCENEPROXYBASE_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/gd_script.hpp>
#include <godot_cpp/variant/typed_array.hpp>
#include <renderer_base.h>
#include <proxy_registry.h>
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

private:
	static void _bind_methods();

	Node* scene_root;
	Ref<ORC_ProxyRegistry> proxy_cache;
	std::unordered_map<Node*, Ref<ORC_ProxyObject>> proxy_objects_pool;

	Ref<ORC_ProxyFactory> proxy_factory;
	Ref<ORC_ProxyFactory> get_proxy_factory() const { return proxy_factory; }
	void set_proxy_factory(const Ref<ORC_ProxyFactory> &proxy_factory) { this->proxy_factory = proxy_factory; }

	void setup(Node* scene);
	void pre_render();
	void cleanup();

	void on_node_enter_tree(Node* node);
	void on_node_exit_tree(Node* node);

public:
	ORC_SceneProxyBase();
	~ORC_SceneProxyBase();

	// TODO maybe useless
	Ref<ORC_RendererBase> renderer;
	Ref<ORC_RendererBase> get_renderer() const { return renderer; }
	void set_renderer(const Ref<ORC_RendererBase> &renderer) { this->renderer = renderer; }

	template <class T>
	std::vector<Ref<T>> get_by_type() const {
		std::vector<Ref<ORC_ProxyData>> raw = proxy_cache->get_by_type(TypeKey(typeid(T)));
		std::vector<Ref<T>> result;
		result.reserve(raw.size());
		for (const auto& data : raw) {
			result.push_back(Ref<T>(Object::cast_to<T>(data.ptr())));
		}
		return result;
	}

	TypedArray<ORC_ProxyData> get_by_type_gd(const Ref<GDScript>& script) const;
	String dump_cache() const;
};

}

#endif