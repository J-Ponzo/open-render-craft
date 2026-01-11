#ifndef ORC_RD_HELPER_H
#define ORC_RD_HELPER_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/rendering_device.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/rd_sampler_state.hpp>
#include <pso.h>
#include <pso_info.h>

namespace godot {

class ORC_VertexFormatInfo : public Resource {
    GDCLASS(ORC_VertexFormatInfo, Resource)

protected:
    static void _bind_methods();

private:
    bool is_2d = false;
    bool has_normal = false;
    bool has_tangent = false;
    bool has_color = false;
    bool has_uv = false;
    bool has_uv2 = false;
    bool has_bones = false;
    bool has_weights = false;

public:
    ORC_VertexFormatInfo();
    ~ORC_VertexFormatInfo();

    void set_is_2d(bool value);
    bool get_is_2d() const;

    void set_has_normal(bool value);
    bool get_has_normal() const;

    void set_has_tangent(bool value);
    bool get_has_tangent() const;

    void set_has_color(bool value);
    bool get_has_color() const;

    void set_has_uv(bool value);
    bool get_has_uv() const;

    void set_has_uv2(bool value);
    bool get_has_uv2() const;

    void set_has_bones(bool value);
    bool get_has_bones() const;

    void set_has_weights(bool value);
    bool get_has_weights() const;
};

// TODO : singleton ?
class ORC_RDHelper : public RefCounted {
    GDCLASS(ORC_RDHelper, RefCounted)

protected:
    static void _bind_methods();

public:
    ORC_RDHelper();
    ~ORC_RDHelper();

    static RenderingDevice* get_rd();
    static int64_t create_vertex_format(const Ref<ORC_VertexFormatInfo>& vertex_format_def);
    static PackedByteArray proj_to_bytes(const Projection& proj);
    static PackedByteArray projs_to_bytes(const TypedArray<Projection>& projs);
    static Ref<RDSamplerState> create_sampler_state(
        RenderingDevice::SamplerFilter mag_filter = RenderingDevice::SAMPLER_FILTER_LINEAR,
        RenderingDevice::SamplerFilter min_filter = RenderingDevice::SAMPLER_FILTER_LINEAR,
        RenderingDevice::SamplerRepeatMode repeat_u = RenderingDevice::SAMPLER_REPEAT_MODE_REPEAT,
        RenderingDevice::SamplerRepeatMode repeat_v = RenderingDevice::SAMPLER_REPEAT_MODE_REPEAT
    );
    static Ref<ORC_PSO> create_pso(const Ref<ORC_PSOInfo>& pso_info, int64_t framebuffer_format);
    static RID compile_shader(const String& vertex_src, const String& fragment_src);
};

}

#endif
