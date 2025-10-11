#include "renderer_base.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/rendering_device.hpp>
#include <godot_cpp/classes/rd_texture_format.hpp>
#include <godot_cpp/classes/rd_texture_view.hpp>

using namespace godot;

void ORC_RendererBase::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_scene_proxy"), &ORC_RendererBase::get_scene_proxy);
    ClassDB::bind_method(D_METHOD("set_scene_proxy", "scene_proxy"), &ORC_RendererBase::set_scene_proxy);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "scene_proxy", PROPERTY_HINT_RESOURCE_TYPE, "ORC_SceneProxyBase"), "set_scene_proxy", "get_scene_proxy");

	ClassDB::bind_method(D_METHOD("setup"), &ORC_RendererBase::setup);
	ClassDB::bind_method(D_METHOD("pre_render"), &ORC_RendererBase::pre_render);
	ClassDB::bind_method(D_METHOD("render"), &ORC_RendererBase::render);
	ClassDB::bind_method(D_METHOD("get_render_target"), &ORC_RendererBase::get_render_target);
	ClassDB::bind_method(D_METHOD("cleanup"), &ORC_RendererBase::cleanup);
}

ORC_RendererBase::ORC_RendererBase() {
	// Initialize any variables here.
}

ORC_RendererBase::~ORC_RendererBase() {
	// Add your cleanup here.
}

void ORC_RendererBase::setup() {
	UtilityFunctions::print("ORC_RendererBase.setup");
}

void ORC_RendererBase::pre_render(){
	UtilityFunctions::print("ORC_RendererBase.pre_render");
}

void ORC_RendererBase::render() {
	UtilityFunctions::print("ORC_RendererBase.render");
}

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

void ORC_RendererBase::cleanup() {
	UtilityFunctions::print("ORC_SceneProxyBase.cleanup");
}