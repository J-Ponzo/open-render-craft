#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/rendering_device.hpp>
#include <godot_cpp/classes/rd_texture_format.hpp>
#include <godot_cpp/classes/rd_texture_view.hpp>

#include <renderer_base.h>
#include <macros.h>

using namespace godot;

void ORC_RendererBase::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_scene_proxy"), &ORC_RendererBase::get_scene_proxy);
    ClassDB::bind_method(D_METHOD("set_scene_proxy", "scene_proxy"), &ORC_RendererBase::set_scene_proxy);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "scene_proxy", PROPERTY_HINT_RESOURCE_TYPE, "ORC_SceneProxyBase"), "set_scene_proxy", "get_scene_proxy");

	ClassDB::bind_method(D_METHOD("get_render_passes"), &ORC_RendererBase::get_render_passes);
	ClassDB::bind_method(D_METHOD("set_render_passes", "render_passes"), &ORC_RendererBase::set_render_passes);
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "render_passes"), "set_render_passes", "get_render_passes");

	ClassDB::bind_method(D_METHOD("get_attachments"), &ORC_RendererBase::get_attachments);
	ClassDB::bind_method(D_METHOD("set_attachments", "attachments"), &ORC_RendererBase::set_attachments);
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "attachments"), "set_attachments", "get_attachments");

    BIND_GD_OVERRIDABLE_METHOD(ORC_RendererBase, setup)
    BIND_GD_OVERRIDABLE_METHOD(ORC_RendererBase, pre_render)
    BIND_GD_OVERRIDABLE_METHOD(ORC_RendererBase, render)
    BIND_GD_OVERRIDABLE_METHOD(ORC_RendererBase, get_render_target)
    BIND_GD_OVERRIDABLE_METHOD(ORC_RendererBase, cleanup)
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