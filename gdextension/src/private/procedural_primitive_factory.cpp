#include <procedural_primitive_factory.h>
#include <rd_helper.h>
#include <godot_cpp/classes/rendering_device.hpp>
#include <godot_cpp/variant/packed_vector2_array.hpp>
#include <godot_cpp/variant/packed_vector3_array.hpp>
#include <godot_cpp/variant/packed_int32_array.hpp>
#include <godot_cpp/core/math.hpp>

using namespace godot;

static const char* ERR_PPF_INVALID_PRIMITIVE = "[ORC] Primitive is not valid.";

void ORC_ProceduralPrimitive::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_index_buffer"), &ORC_ProceduralPrimitive::get_index_buffer);
    ClassDB::bind_method(D_METHOD("get_index_array"), &ORC_ProceduralPrimitive::get_index_array);
    ClassDB::bind_method(D_METHOD("get_position_buffer"), &ORC_ProceduralPrimitive::get_position_buffer);
    ClassDB::bind_method(D_METHOD("get_normal_buffer"), &ORC_ProceduralPrimitive::get_normal_buffer);
    ClassDB::bind_method(D_METHOD("get_tangent_buffer"), &ORC_ProceduralPrimitive::get_tangent_buffer);
    ClassDB::bind_method(D_METHOD("get_color_buffer"), &ORC_ProceduralPrimitive::get_color_buffer);
    ClassDB::bind_method(D_METHOD("get_uv_buffer"), &ORC_ProceduralPrimitive::get_uv_buffer);
    ClassDB::bind_method(D_METHOD("get_uv2_buffer"), &ORC_ProceduralPrimitive::get_uv2_buffer);
    ClassDB::bind_method(D_METHOD("get_bones_buffer"), &ORC_ProceduralPrimitive::get_bones_buffer);
    ClassDB::bind_method(D_METHOD("get_weights_buffer"), &ORC_ProceduralPrimitive::get_weights_buffer);
    ClassDB::bind_method(D_METHOD("get_vertex_array"), &ORC_ProceduralPrimitive::get_vertex_array);
}

ORC_ProceduralPrimitive::ORC_ProceduralPrimitive() {}
ORC_ProceduralPrimitive::~ORC_ProceduralPrimitive() {}

RID ORC_ProceduralPrimitive::get_index_buffer() const {return index_buffer;}
RID ORC_ProceduralPrimitive::get_index_array() const {return index_array;}
RID ORC_ProceduralPrimitive::get_position_buffer() const {return position_buffer;}
RID ORC_ProceduralPrimitive::get_normal_buffer() const {return normal_buffer;}
RID ORC_ProceduralPrimitive::get_tangent_buffer() const {return tangent_buffer;}
RID ORC_ProceduralPrimitive::get_color_buffer() const {return color_buffer;}
RID ORC_ProceduralPrimitive::get_uv_buffer() const {return uv_buffer;}
RID ORC_ProceduralPrimitive::get_uv2_buffer() const {return uv2_buffer;}
RID ORC_ProceduralPrimitive::get_bones_buffer() const {return bones_buffer;}
RID ORC_ProceduralPrimitive::get_weights_buffer() const {return weights_buffer;}
RID ORC_ProceduralPrimitive::get_vertex_array() const {return vertex_array;}

void ORC_ProceduralPrimitiveFactory::_bind_methods() {
    ClassDB::bind_static_method("ORC_ProceduralPrimitiveFactory", D_METHOD("create_screen_quad", "setup_uv"), &ORC_ProceduralPrimitiveFactory::create_screen_quad, DEFVAL(false));
    ClassDB::bind_static_method("ORC_ProceduralPrimitiveFactory", D_METHOD("create_inverted_sphere", "stacks", "slices"), &ORC_ProceduralPrimitiveFactory::create_inverted_sphere, DEFVAL(32), DEFVAL(32));
    ClassDB::bind_static_method("ORC_ProceduralPrimitiveFactory", D_METHOD("create_inverted_cone", "slices"), &ORC_ProceduralPrimitiveFactory::create_inverted_cone, DEFVAL(32));
    ClassDB::bind_static_method("ORC_ProceduralPrimitiveFactory", D_METHOD("free_rids", "primitive"), &ORC_ProceduralPrimitiveFactory::free_rids);
}

ORC_ProceduralPrimitiveFactory::ORC_ProceduralPrimitiveFactory() {}
ORC_ProceduralPrimitiveFactory::~ORC_ProceduralPrimitiveFactory() {}

