#include <pso_info.h>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void ORC_PSOInfo::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_vertex_shader_src", "value"), &ORC_PSOInfo::set_vertex_shader_src);
	ClassDB::bind_method(D_METHOD("get_vertex_shader_src"), &ORC_PSOInfo::get_vertex_shader_src);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "vertex_shader_src"), "set_vertex_shader_src", "get_vertex_shader_src");

	ClassDB::bind_method(D_METHOD("set_fragment_shader_src", "value"), &ORC_PSOInfo::set_fragment_shader_src);
	ClassDB::bind_method(D_METHOD("get_fragment_shader_src"), &ORC_PSOInfo::get_fragment_shader_src);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "fragment_shader_src"), "set_fragment_shader_src", "get_fragment_shader_src");

	ClassDB::bind_method(D_METHOD("set_vertex_format", "value"), &ORC_PSOInfo::set_vertex_format);
	ClassDB::bind_method(D_METHOD("get_vertex_format"), &ORC_PSOInfo::get_vertex_format);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "vertex_format"), "set_vertex_format", "get_vertex_format");

	ClassDB::bind_method(D_METHOD("set_rasterization_state", "value"), &ORC_PSOInfo::set_rasterization_state);
	ClassDB::bind_method(D_METHOD("get_rasterization_state"), &ORC_PSOInfo::get_rasterization_state);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "rasterization_state", PROPERTY_HINT_RESOURCE_TYPE, "RDPipelineRasterizationState"), "set_rasterization_state", "get_rasterization_state");

	ClassDB::bind_method(D_METHOD("set_multisample_state", "value"), &ORC_PSOInfo::set_multisample_state);
	ClassDB::bind_method(D_METHOD("get_multisample_state"), &ORC_PSOInfo::get_multisample_state);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "multisample_state", PROPERTY_HINT_RESOURCE_TYPE, "RDPipelineMultisampleState"), "set_multisample_state", "get_multisample_state");

	ClassDB::bind_method(D_METHOD("set_depth_stencil_state", "value"), &ORC_PSOInfo::set_depth_stencil_state);
	ClassDB::bind_method(D_METHOD("get_depth_stencil_state"), &ORC_PSOInfo::get_depth_stencil_state);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "depth_stencil_state", PROPERTY_HINT_RESOURCE_TYPE, "RDPipelineDepthStencilState"), "set_depth_stencil_state", "get_depth_stencil_state");

	ClassDB::bind_method(D_METHOD("set_color_blend_state", "value"), &ORC_PSOInfo::set_color_blend_state);
	ClassDB::bind_method(D_METHOD("get_color_blend_state"), &ORC_PSOInfo::get_color_blend_state);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "color_blend_state", PROPERTY_HINT_RESOURCE_TYPE, "RDPipelineColorBlendState"), "set_color_blend_state", "get_color_blend_state");
}

ORC_PSOInfo::ORC_PSOInfo() {}
ORC_PSOInfo::~ORC_PSOInfo() {}

void ORC_PSOInfo::set_vertex_shader_src(const String &value) { vertex_shader_src = value; }
String ORC_PSOInfo::get_vertex_shader_src() const { return vertex_shader_src; }

void ORC_PSOInfo::set_fragment_shader_src(const String &value) { fragment_shader_src = value; }
String ORC_PSOInfo::get_fragment_shader_src() const { return fragment_shader_src; }

void ORC_PSOInfo::set_vertex_format(int64_t value) { vertex_format = value; }
int64_t ORC_PSOInfo::get_vertex_format() const { return vertex_format; }

void ORC_PSOInfo::set_rasterization_state(const Ref<RDPipelineRasterizationState> &value) { rasterization_state = value; }
Ref<RDPipelineRasterizationState> ORC_PSOInfo::get_rasterization_state() const { return rasterization_state; }

void ORC_PSOInfo::set_multisample_state(const Ref<RDPipelineMultisampleState> &value) { multisample_state = value; }
Ref<RDPipelineMultisampleState> ORC_PSOInfo::get_multisample_state() const { return multisample_state; }

void ORC_PSOInfo::set_depth_stencil_state(const Ref<RDPipelineDepthStencilState> &value) { depth_stencil_state = value; }
Ref<RDPipelineDepthStencilState> ORC_PSOInfo::get_depth_stencil_state() const { return depth_stencil_state; }

void ORC_PSOInfo::set_color_blend_state(const Ref<RDPipelineColorBlendState> &value) { color_blend_state = value; }
Ref<RDPipelineColorBlendState> ORC_PSOInfo::get_color_blend_state() const { return color_blend_state; }
