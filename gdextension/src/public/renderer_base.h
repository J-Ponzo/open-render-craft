#ifndef ORC_RENDERBASE_H
#define ORC_RENDERBASE_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <scene_proxy_base.h>
#include <macros.h>
#include <unordered_map>

namespace godot {

class ORC_SceneProxyBase;
class ORC_RenderPassBase;

class ORC_RendererBase : public RefCounted {
	GDCLASS(ORC_RendererBase, RefCounted)

private:
	std::unordered_map<StringName, Ref<ORC_RenderPassBase>> render_passes;
	std::unordered_map<StringName, RID> attachments;

protected:
	static void _bind_methods();

public:
	Ref<ORC_SceneProxyBase> scene_proxy;
	Ref<ORC_SceneProxyBase> get_scene_proxy() const { return scene_proxy; }
	void set_scene_proxy(const Ref<ORC_SceneProxyBase>& scene_proxy) { this->scene_proxy = scene_proxy; }

	void create_render_pass(const StringName& pass_name, const Ref<ORC_RenderPassBase>& render_pass);
	Ref<ORC_RenderPassBase> get_render_pass(const StringName& pass_name) const;

	void create_attachment(const StringName& attachment_name, const RID& attachment);
	RID get_attachment(const StringName& attachment_name) const;

	DECLARE_GD_OVERRIDABLE_METHOD(void, setup)
	DECLARE_GD_OVERRIDABLE_METHOD(void, pre_render)
	DECLARE_GD_OVERRIDABLE_METHOD(void, render)
	DECLARE_GD_OVERRIDABLE_METHOD(RID, get_render_target)
	DECLARE_GD_OVERRIDABLE_METHOD(void, cleanup)
};

}

#endif