#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/rendering_device.hpp>
#include <godot_cpp/classes/rd_texture_format.hpp>
#include <godot_cpp/classes/rd_texture_view.hpp>

#include <renderer_base.h>
#include <render_pass_base.h>
#include <macros.h>

using namespace godot;

Ref<ORC_RendererBase> ORC_RendererBase::instance;

#define ERR_RB_RENDER_PASS_ALREADY_EXISTS "ORC_RendererBase::create_render_pass: Render pass '%s' already exists."
#define ERR_RB_RENDER_PASS_NOT_FOUND "ORC_RendererBase::get_render_pass: Render pass '%s' not found."
#define ERR_RB_ATTACHMENT_ALREADY_EXISTS "ORC_RendererBase::create_attachment: Attachment '%s' already exists."
#define ERR_RB_ATTACHMENT_NOT_FOUND "ORC_RendererBase::get_attachment: Attachment '%s' not found."

Ref<ORC_RendererBase> ORC_RendererBase::get_instance() {
	return ORC_RendererBase::instance;
}

void ORC_RendererBase::set_instance(const Ref<ORC_RendererBase>& instance) {
	ORC_RendererBase::instance = instance;
}

void ORC_RendererBase::_bind_methods() {
	ClassDB::bind_static_method("ORC_RendererBase", D_METHOD("get_instance"), &ORC_RendererBase::get_instance);
	// TODO : find a way to setup descriptions in exposed methods so that we can say to NOT using it directly
	ClassDB::bind_static_method("ORC_RendererBase", D_METHOD("_set_instance", "instance"), &ORC_RendererBase::set_instance);

	ClassDB::bind_method(D_METHOD("get_scene_proxy"), &ORC_RendererBase::get_scene_proxy);
    ClassDB::bind_method(D_METHOD("set_scene_proxy", "scene_proxy"), &ORC_RendererBase::set_scene_proxy);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "scene_proxy", PROPERTY_HINT_RESOURCE_TYPE, "ORC_SceneProxyBase"), "set_scene_proxy", "get_scene_proxy");

	ClassDB::bind_method(D_METHOD("create_render_pass", "pass_name", "render_pass"), &ORC_RendererBase::create_render_pass);
	ClassDB::bind_method(D_METHOD("get_render_pass", "pass_name"), &ORC_RendererBase::get_render_pass);
	ClassDB::bind_method(D_METHOD("get_all_pass_names"), &ORC_RendererBase::get_all_pass_names);

	ClassDB::bind_method(D_METHOD("create_attachment", "attachment_name", "attachment"), &ORC_RendererBase::create_attachment);
	ClassDB::bind_method(D_METHOD("get_attachment", "attachment_name"), &ORC_RendererBase::get_attachment);

    BIND_GD_OVERRIDABLE_METHOD(ORC_RendererBase, setup)
    BIND_GD_OVERRIDABLE_METHOD(ORC_RendererBase, pre_render)
    BIND_GD_OVERRIDABLE_METHOD(ORC_RendererBase, render)
    BIND_GD_OVERRIDABLE_METHOD(ORC_RendererBase, get_render_target)
    BIND_GD_OVERRIDABLE_METHOD(ORC_RendererBase, cleanup)
}

void ORC_RendererBase::create_render_pass(const StringName& pass_name, const Ref<ORC_RenderPassBase>& render_pass) {
	if (render_passes.find(pass_name) != render_passes.end()) {
		ERR_FAIL_MSG(vformat(ERR_RB_RENDER_PASS_ALREADY_EXISTS, String(pass_name)));
	}
	render_passes[pass_name] = render_pass;
}

Ref<ORC_RenderPassBase> ORC_RendererBase::get_render_pass(const StringName& pass_name) const {
	auto it = render_passes.find(pass_name);
	if (it == render_passes.end()) {
		ERR_FAIL_V_MSG(Ref<ORC_RenderPassBase>(), vformat(ERR_RB_RENDER_PASS_NOT_FOUND, String(pass_name)));
	}
	return it->second;
}

TypedArray<StringName> ORC_RendererBase::get_all_pass_names() const {
	TypedArray<StringName> names;
	for (const auto& pair : render_passes) {
		names.append(pair.first);
	}
	return names;
}

void ORC_RendererBase::create_attachment(const StringName& attachment_name, const RID& attachment) {
	if (attachments.find(attachment_name) != attachments.end()) {
		ERR_FAIL_MSG(vformat(ERR_RB_ATTACHMENT_ALREADY_EXISTS, String(attachment_name)));
	}
	attachments[attachment_name] = attachment;
}

RID ORC_RendererBase::get_attachment(const StringName& attachment_name) const {
	auto it = attachments.find(attachment_name);
	if (it == attachments.end()) {
		ERR_FAIL_V_MSG(RID(), vformat(ERR_RB_ATTACHMENT_NOT_FOUND, String(attachment_name)));
	}
	return it->second;
}

DEFINE_GD_OVERRIDABLE_METHOD_0_ARGS(ORC_RendererBase, void, setup)
void ORC_RendererBase::setup_impl() {
}

DEFINE_GD_OVERRIDABLE_METHOD_0_ARGS(ORC_RendererBase, void, pre_render)
void ORC_RendererBase::pre_render_impl(){
}

DEFINE_GD_OVERRIDABLE_METHOD_0_ARGS(ORC_RendererBase, void, render)
void ORC_RendererBase::render_impl() {
}

DEFINE_GD_OVERRIDABLE_METHOD_0_ARGS(ORC_RendererBase, RID, get_render_target)
RID ORC_RendererBase::get_render_target_impl() {
    return RID();
}

DEFINE_GD_OVERRIDABLE_METHOD_0_ARGS(ORC_RendererBase, void, cleanup)
void ORC_RendererBase::cleanup_impl() {
}