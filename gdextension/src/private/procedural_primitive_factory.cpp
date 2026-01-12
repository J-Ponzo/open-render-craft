#include <procedural_primitive_factory.h>

using namespace godot;

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
}

ORC_ProceduralPrimitiveFactory::ORC_ProceduralPrimitiveFactory() {}
ORC_ProceduralPrimitiveFactory::~ORC_ProceduralPrimitiveFactory() {}

Ref<ORC_ProceduralPrimitive> ORC_ProceduralPrimitiveFactory::create_screen_quad(bool setup_uv) {
    Ref<ORC_ProceduralPrimitive> primitive;
    primitive.instantiate();

    // Implementation to create a screen quad goes here.
    // This typically involves creating vertex buffers, index buffers, and setting up the vertex array.

    return primitive;
}