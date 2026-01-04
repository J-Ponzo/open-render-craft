#ifndef ORC_PSO_INFO_H
#define ORC_PSO_INFO_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/rendering_device.hpp>
#include <godot_cpp/classes/rd_pipeline_color_blend_state.hpp>
#include <godot_cpp/classes/rd_pipeline_rasterization_state.hpp>
#include <godot_cpp/classes/rd_pipeline_multisample_state.hpp>
#include <godot_cpp/classes/rd_pipeline_depth_stencil_state.hpp>

namespace godot {

class ORC_PSOInfo : public RefCounted {
    GDCLASS(ORC_PSOInfo, RefCounted)

protected:
    static void _bind_methods();

private:
    void set_vertex_shader_src(const String &value);
    String get_vertex_shader_src() const;

    void set_fragment_shader_src(const String &value);
    String get_fragment_shader_src() const;

    void set_vertex_format(int64_t value);
    int64_t get_vertex_format() const;

    void set_rasterization_state(const Ref<RDPipelineRasterizationState> &value);
    Ref<RDPipelineRasterizationState> get_rasterization_state() const;

    void set_multisample_state(const Ref<RDPipelineMultisampleState> &value);
    Ref<RDPipelineMultisampleState> get_multisample_state() const;

    void set_depth_stencil_state(const Ref<RDPipelineDepthStencilState> &value);
    Ref<RDPipelineDepthStencilState> get_depth_stencil_state() const;

    void set_color_blend_state(const Ref<RDPipelineColorBlendState> &value);
    Ref<RDPipelineColorBlendState> get_color_blend_state() const;

    void set_blend_constant(const Color &value);
    Color get_blend_constant() const;

    void set_enable_logic_op(bool value);
    bool get_enable_logic_op() const;

    void set_logic_op(RenderingDevice::LogicOperation value);
    RenderingDevice::LogicOperation get_logic_op() const;

public:
    ORC_PSOInfo();
    ~ORC_PSOInfo();

    String vertex_shader_src;
    String fragment_shader_src;
    int64_t vertex_format;
    Ref<RDPipelineRasterizationState> rasterization_state;
    Ref<RDPipelineMultisampleState> multisample_state;
    Ref<RDPipelineDepthStencilState> depth_stencil_state;
    Ref<RDPipelineColorBlendState> color_blend_state;
};
}

#endif