void ORC_ProceduralPrimitiveFactory::generate_vertex_array_from_data(Ref<ORC_ProceduralPrimitive> primitive, int vertex_count, bool is_2d) {
    Ref<ORC_VertexFormatInfo> vf_info;
    vf_info.instantiate();
    
    vf_info->set_is_2d(is_2d);
    vf_info->set_has_normal(primitive->normal_buffer.is_valid());
    vf_info->set_has_tangent(primitive->tangent_buffer.is_valid());
    vf_info->set_has_color(primitive->color_buffer.is_valid());
    vf_info->set_has_uv(primitive->uv_buffer.is_valid());
    vf_info->set_has_uv2(primitive->uv2_buffer.is_valid());
    vf_info->set_has_bones(primitive->bones_buffer.is_valid());
    vf_info->set_has_weights(primitive->weights_buffer.is_valid());
    
    TypedArray<RID> src_buffers;
    src_buffers.append(primitive->position_buffer);
    if (vf_info->get_has_normal()) {
        src_buffers.append(primitive->normal_buffer);
    }
    if (vf_info->get_has_tangent()) {
        src_buffers.append(primitive->tangent_buffer);
    }
    if (vf_info->get_has_color()) {
        src_buffers.append(primitive->color_buffer);
    }
    if (vf_info->get_has_uv()) {
        src_buffers.append(primitive->uv_buffer);
    }
    if (vf_info->get_has_uv2()) {
        src_buffers.append(primitive->uv2_buffer);
    }
    if (vf_info->get_has_bones()) {
        src_buffers.append(primitive->bones_buffer);
    }
    if (vf_info->get_has_weights()) {
        src_buffers.append(primitive->weights_buffer);
    }
    
    int64_t vertex_format = ORC_RDHelper::create_vertex_format(vf_info);
    primitive->vertex_array = ORC_RDHelper::get_rd()->vertex_array_create(vertex_count, vertex_format, src_buffers);
}

Ref<ORC_ProceduralPrimitive> ORC_ProceduralPrimitiveFactory::create_screen_quad(bool setup_uv) {
    Ref<ORC_ProceduralPrimitive> primitive;
    primitive.instantiate();

    PackedVector2Array position_data;
    position_data.append(Vector2(-1.0, -1.0));
    position_data.append(Vector2(1.0, -1.0));
    position_data.append(Vector2(1.0, 1.0));
    position_data.append(Vector2(-1.0, 1.0));
    
    PackedByteArray byte_array = position_data.to_byte_array();
    primitive->position_buffer = ORC_RDHelper::get_rd()->vertex_buffer_create(byte_array.size(), byte_array);

    if (setup_uv) {
        PackedVector2Array uv_data;
        uv_data.append(Vector2(0.0, 0.0));
        uv_data.append(Vector2(1.0, 0.0));
        uv_data.append(Vector2(1.0, 1.0));
        uv_data.append(Vector2(0.0, 1.0));
        
        byte_array = uv_data.to_byte_array();
        primitive->uv_buffer = ORC_RDHelper::get_rd()->vertex_buffer_create(byte_array.size(), byte_array);
    }

    PackedInt32Array index_data;
    index_data.append(0);
    index_data.append(1);
    index_data.append(2);
    index_data.append(0);
    index_data.append(2);
    index_data.append(3);
    
    byte_array = index_data.to_byte_array();
    primitive->index_buffer = ORC_RDHelper::get_rd()->index_buffer_create(index_data.size(), RenderingDevice::INDEX_BUFFER_FORMAT_UINT32, byte_array);
    primitive->index_array = ORC_RDHelper::get_rd()->index_array_create(primitive->index_buffer, 0, index_data.size());

    ORC_ProceduralPrimitiveFactory::generate_vertex_array_from_data(primitive, 4, true);

    return primitive;
}

