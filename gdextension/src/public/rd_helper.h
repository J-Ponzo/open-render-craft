#ifndef ORC_RD_HELPER_H
#define ORC_RD_HELPER_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/rendering_device.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/dictionary.hpp>

#ifdef ORC_RENDERER_EXPORTS
#define ORC_API __declspec(dllexport)
#else
#define ORC_API __declspec(dllimport)
#endif

namespace godot {

class ORC_API ORC_RDHelper : public RefCounted {
    GDCLASS(ORC_RDHelper, RefCounted)

protected:
    static void _bind_methods();

public:
    RenderingDevice* rd;

    ORC_RDHelper();
    ~ORC_RDHelper();

    RenderingDevice* get_rd() const;
    
    static int64_t create_vertex_format(const Ref<Resource>& vertex_format_def);

private:
    // Constants for vertex format calculation
    static constexpr int SIZEOF_FLOAT = 4;
    static constexpr int SIZEOF_INT = 4;
    static constexpr int POSITION_2D_NB_FLOATS = 2;
    static constexpr int POSITION_3D_NB_FLOATS = 3;
    static constexpr int NORMAL_NB_FLOATS = 3;
    static constexpr int TANGENT_NB_FLOATS = 4;
    static constexpr int COLOR_NB_FLOATS = 4;
    static constexpr int UV_NB_FLOATS = 2;
    static constexpr int UV2_NB_FLOATS = 2;
    static constexpr int BONES_NB_INTS = 4;
    static constexpr int WEIGHT_NB_FLOATS = 4;
};

}

#endif
