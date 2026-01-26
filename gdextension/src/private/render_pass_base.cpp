#include <render_pass_base.h>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

#define ERR_RP_FB_ALREADY_EXISTS "ORC_RendererPassBase::create_framebuffer: Framebuffer '%s' already exists."
#define ERR_RP_FB_NOT_FOUND "ORC_RendererPassBase::get_framebuffer_format: Framebuffer '%s' not found."

void ORC_RenderPassBase::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_renderer"), &ORC_RenderPassBase::get_renderer);
    ClassDB::bind_method(D_METHOD("set_renderer", "renderer"), &ORC_RenderPassBase::set_renderer);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "renderer", PROPERTY_HINT_RESOURCE_TYPE, "ORC_RendererBase"), "set_renderer", "get_renderer");

    ClassDB::bind_method(D_METHOD("get_direct_psos"), &ORC_RenderPassBase::get_direct_psos);
    ClassDB::bind_method(D_METHOD("set_direct_psos", "direct_psos"), &ORC_RenderPassBase::set_direct_psos);
    ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "direct_psos"), "set_direct_psos", "get_direct_psos");

    ClassDB::bind_method(D_METHOD("get_pso_factories"), &ORC_RenderPassBase::get_pso_factories);
    ClassDB::bind_method(D_METHOD("set_pso_factories", "pso_factories"), &ORC_RenderPassBase::set_pso_factories);
    ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "pso_factories"), "set_pso_factories", "get_pso_factories");

    ClassDB::bind_method(D_METHOD("create_framebuffer", "fb_name", "fb_format", "fb"), &ORC_RenderPassBase::create_framebuffer);
    ClassDB::bind_method(D_METHOD("get_framebuffer_format", "attachment_name"), &ORC_RenderPassBase::get_framebuffer_format);
    ClassDB::bind_method(D_METHOD("get_framebuffer", "attachment_name"), &ORC_RenderPassBase::get_framebuffer);

    BIND_GD_OVERRIDABLE_METHOD(ORC_RenderPassBase, setup)
    BIND_GD_OVERRIDABLE_METHOD(ORC_RenderPassBase, render)
    BIND_GD_OVERRIDABLE_METHOD(ORC_RenderPassBase, cleanup)
}

void ORC_RenderPassBase::create_framebuffer(const StringName& fb_name, const int64_t fb_format, const RID& fb) {
	if (framebuffers.find(fb_name) != framebuffers.end()) {
		ERR_FAIL_MSG(vformat(ERR_RP_FB_ALREADY_EXISTS, String(fb_name)));
	}
	framebuffer_formats[fb_name] = fb_format;
	framebuffers[fb_name] = fb;
}

// TODO : get_framebuffer_format() and get_framebuffer() returns framebuffer format / RID but the param is named attachment. Check this it's misleading
int64_t ORC_RenderPassBase::get_framebuffer_format(const StringName& attachment_name) const {
	auto it = framebuffer_formats.find(attachment_name);
	if (it == framebuffer_formats.end()) {
		ERR_FAIL_V_MSG(-1, vformat(ERR_RP_FB_NOT_FOUND, String(attachment_name)));
	}
	return it->second;
}

RID ORC_RenderPassBase::get_framebuffer(const StringName& attachment_name) const {
	auto it = framebuffers.find(attachment_name);
	if (it == framebuffers.end()) {
		ERR_FAIL_V_MSG(RID(), vformat(ERR_RP_FB_NOT_FOUND, String(attachment_name)));
	}
	return it->second;
}

DEFINE_GD_OVERRIDABLE_METHOD_0_ARGS(ORC_RenderPassBase, void, setup)
void ORC_RenderPassBase::setup_impl() {
}

DEFINE_GD_OVERRIDABLE_METHOD_0_ARGS(ORC_RenderPassBase, void, render)
void ORC_RenderPassBase::render_impl() {
}

DEFINE_GD_OVERRIDABLE_METHOD_0_ARGS(ORC_RenderPassBase, void, cleanup)
void ORC_RenderPassBase::cleanup_impl() {
}
