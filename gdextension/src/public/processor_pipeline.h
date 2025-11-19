#ifndef ORC_PROCESSORPIPELINE_H
#define ORC_PROCESSORPIPELINE_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/typed_array.hpp>
#include <proxy_data.h>
#include <queue_processor.h>
#include <macros.h>

namespace godot { class ORC_SceneProxyBase; }

#ifdef ORC_RENDERER_EXPORTS
#define ORC_API __declspec(dllexport)
#else
#define ORC_API __declspec(dllimport)
#endif

namespace godot {

class ORC_API ORC_ProcessorPipeline : public RefCounted {
    GDCLASS(ORC_ProcessorPipeline, RefCounted)

private:
    TypedArray<ORC_QueueProcessor> processors;
    TypedArray<ORC_ProxyData> cached_result;

protected:
    static void _bind_methods();

public:
    void add_processor(const Ref<ORC_QueueProcessor>& processor);
    void clear_processors();
    
    void execute(const TypedArray<ORC_ProxyData>& input);
    
    TypedArray<ORC_ProxyData> get_cached_data() const { return cached_result; }
};

}

#endif // ORC_PROCESSORPIPELINE_H
