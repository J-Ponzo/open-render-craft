#ifndef ORC_PSO_H
#define ORC_PSO_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/rid.hpp>

#ifdef ORC_RENDERER_EXPORTS
#define ORC_API __declspec(dllexport)
#else
#define ORC_API __declspec(dllimport)
#endif

namespace godot {

class ORC_API ORC_PSO : public RefCounted {
    GDCLASS(ORC_PSO, RefCounted)

protected:
    static void _bind_methods();

public:
    ORC_PSO();
    ~ORC_PSO();

    RID shader_program;
    RID get_shader_program() const { return shader_program; }
    void set_shader_program(const RID &shader_program) { this->shader_program = shader_program; }

    RID pipeline;
    RID get_pipeline() const { return pipeline; }
    void set_pipeline(const RID &pipeline) { this->pipeline = pipeline; }

    int vertex_format = -1;
    int get_vertex_format() const { return vertex_format; }
    void set_vertex_format(int vertex_format) { this->vertex_format = vertex_format; }
};

}

#endif
