#ifndef ORC_PROXYQUEUE_H
#define ORC_PROXYQUEUE_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/typed_array.hpp>
#include <godot_cpp/variant/string_name.hpp>
#include <proxy_data.h>
#include <queue_processor.h>
#include <proxy_registry.h>
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
    Ref<ORC_SceneProxyBase> scene_proxy;
    Ref<ORC_DataQuery> init_query;
    TypedArray<ORC_QueueProcessor> processors;
    TypedArray<ORC_ProxyData> cached_result;

protected:
    static void _bind_methods();

public:
    ORC_ProxyQueue();
    
    void set_scene_proxy(const Ref<ORC_SceneProxyBase>& scene_proxy);
    
    void set_init_query(const Ref<ORC_DataQuery>& query) { init_query = query; }

    void add_processor(const Ref<ORC_QueueProcessor>& processor);
    void clear_processors();
    
    void execute();
    TypedArray<ORC_ProxyData> get_cached_data() const { return cached_result; }
};

}

#endif // ORC_PROXYQUEUE_H
