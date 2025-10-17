#ifndef ORC_SCENEPROXYBASE_H
#define ORC_SCENEPROXYBASE_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/node.hpp>
#include "renderer_base.h"
#include "macros.h"

#ifdef ORC_RENDERER_EXPORTS
#define ORC_API __declspec(dllexport)
#else
#define ORC_API __declspec(dllimport)
#endif

namespace godot {

class ORC_RORC_RendererBase;

class ORC_API ORC_SceneProxyBase : public RefCounted {
	GDCLASS(ORC_SceneProxyBase, RefCounted)

protected:
	static void _bind_methods();

	// TODO : expose or not expose ?
	Node* scene_root;

	Array find_all_in_tree(Node* root, const Callable &selector);

public:
	ORC_RendererBase* renderer;
	ORC_RendererBase* get_renderer() { return renderer; }
	void set_renderer(ORC_RendererBase* renderer) { this->renderer = renderer; }

	DECLARE_GD_OVERRIDABLE_METHOD(void, setup, Node*)
	DECLARE_GD_OVERRIDABLE_METHOD(void, pre_render)
	DECLARE_GD_OVERRIDABLE_METHOD(void, post_render)
	DECLARE_GD_OVERRIDABLE_METHOD(void, cleanup)
};

}

#endif