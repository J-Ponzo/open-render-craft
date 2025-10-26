#include <render_pass_base.h>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void ORC_RenderPassBase::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_renderer"), &ORC_RenderPassBase::get_renderer);
    ClassDB::bind_method(D_METHOD("set_renderer", "renderer"), &ORC_RenderPassBase::set_renderer);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "renderer", PROPERTY_HINT_RESOURCE_TYPE, "ORC_RendererBase"), "set_renderer", "get_renderer");

    BIND_GD_OVERRIDABLE_METHOD(ORC_RenderPassBase, setup)
    BIND_GD_OVERRIDABLE_METHOD(ORC_RenderPassBase, render)
    BIND_GD_OVERRIDABLE_METHOD(ORC_RenderPassBase, cleanup)
}

DEFINE_GD_OVERRIDABLE_METHOD_0_ARGS(ORC_RenderPassBase, void, setup)
void ORC_RenderPassBase::setup_impl() {
    UtilityFunctions::print("ORC_RenderPassBase.setup");
}

DEFINE_GD_OVERRIDABLE_METHOD_0_ARGS(ORC_RenderPassBase, void, render)
void ORC_RenderPassBase::render_impl() {
    UtilityFunctions::print("ORC_RenderPassBase.render");
}

DEFINE_GD_OVERRIDABLE_METHOD_0_ARGS(ORC_RenderPassBase, void, cleanup)
void ORC_RenderPassBase::cleanup_impl() {
    UtilityFunctions::print("ORC_RenderPassBase.cleanup");
}
