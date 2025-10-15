#ifndef ORC_RENDERBASE_H
#define ORC_RENDERBASE_H

#include <godot_cpp/classes/ref_counted.hpp>
#include "scene_proxy_base.h"

#ifdef ORC_RENDERER_EXPORTS
#define ORC_API __declspec(dllexport)
#else
#define ORC_API __declspec(dllimport)
#endif

namespace godot {

class ORC_API ORC_RendererBase : public RefCounted {
	GDCLASS(ORC_RendererBase, RefCounted)

protected:
	static void _bind_methods();

public:
	ORC_RendererBase();
	~ORC_RendererBase();

	Ref<ORC_SceneProxyBase> scene_proxy;
	Ref<ORC_SceneProxyBase> get_scene_proxy() { return scene_proxy; }
	void set_scene_proxy(Ref<ORC_SceneProxyBase> scene_proxy) { this->scene_proxy = scene_proxy; }

	void setup();
	void pre_render();
	void render();
	RID get_render_target();
	void cleanup();
};

}

#endif