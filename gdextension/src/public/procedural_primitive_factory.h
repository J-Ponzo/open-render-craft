#ifndef ORC_PROCEDURAL_PRIMITIVE_FACTORY_H
#define ORC_PROCEDURAL_PRIMITIVE_FACTORY_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/rid.hpp>

namespace godot {

class ORC_ProceduralPrimitive : public RefCounted {
    GDCLASS(ORC_ProceduralPrimitive, RefCounted)

friend class ORC_ProceduralPrimitiveFactory;

protected:
    static void _bind_methods();

private:
    RID index_buffer;
    RID index_array;

    RID position_buffer;
    RID normal_buffer;
    RID tangent_buffer;
    RID color_buffer;
    RID uv_buffer;
    RID uv2_buffer;
    RID bones_buffer;
    RID weights_buffer;
    RID vertex_array;

public:
    ORC_ProceduralPrimitive();
    ~ORC_ProceduralPrimitive();

    RID get_index_buffer() const;
    RID get_index_array() const;
    RID get_position_buffer() const;
    RID get_normal_buffer() const;
    RID get_tangent_buffer() const;
    RID get_color_buffer() const;
    RID get_uv_buffer() const;
    RID get_uv2_buffer() const;
    RID get_bones_buffer() const;
    RID get_weights_buffer() const;
    RID get_vertex_array() const;
};

// TODO : rename to ORC_ProceduralPrimitiveHelper ?
class ORC_ProceduralPrimitiveFactory : public RefCounted {
    GDCLASS(ORC_ProceduralPrimitiveFactory, RefCounted)

protected:
    static void _bind_methods();

public:
    ORC_ProceduralPrimitiveFactory();
    ~ORC_ProceduralPrimitiveFactory();

    static Ref<ORC_ProceduralPrimitive> create_screen_quad(bool setup_uv = false);
};

} // namespace godot

#endif // ORC_PROCEDURAL_PRIMITIVE_FACTORY_H