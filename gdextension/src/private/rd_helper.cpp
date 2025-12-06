#include <rd_helper.h>
#include <godot_cpp/classes/rd_vertex_attribute.hpp>
#include <godot_cpp/variant/typed_array.hpp>

#define SIZEOF_FLOAT 4
#define SIZEOF_INT 4
#define POSITION_2D_NB_FLOATS 2
#define POSITION_3D_NB_FLOATS 3
#define NORMAL_NB_FLOATS 3
#define TANGENT_NB_FLOATS 4
#define COLOR_NB_FLOATS 4
#define UV_NB_FLOATS 2
#define UV2_NB_FLOATS 2
#define BONES_NB_INTS 4
#define WEIGHT_NB_FLOATS 4

static const char* ERR_RDH_INVALID_VERTEX_FORMAT_INFO = "[ORC] Invalid vertex format info.";

using namespace godot;

void ORC_VertexFormatInfo::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_is_2d", "value"), &ORC_VertexFormatInfo::set_is_2d);
    ClassDB::bind_method(D_METHOD("get_is_2d"), &ORC_VertexFormatInfo::get_is_2d);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_2d"), "set_is_2d", "get_is_2d");

    ClassDB::bind_method(D_METHOD("set_has_normal", "value"), &ORC_VertexFormatInfo::set_has_normal);
    ClassDB::bind_method(D_METHOD("get_has_normal"), &ORC_VertexFormatInfo::get_has_normal);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "has_normal"), "set_has_normal", "get_has_normal");

    ClassDB::bind_method(D_METHOD("set_has_tangent", "value"), &ORC_VertexFormatInfo::set_has_tangent);
    ClassDB::bind_method(D_METHOD("get_has_tangent"), &ORC_VertexFormatInfo::get_has_tangent);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "has_tangent"), "set_has_tangent", "get_has_tangent");

    ClassDB::bind_method(D_METHOD("set_has_color", "value"), &ORC_VertexFormatInfo::set_has_color);
    ClassDB::bind_method(D_METHOD("get_has_color"), &ORC_VertexFormatInfo::get_has_color);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "has_color"), "set_has_color", "get_has_color");

    ClassDB::bind_method(D_METHOD("set_has_uv", "value"), &ORC_VertexFormatInfo::set_has_uv);
    ClassDB::bind_method(D_METHOD("get_has_uv"), &ORC_VertexFormatInfo::get_has_uv);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "has_uv"), "set_has_uv", "get_has_uv");

    ClassDB::bind_method(D_METHOD("set_has_uv2", "value"), &ORC_VertexFormatInfo::set_has_uv2);
    ClassDB::bind_method(D_METHOD("get_has_uv2"), &ORC_VertexFormatInfo::get_has_uv2);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "has_uv2"), "set_has_uv2", "get_has_uv2");

    ClassDB::bind_method(D_METHOD("set_has_bones", "value"), &ORC_VertexFormatInfo::set_has_bones);
    ClassDB::bind_method(D_METHOD("get_has_bones"), &ORC_VertexFormatInfo::get_has_bones);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "has_bones"), "set_has_bones", "get_has_bones");

    ClassDB::bind_method(D_METHOD("set_has_weights", "value"), &ORC_VertexFormatInfo::set_has_weights);
    ClassDB::bind_method(D_METHOD("get_has_weights"), &ORC_VertexFormatInfo::get_has_weights);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "has_weights"), "set_has_weights", "get_has_weights");
}

ORC_VertexFormatInfo::ORC_VertexFormatInfo() {
}

ORC_VertexFormatInfo::~ORC_VertexFormatInfo() {
}

void ORC_VertexFormatInfo::set_is_2d(bool value) { is_2d = value; }
bool ORC_VertexFormatInfo::get_is_2d() const { return is_2d; }

void ORC_VertexFormatInfo::set_has_normal(bool value) { has_normal = value; }
bool ORC_VertexFormatInfo::get_has_normal() const { return has_normal; }

void ORC_VertexFormatInfo::set_has_tangent(bool value) { has_tangent = value; }
bool ORC_VertexFormatInfo::get_has_tangent() const { return has_tangent; }

void ORC_VertexFormatInfo::set_has_color(bool value) { has_color = value; }
bool ORC_VertexFormatInfo::get_has_color() const { return has_color; }

void ORC_VertexFormatInfo::set_has_uv(bool value) { has_uv = value; }
bool ORC_VertexFormatInfo::get_has_uv() const { return has_uv; }

void ORC_VertexFormatInfo::set_has_uv2(bool value) { has_uv2 = value; }
bool ORC_VertexFormatInfo::get_has_uv2() const { return has_uv2; }

void ORC_VertexFormatInfo::set_has_bones(bool value) { has_bones = value; }
bool ORC_VertexFormatInfo::get_has_bones() const { return has_bones; }

void ORC_VertexFormatInfo::set_has_weights(bool value) { has_weights = value; }
bool ORC_VertexFormatInfo::get_has_weights() const { return has_weights; }

void ORC_RDHelper::_bind_methods() {
    ClassDB::bind_static_method("ORC_RDHelper", D_METHOD("get_rd"), &ORC_RDHelper::get_rd);
    ClassDB::bind_static_method("ORC_RDHelper", D_METHOD("create_vertex_format", "vertex_format_def"), &ORC_RDHelper::create_vertex_format);
    ClassDB::bind_static_method("ORC_RDHelper", D_METHOD("proj_to_bytes", "proj"), &ORC_RDHelper::proj_to_bytes);
}

ORC_RDHelper::ORC_RDHelper() {
}

ORC_RDHelper::~ORC_RDHelper() {
}

RenderingDevice* ORC_RDHelper::get_rd() {
    return RenderingServer::get_singleton()->get_rendering_device();
}

int64_t ORC_RDHelper::create_vertex_format(const Ref<ORC_VertexFormatInfo>& vf_info) {
    if (!vf_info.is_valid()) ERR_FAIL_V_MSG(-1, ERR_RDH_INVALID_VERTEX_FORMAT_INFO);

    RenderingDevice* rd = RenderingServer::get_singleton()->get_rendering_device();
    TypedArray<RDVertexAttribute> attrs;

    // Position attribute
    if (vf_info->get_is_2d()) {
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
    if (vf_info->get_has_normal()) {
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
    if (vf_info->get_has_tangent()) {
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
    if (vf_info->get_has_color()) {
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
    if (vf_info->get_has_uv()) {
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
    if (vf_info->get_has_uv2()) {
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
    if (vf_info->get_has_bones()) {
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
    if (vf_info->get_has_weights()) {
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

PackedByteArray ORC_RDHelper::proj_to_bytes(const Projection& proj) {
    const int SIZEOF_MAT4 = 64;
    PackedByteArray byte_array;
    byte_array.resize(SIZEOF_MAT4);
    
    int offset = 0;
    for (int i = 0; i < 4; i++) {
        Vector4 column = proj.columns[i];
        byte_array.encode_float(offset, column.x);
        offset += 4;
        byte_array.encode_float(offset, column.y);
        offset += 4;
        byte_array.encode_float(offset, column.z);
        offset += 4;
        byte_array.encode_float(offset, column.w);
        offset += 4;
    }
    
    return byte_array;
}
