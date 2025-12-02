#ifndef ORC_RENDER_PASS_BASE_H
#define ORC_RENDER_PASS_BASE_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/rid.hpp>
#include <macros.h>
#include <renderer_base.h>

namespace godot {

class ORC_RendererBase;

class ORC_RenderPassBase : public RefCounted {
    GDCLASS(ORC_RenderPassBase, RefCounted)

protected:
    static void _bind_methods();

public:
    Ref<ORC_RendererBase> renderer;
    Ref<ORC_RendererBase> get_renderer() const { return renderer; }
    void set_renderer(const Ref<ORC_RendererBase> &renderer) { this->renderer = renderer; }

    Dictionary explicits_pso;
    Dictionary get_explicits_pso() const { return explicits_pso; }
    void set_explicits_pso(const Dictionary &explicits_pso) { this->explicits_pso = explicits_pso; }

    int64_t framebuffer_format = -1;
    int64_t get_framebuffer_format() const { return framebuffer_format; }
    void set_framebuffer_format(int64_t framebuffer_format) { this->framebuffer_format = framebuffer_format; }

    RID framebuffer;
    RID get_framebuffer() const { return framebuffer; }
    void set_framebuffer(const RID &framebuffer) { this->framebuffer = framebuffer; }

    DECLARE_GD_OVERRIDABLE_METHOD(void, setup)
    DECLARE_GD_OVERRIDABLE_METHOD(void, render)
    DECLARE_GD_OVERRIDABLE_METHOD(void, cleanup)
};

}

#endif
