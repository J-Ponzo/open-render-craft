#include <rd_helper.h>
#include <godot_cpp/classes/rd_vertex_attribute.hpp>
#include <godot_cpp/variant/typed_array.hpp>

using namespace godot;

void ORC_RDHelper::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_rd"), &ORC_RDHelper::get_rd);
    ClassDB::bind_static_method("ORC_RDHelper", D_METHOD("create_vertex_format", "vertex_format_def"), &ORC_RDHelper::create_vertex_format);
}

ORC_RDHelper::ORC_RDHelper() {
    rd = RenderingServer::get_singleton()->get_rendering_device();
}

ORC_RDHelper::~ORC_RDHelper() {
}

RenderingDevice* ORC_RDHelper::get_rd() const {
    return rd;
}

int64_t ORC_RDHelper::create_vertex_format(const Ref<Resource>& vertex_format_def) {
    if (!vertex_format_def.is_valid()) {
        return -1;
    }

    RenderingDevice* rd = RenderingServer::get_singleton()->get_rendering_device();
    TypedArray<RDVertexAttribute> attrs;

    bool is_2d = vertex_format_def->get("is_2d");
    bool has_normal = vertex_format_def->get("has_normal");
    bool has_tangent = vertex_format_def->get("has_tangent");
    bool has_color = vertex_format_def->get("has_color");
    bool has_uv = vertex_format_def->get("has_uv");
    bool has_uv2 = vertex_format_def->get("has_uv2");
    bool has_bones = vertex_format_def->get("has_bones");
    bool has_weights = vertex_format_def->get("has_weights");

    // Position attribute
    if (is_2d) {
        Ref<RDVertexAttribute> position_attr;
        position_attr.instantiate();
        position_attr->set_format(RenderingDevice::DATA_FORMAT_R32G32_SFLOAT);
        position_attr->set_stride(POSITION_2D_NB_FLOATS * SIZEOF_FLOAT);
        position_attr->set_offset(0);
        position_attr->set_location(0);
        position_attr->set_frequency(RenderingDevice::VERTEX_FREQUENCY_VERTEX);
        attrs.append(position_attr);
    } else {
        Ref<RDVertexAttribute> position_attr;
        position_attr.instantiate();
        position_attr->set_format(RenderingDevice::DATA_FORMAT_R32G32B32_SFLOAT);
        position_attr->set_stride(POSITION_3D_NB_FLOATS * SIZEOF_FLOAT);
        position_attr->set_offset(0);
        position_attr->set_location(0);
        position_attr->set_frequency(RenderingDevice::VERTEX_FREQUENCY_VERTEX);
        attrs.append(position_attr);
    }

    // Normal attribute
    if (has_normal) {
        Ref<RDVertexAttribute> normal_attr;
        normal_attr.instantiate();
        normal_attr->set_format(RenderingDevice::DATA_FORMAT_R32G32B32_SFLOAT);
        normal_attr->set_stride(NORMAL_NB_FLOATS * SIZEOF_FLOAT);
        normal_attr->set_offset(0);
        normal_attr->set_location(1);
        normal_attr->set_frequency(RenderingDevice::VERTEX_FREQUENCY_VERTEX);
        attrs.append(normal_attr);
    }

    // Tangent attribute
    if (has_tangent) {
        Ref<RDVertexAttribute> tangent_attr;
        tangent_attr.instantiate();
        tangent_attr->set_format(RenderingDevice::DATA_FORMAT_R32G32B32A32_SFLOAT);
        tangent_attr->set_stride(TANGENT_NB_FLOATS * SIZEOF_FLOAT);
        tangent_attr->set_offset(0);
        tangent_attr->set_location(2);
        tangent_attr->set_frequency(RenderingDevice::VERTEX_FREQUENCY_VERTEX);
        attrs.append(tangent_attr);
    }

    // Color attribute
    if (has_color) {
        Ref<RDVertexAttribute> color_attr;
        color_attr.instantiate();
        color_attr->set_format(RenderingDevice::DATA_FORMAT_R32G32B32A32_SFLOAT);
        color_attr->set_stride(COLOR_NB_FLOATS * SIZEOF_FLOAT);
        color_attr->set_offset(0);
        color_attr->set_location(3);
        color_attr->set_frequency(RenderingDevice::VERTEX_FREQUENCY_VERTEX);
        attrs.append(color_attr);
    }

    // UV attribute
    if (has_uv) {
        Ref<RDVertexAttribute> uv_attr;
        uv_attr.instantiate();
        uv_attr->set_format(RenderingDevice::DATA_FORMAT_R32G32_SFLOAT);
        uv_attr->set_stride(UV_NB_FLOATS * SIZEOF_FLOAT);
        uv_attr->set_offset(0);
        uv_attr->set_location(4);
        uv_attr->set_frequency(RenderingDevice::VERTEX_FREQUENCY_VERTEX);
        attrs.append(uv_attr);
    }

    // UV2 attribute
    if (has_uv2) {
        Ref<RDVertexAttribute> uv2_attr;
        uv2_attr.instantiate();
        uv2_attr->set_format(RenderingDevice::DATA_FORMAT_R32G32_SFLOAT);
        uv2_attr->set_stride(UV2_NB_FLOATS * SIZEOF_FLOAT);
        uv2_attr->set_offset(0);
        uv2_attr->set_location(5);
        uv2_attr->set_frequency(RenderingDevice::VERTEX_FREQUENCY_VERTEX);
        attrs.append(uv2_attr);
    }

    // Bones attribute
    if (has_bones) {
        Ref<RDVertexAttribute> bones_attr;
        bones_attr.instantiate();
        bones_attr->set_format(RenderingDevice::DATA_FORMAT_R32G32B32A32_SINT);
        bones_attr->set_stride(BONES_NB_INTS * SIZEOF_INT);
        bones_attr->set_offset(0);
        bones_attr->set_location(6);
        bones_attr->set_frequency(RenderingDevice::VERTEX_FREQUENCY_VERTEX);
        attrs.append(bones_attr);
    }

    // Weights attribute
    if (has_weights) {
        Ref<RDVertexAttribute> weights_attr;
        weights_attr.instantiate();
        weights_attr->set_format(RenderingDevice::DATA_FORMAT_R32G32B32A32_SFLOAT);
        weights_attr->set_stride(WEIGHT_NB_FLOATS * SIZEOF_FLOAT);
        weights_attr->set_offset(0);
        weights_attr->set_location(7);
        weights_attr->set_frequency(RenderingDevice::VERTEX_FREQUENCY_VERTEX);
        attrs.append(weights_attr);
    }

    return rd->vertex_format_create(attrs);
}
