#ifndef ORC_RENDERBASE_H
#define ORC_RENDERBASE_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <scene_proxy_base.h>
#include <macros.h>

namespace godot {

class ORC_SceneProxyBase;

class ORC_RendererBase : public RefCounted {
	GDCLASS(ORC_RendererBase, RefCounted)

protected:
	static void _bind_methods();

public:
	Ref<ORC_SceneProxyBase> scene_proxy;
	Ref<ORC_SceneProxyBase> get_scene_proxy() const { return scene_proxy; }
	void set_scene_proxy(const Ref<ORC_SceneProxyBase> &scene_proxy) { this->scene_proxy = scene_proxy; }

	// TODO : would be greate to type this. But could not manage to make it work so far
	Dictionary render_passes;
	Dictionary get_render_passes() const { return render_passes; }
	void set_render_passes(const Dictionary &render_passes) { this->render_passes = render_passes; }

	// TODO : would be greate to type this. But could not manage to make it work so far
	Dictionary attachments;
	Dictionary get_attachments() const { return attachments; }
	void set_attachments(const Dictionary &attachments) { this->attachments = attachments; }

	DECLARE_GD_OVERRIDABLE_METHOD(void, setup)
	DECLARE_GD_OVERRIDABLE_METHOD(void, pre_render)
	DECLARE_GD_OVERRIDABLE_METHOD(void, render)
	DECLARE_GD_OVERRIDABLE_METHOD(RID, get_render_target)
	DECLARE_GD_OVERRIDABLE_METHOD(void, cleanup)
};

}

#endif