#ifndef ORC_PROXYQUEUE_H
#define ORC_PROXYQUEUE_H

#include <godot_cpp/variant/typed_array.hpp>
#include <godot_cpp/variant/string_name.hpp>
#include <proxy_data.h>
#include <queue_processor.h>
#include <proxy_registry.h>
#include <macros.h>

namespace godot { class ORC_SceneProxyBase; }

namespace godot {

class ORC_ProxyQueue {
private:
    ORC_SceneProxyBase* scene_proxy;
    Ref<ORC_DataQuery> init_query;
    TypedArray<ORC_QueueProcessor> processors;
    TypedArray<ORC_ProxyData> cached_result;

public:
    ORC_ProxyQueue(ORC_SceneProxyBase* scene_proxy, const Ref<ORC_DataQuery>& init_query);

    void add_processor(const Ref<ORC_QueueProcessor>& processor);
    
    void execute();
    TypedArray<ORC_ProxyData> get_cached_data() const { return cached_result; }
};

}

#endif // ORC_PROXYQUEUE_H
