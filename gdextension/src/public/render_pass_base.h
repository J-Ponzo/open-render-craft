#ifndef ORC_RENDER_PASS_BASE_H
#define ORC_RENDER_PASS_BASE_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <macros.h>
#include <renderer_base.h>

#ifdef ORC_RENDERER_EXPORTS
#define ORC_API __declspec(dllexport)
#else
#define ORC_API __declspec(dllimport)
#endif

namespace godot {

class ORC_RendererBase;

class ORC_API ORC_RenderPassBase : public RefCounted {
    GDCLASS(ORC_RenderPassBase, RefCounted)

protected:
    static void _bind_methods();

public:
    Ref<ORC_RendererBase> renderer;
    Ref<ORC_RendererBase> get_renderer() { return renderer; }
    void set_renderer(Ref<ORC_RendererBase> renderer) { this->renderer = renderer; }

    DECLARE_GD_OVERRIDABLE_METHOD(void, setup)
    DECLARE_GD_OVERRIDABLE_METHOD(void, render)
    DECLARE_GD_OVERRIDABLE_METHOD(void, cleanup)
};

}

#endif
