#include <proxy_queue.h>
#include <scene_proxy_base.h>

using namespace godot;

ORC_ProxyQueue::ORC_ProxyQueue(ORC_SceneProxyBase* scene_proxy, const Ref<ORC_DataQuery>& init_query) 
    : scene_proxy(scene_proxy), init_query(init_query) {
}

void ORC_ProxyQueue::add_processor(const Ref<ORC_QueueProcessor>& processor) {
    if (!processor.is_valid()) {
        ERR_FAIL_MSG("[ORC_ProxyQueue ERROR] : Cannot add null processor");
    }
    processors.append(processor);
}

void ORC_ProxyQueue::execute() {
	TypedArray<ORC_ProxyData> current_data = scene_proxy->get_by_query(init_query);
    
    for (int i = 0; i < processors.size(); i++) {
        Ref<ORC_QueueProcessor> processor = processors[i];
        if (!processor.is_valid()) {
            ERR_PRINT("[ORC_ProxyQueue WARNING] : Skipping invalid processor at index " + String::num_int64(i));
            continue;
        }
        
        current_data = processor->process(current_data);
    }
    
    cached_result = current_data;
}