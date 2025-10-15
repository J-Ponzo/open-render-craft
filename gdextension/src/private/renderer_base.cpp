#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/rendering_device.hpp>
#include <godot_cpp/classes/rd_texture_format.hpp>
#include <godot_cpp/classes/rd_texture_view.hpp>

#include "renderer_base.h"
#include "macros.h"

using namespace godot;

ORC_RendererBase::ORC_RendererBase() {
	// Initialize any variables here.
}

ORC_RendererBase::~ORC_RendererBase() {
	// Add your cleanup here.
}

void ORC_RendererBase::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_scene_proxy"), &ORC_RendererBase::get_scene_proxy);
    ClassDB::bind_method(D_METHOD("set_scene_proxy", "scene_proxy"), &ORC_RendererBase::set_scene_proxy);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "scene_proxy", PROPERTY_HINT_RESOURCE_TYPE, "ORC_SceneProxyBase"), "set_scene_proxy", "get_scene_proxy");

    BIND_GD_OVERRIDABLE_METHOD(ORC_RendererBase, setup)
    BIND_GD_OVERRIDABLE_METHOD(ORC_RendererBase, pre_render)
    BIND_GD_OVERRIDABLE_METHOD(ORC_RendererBase, render)
    BIND_GD_OVERRIDABLE_METHOD(ORC_RendererBase, get_render_target)
    BIND_GD_OVERRIDABLE_METHOD(ORC_RendererBase, cleanup)

    BIND_GD_OVERRIDABLE_METHOD(ORC_RendererBase, proc_0_args)
	BIND_GD_OVERRIDABLE_METHOD(ORC_RendererBase, proc_1_args, "arg1")
	BIND_GD_OVERRIDABLE_METHOD(ORC_RendererBase, proc_2_args, "arg1", "arg2")
	BIND_GD_OVERRIDABLE_METHOD(ORC_RendererBase, proc_3_args, "arg1", "arg2", "arg3")
	BIND_GD_OVERRIDABLE_METHOD(ORC_RendererBase, proc_4_args, "arg1", "arg2", "arg3", "arg4")
	BIND_GD_OVERRIDABLE_METHOD(ORC_RendererBase, func_0_args)
	BIND_GD_OVERRIDABLE_METHOD(ORC_RendererBase, func_1_args, "arg1")
	BIND_GD_OVERRIDABLE_METHOD(ORC_RendererBase, func_2_args, "arg1", "arg2")
	BIND_GD_OVERRIDABLE_METHOD(ORC_RendererBase, func_3_args, "arg1", "arg2", "arg3")
	BIND_GD_OVERRIDABLE_METHOD(ORC_RendererBase, func_4_args, "arg1", "arg2", "arg3", "arg4")
}

DEFINE_GD_OVERRIDABLE_METHOD_0_ARGS(ORC_RendererBase, void, setup)
void ORC_RendererBase::setup() {
	UtilityFunctions::print("ORC_RendererBase.setup");
}

DEFINE_GD_OVERRIDABLE_METHOD_0_ARGS(ORC_RendererBase, void, pre_render)
void ORC_RendererBase::pre_render(){
	UtilityFunctions::print("ORC_RendererBase.pre_render");
}

DEFINE_GD_OVERRIDABLE_METHOD_0_ARGS(ORC_RendererBase, void, render)
void ORC_RendererBase::render() {
	UtilityFunctions::print("ORC_RendererBase.render");
}

DEFINE_GD_OVERRIDABLE_METHOD_0_ARGS(ORC_RendererBase, RID, get_render_target)
RID ORC_RendererBase::get_render_target() {
	UtilityFunctions::print("ORC_RendererBase.get_render_target");

	RenderingDevice *rd = RenderingServer::get_singleton()->get_rendering_device();

    const int width = 512;
    const int height = 512;

	Ref<RDTextureFormat> tex_format;
    tex_format.instantiate();
    tex_format->set_format(RenderingDevice::DATA_FORMAT_R8G8B8A8_UNORM);
    tex_format->set_width(width);
    tex_format->set_height(height);
    tex_format->set_usage_bits(RenderingDevice::TEXTURE_USAGE_SAMPLING_BIT | RenderingDevice::TEXTURE_USAGE_COLOR_ATTACHMENT_BIT | RenderingDevice::TEXTURE_USAGE_CAN_UPDATE_BIT);

    Ref<RDTextureView> tex_view;
    tex_view.instantiate();

    RID rid = rd->texture_create(tex_format, tex_view, TypedArray<PackedByteArray>());

    PackedByteArray data;
    data.resize(width * height * 4);
    uint8_t *ptr = data.ptrw();
    for (int i = 0; i < width * height; i++) {
        ptr[i * 4 + 0] = 255; // R
        ptr[i * 4 + 1] = 0;   // G
        ptr[i * 4 + 2] = 255; // B
        ptr[i * 4 + 3] = 255; // A
    }

    rd->texture_update(rid, 0, data);
    return rid;
}

