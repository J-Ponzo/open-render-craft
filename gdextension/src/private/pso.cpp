#include <pso.h>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

ORC_PSO::ORC_PSO() {
}

ORC_PSO::~ORC_PSO() {
}

void ORC_PSO::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_shader_program"), &ORC_PSO::get_shader_program);
    ClassDB::bind_method(D_METHOD("set_shader_program", "shader_program"), &ORC_PSO::set_shader_program);
    ADD_PROPERTY(PropertyInfo(Variant::RID, "shader_program"), "set_shader_program", "get_shader_program");

    ClassDB::bind_method(D_METHOD("get_pipeline"), &ORC_PSO::get_pipeline);
    ClassDB::bind_method(D_METHOD("set_pipeline", "pipeline"), &ORC_PSO::set_pipeline);
    ADD_PROPERTY(PropertyInfo(Variant::RID, "pipeline"), "set_pipeline", "get_pipeline");

    ClassDB::bind_method(D_METHOD("get_vertex_format"), &ORC_PSO::get_vertex_format);
    ClassDB::bind_method(D_METHOD("set_vertex_format", "vertex_format"), &ORC_PSO::set_vertex_format);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "vertex_format"), "set_vertex_format", "get_vertex_format");
}
