#ifndef ORC_PSO_FACTORY_H
#define ORC_PSO_FACTORY_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <unordered_map>
#include <macros.h>
#include <pso.h>
#include <proxy_data.h>
#include <rd_helper.h>
#include <shader_preprocessor.h>
#include <render_pass_base.h>

namespace godot {

struct PSOKey {
    int64_t flags_mask;
    int64_t vertex_format;
    
    bool operator==(const PSOKey& other) const {
        return flags_mask == other.flags_mask && vertex_format == other.vertex_format;
    }
};

struct PSOKeyHash {
    std::size_t operator()(const PSOKey& key) const noexcept {
        return static_cast<std::size_t>(key.flags_mask ^ (key.vertex_format * 0x9e3779b97f4a7c15ULL));
    }
};

class ORC_PSOFactory : public RefCounted {
    GDCLASS(ORC_PSOFactory, RefCounted)

protected:
    static void _bind_methods();

private:
    std::unordered_map<PSOKey, Ref<ORC_PSO>, PSOKeyHash> pso_lookup;

public:
    ORC_PSOFactory();
    ~ORC_PSOFactory();

    Ref<ORC_RenderPassBase> render_pass;
	Ref<ORC_RenderPassBase> get_render_pass() const { return render_pass; }
	void set_render_pass(const Ref<ORC_RenderPassBase>& render_pass) { this->render_pass = render_pass; }

    String uber_vertex_shader_src;
    String get_uber_vertex_shader_src() const { return uber_vertex_shader_src; }
    void set_uber_vertex_shader_src(const String& src) { uber_vertex_shader_src = src; }

    String uber_fragment_shader_src;
    String get_uber_fragment_shader_src() const { return uber_fragment_shader_src; }
    void set_uber_fragment_shader_src(const String& src) { uber_fragment_shader_src = src; }

    Ref<ORC_PSO> get_or_create_pso(const int64_t flags_mask,  const int64_t vertex_format);
    DECLARE_GD_OVERRIDABLE_METHOD(Ref<ORC_PSO>, create_pso, const int64_t flags_mask,  const int64_t vertex_format, const String&, const String&)
    void cleanup();
};

}

#endif
