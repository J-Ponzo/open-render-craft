#include <render_pass_base.h>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void ORC_RenderPassBase::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_renderer"), &ORC_RenderPassBase::get_renderer);
    ClassDB::bind_method(D_METHOD("set_renderer", "renderer"), &ORC_RenderPassBase::set_renderer);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "renderer", PROPERTY_HINT_RESOURCE_TYPE, "ORC_RendererBase"), "set_renderer", "get_renderer");

    ClassDB::bind_method(D_METHOD("get_direct_psos"), &ORC_RenderPassBase::get_direct_psos);
    ClassDB::bind_method(D_METHOD("set_direct_psos", "direct_psos"), &ORC_RenderPassBase::set_direct_psos);
    ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "direct_psos"), "set_direct_psos", "get_direct_psos");

    ClassDB::bind_method(D_METHOD("get_pso_factories"), &ORC_RenderPassBase::get_pso_factories);
    ClassDB::bind_method(D_METHOD("set_pso_factories", "pso_factories"), &ORC_RenderPassBase::set_pso_factories);
    ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "pso_factories"), "set_pso_factories", "get_pso_factories");

    ClassDB::bind_method(D_METHOD("get_framebuffer_format"), &ORC_RenderPassBase::get_framebuffer_format);
    ClassDB::bind_method(D_METHOD("set_framebuffer_format", "framebuffer_format"), &ORC_RenderPassBase::set_framebuffer_format);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "framebuffer_format"), "set_framebuffer_format", "get_framebuffer_format");

    ClassDB::bind_method(D_METHOD("get_framebuffer"), &ORC_RenderPassBase::get_framebuffer);
    ClassDB::bind_method(D_METHOD("set_framebuffer", "framebuffer"), &ORC_RenderPassBase::set_framebuffer);
    ADD_PROPERTY(PropertyInfo(Variant::RID, "framebuffer"), "set_framebuffer", "get_framebuffer");

    BIND_GD_OVERRIDABLE_METHOD(ORC_RenderPassBase, setup)
    BIND_GD_OVERRIDABLE_METHOD(ORC_RenderPassBase, render)
    BIND_GD_OVERRIDABLE_METHOD(ORC_RenderPassBase, cleanup)
}

DEFINE_GD_OVERRIDABLE_METHOD_0_ARGS(ORC_RenderPassBase, void, setup)
void ORC_RenderPassBase::setup_impl() {
}

DEFINE_GD_OVERRIDABLE_METHOD_0_ARGS(ORC_RenderPassBase, void, render)
void ORC_RenderPassBase::render_impl() {
}

DEFINE_GD_OVERRIDABLE_METHOD_0_ARGS(ORC_RenderPassBase, void, cleanup)
void ORC_RenderPassBase::cleanup_impl() {
}