Ref<ORC_ProceduralPrimitive> ORC_ProceduralPrimitiveFactory::create_inverted_sphere(int64_t stacks, int64_t slices) {
    Ref<ORC_ProceduralPrimitive> primitive;
    primitive.instantiate();

    PackedVector3Array positions;
    PackedInt32Array indices;


        for (int64_t i = 0; i <= stacks; i++) {
            float v = static_cast<float>(i) / static_cast<float>(stacks);
            float theta = v * Math_PI;
            float sin_theta = Math::sin(theta);
            float cos_theta = Math::cos(theta);

            for (int64_t j = 0; j <= slices; j++) {
                float u = static_cast<float>(j) / static_cast<float>(slices);
                float phi = u * Math_TAU;

                float x = sin_theta * Math::cos(phi);
                float y = cos_theta;
                float z = sin_theta * Math::sin(phi);

                positions.append(Vector3(x, y, z));
            }
        }

        for (int64_t i = 0; i < stacks; i++) {
            for (int64_t j = 0; j < slices; j++) {
                int64_t first = i * (slices + 1) + j;
                int64_t second = first + slices + 1;

                indices.append(first);
                indices.append(second + 1);
                indices.append(second);

                indices.append(first);
                indices.append(first + 1);
                indices.append(second + 1);
            }
        }

    PackedByteArray byte_array = positions.to_byte_array();
    primitive->position_buffer = ORC_RDHelper::get_rd()->vertex_buffer_create(byte_array.size(), byte_array);

    byte_array = indices.to_byte_array();
    primitive->index_buffer = ORC_RDHelper::get_rd()->index_buffer_create(indices.size(), RenderingDevice::INDEX_BUFFER_FORMAT_UINT32, byte_array);
    primitive->index_array = ORC_RDHelper::get_rd()->index_array_create(primitive->index_buffer, 0, indices.size());

    ORC_ProceduralPrimitiveFactory::generate_vertex_array_from_data(primitive, positions.size());

    return primitive;
}

Ref<ORC_ProceduralPrimitive> ORC_ProceduralPrimitiveFactory::create_inverted_cone(int64_t slices) {
    Ref<ORC_ProceduralPrimitive> primitive;
    primitive.instantiate();

    PackedVector3Array positions;
    PackedInt32Array indices;


    positions.append(Vector3(0, 0, 0));
    int64_t apex_index = 0;

    for (int64_t j = 0; j <= slices; j++) {
        float u = static_cast<float>(j) / static_cast<float>(slices);
        float angle = u * Math_TAU;
        float x = Math::cos(angle);
        float y = Math::sin(angle);
        float z = -1.0f;

        positions.append(Vector3(x, y, z));
    }

    for (int64_t j = 0; j < slices; j++) {
        int64_t i0 = apex_index;
        int64_t i1 = j + 1;
        int64_t i2 = j + 2;

        indices.append(i0);
        indices.append(i2);
        indices.append(i1);
    }

    int64_t base_center_index = positions.size();
    positions.append(Vector3(0, 0, -1));

    for (int64_t j = 0; j < slices; j++) {
        int64_t i0 = base_center_index;
        int64_t i1 = j + 1;
        int64_t i2 = j + 2;

        indices.append(i0);
        indices.append(i1);
        indices.append(i2);
    }

    PackedByteArray byte_array = positions.to_byte_array();
    primitive->position_buffer = ORC_RDHelper::get_rd()->vertex_buffer_create(byte_array.size(), byte_array);

    byte_array = indices.to_byte_array();
    primitive->index_buffer = ORC_RDHelper::get_rd()->index_buffer_create(indices.size(), RenderingDevice::INDEX_BUFFER_FORMAT_UINT32, byte_array);
    primitive->index_array = ORC_RDHelper::get_rd()->index_array_create(primitive->index_buffer, 0, indices.size());

    ORC_ProceduralPrimitiveFactory::generate_vertex_array_from_data(primitive, positions.size());

    return primitive;
}

void ORC_ProceduralPrimitiveFactory::free_rids(Ref<ORC_ProceduralPrimitive> primitive) {
    if (!primitive.is_valid()) ERR_FAIL_MSG(ERR_PPF_INVALID_PRIMITIVE);

    RenderingDevice* rd = ORC_RDHelper::get_rd();

    // TODO : try to implement a safe_free_rid in RDHelper to avoid checking is_valid every time
    if (primitive->index_array.is_valid()) rd->free_rid(primitive->index_array);
    if (primitive->index_buffer.is_valid()) rd->free_rid(primitive->index_buffer);

    if (primitive->vertex_array.is_valid()) rd->free_rid(primitive->vertex_array);
    if (primitive->position_buffer.is_valid()) rd->free_rid(primitive->position_buffer);
    if (primitive->normal_buffer.is_valid()) rd->free_rid(primitive->normal_buffer);
    if (primitive->tangent_buffer.is_valid()) rd->free_rid(primitive->tangent_buffer);
    if (primitive->color_buffer.is_valid()) rd->free_rid(primitive->color_buffer);
    if (primitive->uv_buffer.is_valid()) rd->free_rid(primitive->uv_buffer);
    if (primitive->uv2_buffer.is_valid()) rd->free_rid(primitive->uv2_buffer);
    if (primitive->bones_buffer.is_valid()) rd->free_rid(primitive->bones_buffer);
    if (primitive->weights_buffer.is_valid()) rd->free_rid(primitive->weights_buffer);
}