DEFINE_GD_OVERRIDABLE_METHOD_0_ARGS(ORC_RendererBase, void, cleanup)
void ORC_RendererBase::cleanup() {
	UtilityFunctions::print("ORC_SceneProxyBase.cleanup");
}

DEFINE_GD_OVERRIDABLE_METHOD_0_ARGS(ORC_RendererBase, void, proc_0_args)
void ORC_RendererBase::proc_0_args() {
	UtilityFunctions::print("ORC_SceneProxyBase.proc_0_args");
}

DEFINE_GD_OVERRIDABLE_METHOD_1_ARGS(ORC_RendererBase, void, proc_1_args, int, arg1)
void ORC_RendererBase::proc_1_args(int arg1) {
	UtilityFunctions::print("ORC_SceneProxyBase.proc_1_args ", arg1);
}

DEFINE_GD_OVERRIDABLE_METHOD_2_ARGS(ORC_RendererBase, void, proc_2_args, int, arg1, int, arg2)
void ORC_RendererBase::proc_2_args(int arg1, int arg2) {
	UtilityFunctions::print("ORC_SceneProxyBase.proc_1_args ", arg1, arg2);
}

DEFINE_GD_OVERRIDABLE_METHOD_3_ARGS(ORC_RendererBase, void, proc_3_args, int, arg1, int, arg2, int, arg3)
void ORC_RendererBase::proc_3_args(int arg1, int arg2, int arg3) {
	UtilityFunctions::print("ORC_SceneProxyBase.proc_1_args ", arg1, arg2, arg3);
}

DEFINE_GD_OVERRIDABLE_METHOD_4_ARGS(ORC_RendererBase, void, proc_4_args, int, arg1, int, arg2, int, arg3, int, arg4)
void ORC_RendererBase::proc_4_args(int arg1, int arg2, int arg3, int arg4) {
	UtilityFunctions::print("ORC_SceneProxyBase.proc_1_args ", arg1, arg2, arg3, arg4);
}

DEFINE_GD_OVERRIDABLE_METHOD_0_ARGS(ORC_RendererBase, int, func_0_args)
int ORC_RendererBase::func_0_args() {
	UtilityFunctions::print("ORC_SceneProxyBase.func_0_args");
    return 0;
}

DEFINE_GD_OVERRIDABLE_METHOD_1_ARGS(ORC_RendererBase, int, func_1_args, int, arg1)
int ORC_RendererBase::func_1_args(int arg1) {
	UtilityFunctions::print("ORC_SceneProxyBase.func_1_args ", arg1);
    return arg1;
}

DEFINE_GD_OVERRIDABLE_METHOD_2_ARGS(ORC_RendererBase, int, func_2_args, int, arg1, int, arg2)
int ORC_RendererBase::func_2_args(int arg1, int arg2) {
	UtilityFunctions::print("ORC_SceneProxyBase.func_1_args ", arg1, arg2);
    return arg1 + arg2;
}

DEFINE_GD_OVERRIDABLE_METHOD_3_ARGS(ORC_RendererBase, int, func_3_args, int, arg1, int, arg2, int, arg3)
int ORC_RendererBase::func_3_args(int arg1, int arg2, int arg3) {
	UtilityFunctions::print("ORC_SceneProxyBase.func_1_args ", arg1, arg2, arg3);
    return arg1 + arg2 + arg3;
}

DEFINE_GD_OVERRIDABLE_METHOD_4_ARGS(ORC_RendererBase, int, func_4_args, int, arg1, int, arg2, int, arg3, int, arg4)
int ORC_RendererBase::func_4_args(int arg1, int arg2, int arg3, int arg4) {
	UtilityFunctions::print("ORC_SceneProxyBase.func_1_args ", arg1, arg2, arg3, arg4);
    return arg1 + arg2 + arg3 + arg4;
}