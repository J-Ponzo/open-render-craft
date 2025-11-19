#ifndef ORC_PROXYQUEUE_H
#define ORC_PROXYQUEUE_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/typed_array.hpp>
#include <godot_cpp/variant/string_name.hpp>
#include <proxy_data.h>
#include <processor_pipeline.h>
#include <macros.h>

namespace godot { class ORC_SceneProxyBase; }

#ifdef ORC_RENDERER_EXPORTS
#define ORC_API __declspec(dllexport)
#else
#define ORC_API __declspec(dllimport)
#endif

namespace godot {

class ORC_API ORC_ProxyQueue : public RefCounted {
    GDCLASS(ORC_ProxyQueue, RefCounted)

private:
    Ref<ORC_ProcessorPipeline> pipeline;
    Ref<ORC_ProxyQueue> parent_queue;

protected:
    static void _bind_methods();

public:
    ORC_ProxyQueue();
    
    Ref<ORC_ProcessorPipeline> get_pipeline() const { return pipeline; }
    
    void set_parent(const Ref<ORC_ProxyQueue>& parent);
    Ref<ORC_ProxyQueue> get_parent() const { return parent_queue; }
    
    void execute();
    TypedArray<ORC_ProxyData> get_cached_data() const;
};

}

#endif // ORC_PROXYQUEUE_H
